
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#define NGX_SSL_CACHE_PATH    0


#define ngx_ssl_cache_get_conf(cycle)                                         \
    (ngx_ssl_cache_t *) ngx_get_conf(cycle->conf_ctx, ngx_openssl_cache_module)

#define ngx_ssl_cache_get_old_conf(cycle)                                     \
    cycle->old_cycle->conf_ctx ? ngx_ssl_cache_get_conf(cycle->old_cycle)     \
                               : NULL


typedef struct {
    unsigned                    type:2;
    unsigned                    len:30;
    u_char                     *data;
} ngx_ssl_cache_key_t;


typedef void *(*ngx_ssl_cache_create_pt)(ngx_ssl_cache_key_t *id, char **err,
    void *data);
typedef void (*ngx_ssl_cache_free_pt)(void *data);
typedef void *(*ngx_ssl_cache_ref_pt)(char **err, void *data);


typedef struct {
    ngx_ssl_cache_create_pt     create;
    ngx_ssl_cache_free_pt       free;
    ngx_ssl_cache_ref_pt        ref;
} ngx_ssl_cache_type_t;


typedef struct {
    ngx_rbtree_node_t           node;
    ngx_ssl_cache_key_t         id;
    ngx_ssl_cache_type_t       *type;
    void                       *value;

    time_t                      mtime;
    ngx_file_uniq_t             uniq;
} ngx_ssl_cache_node_t;


typedef struct {
    ngx_rbtree_t                rbtree;
    ngx_rbtree_node_t           sentinel;

    ngx_flag_t                  inheritable;
} ngx_ssl_cache_t;


static ngx_int_t ngx_ssl_cache_init_key(ngx_pool_t *pool, ngx_uint_t index,
    ngx_str_t *path, ngx_ssl_cache_key_t *id);
static ngx_ssl_cache_node_t *ngx_ssl_cache_lookup(ngx_ssl_cache_t *cache,
    ngx_ssl_cache_type_t *type, ngx_ssl_cache_key_t *id, uint32_t hash);

static void *ngx_openssl_cache_create_conf(ngx_cycle_t *cycle);
static char *ngx_openssl_cache_init_conf(ngx_cycle_t *cycle, void *conf);
static void ngx_ssl_cache_cleanup(void *data);
static void ngx_ssl_cache_node_insert(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);


static ngx_command_t  ngx_openssl_cache_commands[] = {

    { ngx_string("ssl_object_cache_inheritable"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_ssl_cache_t, inheritable),
      NULL },

      ngx_null_command
};


static ngx_core_module_t  ngx_openssl_cache_module_ctx = {
    ngx_string("openssl_cache"),
    ngx_openssl_cache_create_conf,
    ngx_openssl_cache_init_conf
};


