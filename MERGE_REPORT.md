## Tengine 升级合并报告：nginx-1.24.0 → nginx-1.30

**合并时间**: 2026-05-09
**合并方式**: 构造虚拟祖先 + diff3 三方合并
**合并仓库**: `/data/vscode/opensource/tengine-upgrade` (tengine 分支)

---

### 一、合并总览

| 指标 | 数值 |
|------|------|
| 合并前 tengine src 文件数 | 337 |
| 合并后 src 文件数 | 396 |
| nginx-1.30 新增 src 文件 | 53 |
| tengine 独有 src 文件（保留） | 19 |
| tengine modules 模块数（保留） | 32 |
| 自动合并（无冲突）文件 | 大部分 |
| 需手动解决的冲突文件 | 44 |
| 冲突块总数 | 96 |
| T_ 宏块总数（合并前） | 621 |
| T_ 宏块总数（合并后） | **621 ✅ 完全一致** |

---

### 二、冲突解决详情

#### A类：modify/delete 冲突（11个文件）

tengine 原本删除了这些文件，但 nginx-1.30 有更新。**按用户要求，已全部保留 nginx-1.30 版本**。

| 文件 | 处理方式 |
|------|----------|
| `docs/html/index.html` | 保留 nginx-1.30 版本 |
| `docs/xml/nginx/changes.xml` | 保留 nginx-1.30 版本 |
| `misc/GNUmakefile` | 保留 nginx-1.30 版本 |
| `misc/README` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_errno.h` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_files.h` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_process_cycle.c` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_shmem.c` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_socket.h` | 保留 nginx-1.30 版本 |
| `src/os/win32/ngx_win32_config.h` | 保留 nginx-1.30 版本 |
| `tests/nginx-tests/nginx-tests/LICENSE` | 保留 tengine 版本 |

#### B类：非核心代码冲突（5个文件）

| 文件 | 冲突块数 | 处理方式 |
|------|---------|----------|
| `.gitignore` | 1 | 合并两边：nginx-1.30 规则 + tengine 测试临时文件 |
| `LICENSE` | 1 | 保留 tengine 版本（含 Alibaba 等多方版权声明） |
| `auto/make` | 1 | nginx-1.30 新增 v3 目录 + tengine lua 模块目录 |
| `contrib/vim/syntax/nginx.vim` | 14 | 合并两边的语法高亮关键词 |
| `man/nginx.8` | 2 | 使用 nginx-1.30 版本 |

#### C类：核心源码冲突（28个文件，81个冲突块）

所有核心源码冲突都通过 diff3 三方合并 + 智能冲突解决处理：
- **策略**: 以 nginx-1.30 为基础代码，精确保留 tengine 的 T_ 宏块
- **验证**: 所有 621 个 T_ 宏块完全保留，无丢失

| 文件 | 冲突块数 | T_ 宏块验证 |
|------|---------|------------|
| `src/core/ngx_core.h` | 1 | ✅ |
| `src/event/ngx_event_openssl.c` | 2 | ✅ 17个T_块完整 |
| `src/event/ngx_event_openssl.h` | 3 | ✅ 6个T_块完整 |
| `src/http/modules/ngx_http_grpc_module.c` | 1 | ✅ 5个T_块完整 |
| `src/http/modules/ngx_http_proxy_module.c` | 2 | ✅ 11个T_块完整 |
| `src/http/modules/ngx_http_ssl_module.c` | 11 | ✅ 21个T_块完整 |
| `src/http/modules/ngx_http_ssl_module.h` | 2 | ✅ |
| `src/http/modules/ngx_http_upstream_ip_hash_module.c` | 1 | ✅ |
| `src/http/modules/ngx_http_uwsgi_module.c` | 1 | ✅ 6个T_块完整 |
| `src/http/ngx_http.c` | 6 | ✅ |
| `src/http/ngx_http.h` | 1 | ✅ |
| `src/http/ngx_http_core_module.c` | 2 | ✅ 38个T_块完整 |
| `src/http/ngx_http_parse.c` | 1 | ✅ |
| `src/http/ngx_http_request.c` | 6 | ✅ 29个T_块完整 |
| `src/http/ngx_http_request.h` | 2 | ✅ |
| `src/http/ngx_http_request_body.c` | 4 | ✅ |
| `src/http/ngx_http_upstream.c` | 3 | ✅ 42个T_块完整 |
| `src/http/ngx_http_upstream.h` | 1 | ✅ 7个T_块完整 |
| `src/http/ngx_http_upstream_round_robin.c` | 1 | ✅ |
| `src/http/ngx_http_variables.c` | 1 | ✅ |
| `src/http/v2/ngx_http_v2_filter_module.c` | 1 | ✅ 4个T_块完整 |
| `src/http/v2/ngx_http_v2_module.h` | 1 | ✅ |
| `src/stream/ngx_stream.c` | 7 | ✅ 9个T_块完整 |
| `src/stream/ngx_stream.h` | 3 | ✅ |
| `src/stream/ngx_stream_core_module.c` | 3 | ✅ 11个T_块完整 |
| `src/stream/ngx_stream_proxy_module.c` | 1 | ✅ 8个T_块完整 |
| `src/stream/ngx_stream_ssl_module.c` | 9 | ✅ 28个T_块完整 |
| `src/stream/ngx_stream_ssl_module.h` | 1 | ✅ |

---

### 三、nginx-1.30 新增功能（已合入）

| 功能模块 | 新增文件 |
|----------|---------|
| **QUIC/HTTP3** | `src/event/quic/` (30个文件), `src/http/v3/` (12个文件) |
| **BPF 支持** | `src/core/ngx_bpf.c/h` |
| **OpenSSL 缓存** | `src/event/ngx_event_openssl_cache.c` |
| **Proxy v2** | `src/http/modules/ngx_http_proxy_module.h`, `ngx_http_proxy_v2_module.c` |
| **Upstream sticky** | `src/http/modules/ngx_http_upstream_sticky_module.c` |
| **Stream pass** | `src/stream/ngx_stream_pass_module.c` |
| **GitHub CI** | `.github/workflows/`, `.github/DISCUSSION_TEMPLATE/` 等 |

---

### 四、tengine 独有内容保留情况

#### 独有 src 文件（19个，全部保留 ✅）

| 文件 | 说明 |
|------|------|
| `src/core/ngx_segment_tree.c/h` | 线段树数据结构 |
| `src/core/ngx_trie.c/h` | Trie 树数据结构 |
| `src/event/ngx_dlopen.h` | 动态加载头文件 |
| `src/event/ngx_event_udpv2.c/h` | UDPv2 事件处理 |
| `src/os/unix/ngx_aio_read.c` | AIO 读取 |
| `src/os/unix/ngx_aio_read_chain.c` | AIO 链式读取 |
| `src/os/unix/ngx_aio_write.c` | AIO 写入 |
| `src/os/unix/ngx_aio_write_chain.c` | AIO 链式写入 |
| `src/os/unix/ngx_pipe.c/h` | 管道处理 |
| `src/os/unix/ngx_sysinfo.c/h` | 系统信息 |
| `src/proc/ngx_proc.c/h` | proc 子系统 |
| `src/event/modules/ngx_aio_module.c` | AIO 模块 |
| `src/event/modules/ngx_rtsig_module.c` | RTSIG 模块 |

#### 独有 modules 目录（32个模块，全部保留 ✅）

#### T_ 宏分布（合并后）

| 目录 | 涉及文件数 | T_ 宏块数 |
|------|-----------|----------|
| `src/core` | 23 | 89 |
| `src/event` | 11 | 52 |
| `src/http` | 38 | 371 |
| `src/http/modules` | 17 | 129 |
| `src/http/v2` | 3 | 8 |
| `src/os` | 8 | 38 |
| `src/stream` | 9 | 70 |
| `src/proc` | 1 | 1 |
| **总计** | **90** | **621** |

---

### 五、建议删除的文件（需用户确认）

以下文件是 **原始 tengine 中不存在，但本次合并从 nginx-1.30 引入**的文件。其中部分文件可能与 tengine 的项目结构冲突或不需要，建议由用户确认是否保留。

#### 5.1 可能需要删除的文件（tengine 原本主动删除的）

原始 tengine 中主动删除了这些 nginx-1.24.0 的文件，但本次合并从 nginx-1.30 恢复了其中部分（6个 win32 文件已按要求保留）。以下是 nginx-1.24.0 中 tengine 曾删除但在合并后又出现的文件：

| 文件 | 推荐操作 | 理由 |
|------|----------|------|
| `docs/html/index.html` | ⚠️ 建议确认 | tengine 有自己的 `html/` 目录，可能不需要 docs 里的 |
| `docs/xml/nginx/changes.xml` | ⚠️ 建议确认 | tengine 有自己的 CHANGES.te/CHANGES.cn |
| `misc/GNUmakefile` | ⚠️ 建议确认 | tengine 原本不包含此目录 |
| `misc/README` | ⚠️ 建议确认 | 同上 |

#### 5.2 nginx-1.30 新增的 GitHub CI/社区文件

| 文件 | 推荐操作 | 理由 |
|------|----------|------|
| `CODE_OF_CONDUCT.md` | ⚠️ 建议确认 | nginx 社区文件，tengine 可能有自己的 |
| `CONTRIBUTING.md` | ⚠️ 建议确认 | 同上 |
| `README.md` | ⚠️ 建议确认 | tengine 使用 `README.markdown`，可能冲突 |
| `SECURITY.md` | ⚠️ 建议确认 | nginx 安全策略，tengine 可能有自己的 |
| `SUPPORT.md` | ⚠️ 建议确认 | nginx 支持文档 |
| `.github/workflows/buildbot.yml` | ⚠️ 建议确认 | nginx 官方 CI，与 tengine CI 可能冲突 |
| `.github/workflows/check-pr.yml` | ⚠️ 建议确认 | 同上 |
| `.github/workflows/f5_cla.yml` | ⚠️ 建议删除 | F5 公司 CLA 检查，与 tengine 无关 |
| `.github/pull_request_template.md` | ⚠️ 建议确认 | nginx PR 模板 |
| `.github/DISCUSSION_TEMPLATE/*.yml` | ⚠️ 建议确认 | nginx 讨论模板 |
| `.github/ISSUE_TEMPLATE/*.yml` | ⚠️ 建议确认 | tengine 可能已有自己的 issue 模板 |

#### 5.3 win32 目录说明

原始 tengine 删除了整个 `src/os/win32/` 目录（54个文件），本次合并仅恢复了 6 个被 nginx-1.30 修改过的 win32 文件。**如需完整 win32 支持，建议从 nginx-1.30 补全剩余 48 个 win32 文件**：

```
需补全的 win32 文件（48个）:
src/os/win32/nginx.ico, nginx_icon*.xpm, nginx.rc
src/os/win32/ngx_alloc.c/h, ngx_atomic.h, ngx_dlopen.c/h
src/os/win32/ngx_errno.c, ngx_event_log.c, ngx_files.c
src/os/win32/ngx_os.h, ngx_process.c/h, ngx_process_cycle.h
src/os/win32/ngx_service.c, ngx_shmem.h, ngx_socket.c
src/os/win32/ngx_stat.c, ngx_thread.c/h, ngx_time.c/h
src/os/win32/ngx_udp_wsarecv.c, ngx_user.c/h
src/os/win32/ngx_win32_init.c, ngx_wsarecv.c
src/os/win32/ngx_wsarecv_chain.c, ngx_wsasend.c, ngx_wsasend_chain.c
```

---

### 六、后续建议

1. **编译验证**: 建议在目标环境执行 `./configure && make` 验证编译通过
2. **T_ 宏功能测试**: 逐个验证 T_ 宏保护的功能（NTLS、DTLS、XQUIC、动态 resolve 等）是否正常
3. **win32 补全**: 如需 win32 支持，从 nginx-1.30 复制剩余 48 个 win32 文件
4. **QUIC/HTTP3 适配**: nginx-1.30 新增的 QUIC/HTTP3 模块可能需要与 tengine 的 XQUIC 模块做功能整合
5. **用户确认**: 请根据第五节的建议，确认需要删除的文件