ngx_module_t  ngx_openssl_cache_module = {
    NGX_MODULE_V1,
    &ngx_openssl_cache_module_ctx,         /* module context */
    ngx_openssl_cache_commands,            /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_ssl_cache_type_t  ngx_ssl_cache_types[] = {

};


void *
ngx_ssl_cache_fetch(ngx_conf_t *cf, ngx_uint_t index, char **err,
    ngx_str_t *path, void *data)
{
    void                  *value;
    time_t                 mtime;
    uint32_t               hash;
    ngx_int_t              rc;
    ngx_file_uniq_t        uniq;
    ngx_file_info_t        fi;
    ngx_ssl_cache_t       *cache, *old_cache;
    ngx_ssl_cache_key_t    id;
    ngx_ssl_cache_type_t  *type;
    ngx_ssl_cache_node_t  *cn;

    *err = NULL;

    if (ngx_ssl_cache_init_key(cf->pool, index, path, &id) != NGX_OK) {
        return NULL;
    }

    cache = (ngx_ssl_cache_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                             ngx_openssl_cache_module);

    type = &ngx_ssl_cache_types[index];
    hash = ngx_murmur_hash2(id.data, id.len);

    cn = ngx_ssl_cache_lookup(cache, type, &id, hash);

    if (cn != NULL) {
        return type->ref(err, cn->value);
    }

    value = NULL;

    if (id.type == NGX_SSL_CACHE_PATH
        && (rc = ngx_file_info(id.data, &fi)) != NGX_FILE_ERROR)
    {
        mtime = ngx_file_mtime(&fi);
        uniq = ngx_file_uniq(&fi);

    } else {
        rc = NGX_FILE_ERROR;
        mtime = 0;
        uniq = 0;
    }

    /* try to use a reference from the old cycle */

    old_cache = ngx_ssl_cache_get_old_conf(cf->cycle);

    if (old_cache && old_cache->inheritable) {
        cn = ngx_ssl_cache_lookup(old_cache, type, &id, hash);

        if (cn != NULL) {
            switch (id.type) {

            case NGX_SSL_CACHE_DATA:
                value = type->ref(err, cn->value);
                break;

            default:
                if (rc != NGX_FILE_ERROR
                    && uniq == cn->uniq && mtime == cn->mtime)
                {
                    value = type->ref(err, cn->value);
                }
                break;
            }
        }
    }

    if (value == NULL) {
        value = type->create(&id, err, data);
        if (value == NULL) {
            return NULL;
        }
    }

    cn = ngx_palloc(cf->pool, sizeof(ngx_ssl_cache_node_t) + id.len + 1);
    if (cn == NULL) {
        type->free(value);
        return NULL;
    }

    cn->node.key = hash;
    cn->id.data = (u_char *)(cn + 1);
    cn->id.len = id.len;
    cn->id.type = id.type;
    cn->type = type;
    cn->value = value;
    cn->mtime = mtime;
    cn->uniq = uniq;

    ngx_cpystrn(cn->id.data, id.data, id.len + 1);

    ngx_rbtree_insert(&cache->rbtree, &cn->node);

    return type->ref(err, cn->value);
}


void *
ngx_ssl_cache_connection_fetch(ngx_pool_t *pool, ngx_uint_t index, char **err,
    ngx_str_t *path, void *data)
{
    ngx_ssl_cache_key_t  id;

    *err = NULL;

    if (ngx_ssl_cache_init_key(pool, index, path, &id) != NGX_OK) {
        return NULL;
    }

    return ngx_ssl_cache_types[index].create(&id, err, data);
}


static ngx_int_t
ngx_ssl_cache_init_key(ngx_pool_t *pool, ngx_uint_t index, ngx_str_t *path,
    ngx_ssl_cache_key_t *id)
{
    if (ngx_get_full_name(pool, (ngx_str_t *) &ngx_cycle->conf_prefix, path)
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    id->type = NGX_SSL_CACHE_PATH;

    id->len = path->len;
    id->data = path->data;

    return NGX_OK;
}


static ngx_ssl_cache_node_t *
ngx_ssl_cache_lookup(ngx_ssl_cache_t *cache, ngx_ssl_cache_type_t *type,
    ngx_ssl_cache_key_t *id, uint32_t hash)
{
    ngx_int_t              rc;
    ngx_rbtree_node_t     *node, *sentinel;
    ngx_ssl_cache_node_t  *cn;

    node = cache->rbtree.root;
    sentinel = cache->rbtree.sentinel;

    while (node != sentinel) {

        if (hash < node->key) {
            node = node->left;
            continue;
        }

        if (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        cn = (ngx_ssl_cache_node_t *) node;

        if (type < cn->type) {
            node = node->left;
            continue;
        }

        if (type > cn->type) {
            node = node->right;
            continue;
        }

        /* type == cn->type */

        rc = ngx_memn2cmp(id->data, cn->id.data, id->len, cn->id.len);

        if (rc == 0) {
            return cn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


static void *
ngx_openssl_cache_create_conf(ngx_cycle_t *cycle)
{
    ngx_ssl_cache_t     *cache;
    ngx_pool_cleanup_t  *cln;

    cache = ngx_pcalloc(cycle->pool, sizeof(ngx_ssl_cache_t));
    if (cache == NULL) {
        return NULL;
    }

    cache->inheritable = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    if (cln == NULL) {
        return NULL;
    }

    cln->handler = ngx_ssl_cache_cleanup;
    cln->data = cache;

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_ssl_cache_node_insert);

    return cache;
}


static char *
ngx_openssl_cache_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_ssl_cache_t *cache = conf;

    ngx_conf_init_value(cache->inheritable, 1);

    return NGX_CONF_OK;
}


static void
ngx_ssl_cache_cleanup(void *data)
{
    ngx_ssl_cache_t  *cache = data;

    ngx_rbtree_t          *tree;
    ngx_rbtree_node_t     *node;
    ngx_ssl_cache_node_t  *cn;

    tree = &cache->rbtree;

    if (tree->root == tree->sentinel) {
        return;
    }

    for (node = ngx_rbtree_min(tree->root, tree->sentinel);
         node;
         node = ngx_rbtree_next(tree, node))
    {
        cn = ngx_rbtree_data(node, ngx_ssl_cache_node_t, node);
        cn->type->free(cn->value);
    }
}


static void
ngx_ssl_cache_node_insert(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t     **p;
    ngx_ssl_cache_node_t   *n, *t;

    for ( ;; ) {

        n = ngx_rbtree_data(node, ngx_ssl_cache_node_t, node);
        t = ngx_rbtree_data(temp, ngx_ssl_cache_node_t, node);

        if (node->key != temp->key) {

            p = (node->key < temp->key) ? &temp->left : &temp->right;

        } else if (n->type != t->type) {

            p = (n->type < t->type) ? &temp->left : &temp->right;

        } else {

            p = (ngx_memn2cmp(n->id.data, t->id.data, n->id.len, t->id.len)
                 < 0) ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}
