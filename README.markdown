<h1 align="center" style="border-bottom: none">
    <br>Tengine
</h1>

<p align="center">Visit <a href="https://tengine.taobao.org" target="_blank">tengine.taobao.org</a> for the full documentation,
examples and guides.</p>

<div align="center">

[![GitHub license](https://img.shields.io/github/license/alibaba/tengine.svg)](https://github.com/alibaba/tengine/blob/main/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/alibaba/tengine.svg)](https://github.com/alibaba/tengine/stargazers)
[![GitHub stars](https://img.shields.io/badge/contributions-welcome-orange.svg)](https://github.com/alibaba/tengine/blob/main/CONTRIBUTING.md)
[![Build Status](https://github.com/alibaba/tengine/actions/workflows/ci.yml/badge.svg)](https://github.com/alibaba/tengine/actions/workflows/ci.yml)

</div>


## Introduction
Tengine is a web server originated by [Taobao](http://en.wikipedia.org/wiki/Taobao), the largest e-commerce website in Asia. It is based on the [Nginx](http://nginx.org) HTTP server and has many advanced features. Tengine has proven to be very stable and efficient on some of the top 100 websites in the world, including [taobao.com](http://www.taobao.com) and [tmall.com](http://www.tmall.com).

Tengine has been an open source project since December 2011. It is being actively developed by the Tengine team, whose core members are from Taobao, Sogou and other Internet companies. Tengine is a community effort and everyone is encouraged to [get involved](https://github.com/alibaba/tengine).

## Features
* All features of nginx-1.30.0 are inherited, i.e., it is 100% compatible with nginx.
* Dynamically configure the servers, locations and upstreams without reloading or restarting worker processes with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* HTTP/3 support (QUIC v1 and draft-29) with [xquic](https://github.com/alibaba/xquic).
* High-speed UDP transmission with kernel-bypass.
* Dynamically configure different TLS protocols for different server names with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure timeout setting, SSL Redirects, CORS and enabling/disabling robots for the server and location with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure HTTP routing based on multiple values of a specific header, cookie or query parameter with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure HTTP routing based on multiple upstream according to weight with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure HTTP routing based on modulo operation for a specific header, cookie or query parameter with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure HTTP routing to add/append custom header or add query parameter in the HTTP request to the upstream with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Dynamically configure HTTP routing to add custom header in the HTTP response to the client with [tengine-ingress](https://github.com/alibaba/tengine-ingress).
* Support the CONNECT HTTP method for forward proxy.
* Support asynchronous OpenSSL, using hardware such as QAT for HTTPS acceleration.
* Enhanced operations monitoring, such as asynchronous log & rollback, DNS caching, memory usage, etc.
* Support server_name in Stream module.
* More load balancing methods, e.g., consistent hashing, and session persistence.
* Input body filter support. It's quite handy to write Web Application Firewalls using this mechanism.
* Dynamic scripting language (Lua) support, which is very efficient and makes it easy to extend core functionalities.
* Limits retries for upstream servers (proxy, memcached, fastcgi, scgi, uwsgi).
* Includes a mechanism to support standalone processes.
* Protects the server in case system load or memory use goes too high.
* Multiple CSS or JavaScript requests can be combined into one request to reduce download time.
* Removes unnecessary white spaces and comments to reduce the size of a page.
* Proactive health checks of upstream servers can be performed.
* The number of worker processes and CPU affinities can be set automatically.
* The limit_req module is enhanced with whitelist support and more conditions are allowed in a single location.
* Enhanced diagnostic information makes it easier to troubleshoot errors.
* More user-friendly command lines, e.g., showing all compiled-in modules and supported directives.
* Expiration times can be specified for certain MIME types.
* Receives HTTP traffic on the TLS listener with option.
* Debugging HTTP connection usage.
* ...

## New Features Inherited from nginx-1.25.0 to nginx-1.30.0

The following upstream nginx features have been added compared to the previous nginx-1.24.0 base.

### HTTP/3 & QUIC
* Experimental HTTP/3 support (since 1.25.0), now production-ready.
* Path MTU discovery when using HTTP/3 (since 1.25.2).
* TLS_AES_128_CCM_SHA256 cipher suite support when using HTTP/3 (since 1.25.2).
* Support for 0-RTT in QUIC when using OpenSSL 3.5.1 or newer (since 1.29.1).
* CUBIC congestion control in QUIC connections (since 1.27.5).

### SSL/TLS Enhancements
* The `ssl_certificate_compression` directive for certificate compression (since 1.29.1).
* SSL certificates, secret keys, and CRLs are now cached on start or during reconfiguration (since 1.27.2).
* The `ssl_object_cache_inheritable`, `ssl_certificate_cache`, `proxy_ssl_certificate_cache`, `grpc_ssl_certificate_cache`, and `uwsgi_ssl_certificate_cache` directives (since 1.27.4).
* Client certificate validation with OCSP in the stream module (since 1.27.2).
* OCSP stapling support in the stream module (since 1.27.2).
* Loading of secret keys from hardware tokens with OpenSSL provider (since 1.29.0).
* Encrypted ClientHello (ECH) TLS extension support; the `ssl_ech_file` directive (since 1.29.4).
* OpenSSL 4.0 compatibility (since 1.29.8).
* Now nginx can be built with AWS-LC (since 1.29.2).
* Now certificate compression is available with BoringSSL (since 1.29.3).

### HTTP/2 & Proxy
* The ngx_http_proxy_module now supports HTTP/2 upstream connections (since 1.29.4).
* The `proxy_pass_trailers` directive in the ngx_http_proxy_module (since 1.27.2).
* The `add_header_inherit` and `add_trailer_inherit` directives (since 1.29.3).
* Support for response code 103 (Early Hints) from proxy and gRPC backends; the `early_hints` directive (since 1.29.0).
* Now the `http2` directive enables HTTP/2 on a per-server basis; the `http2` parameter of the `listen` directive is now deprecated (since 1.25.1).

### Upstream & Load Balancing
* Session affinity support; the `sticky` directive in the `upstream` block; the `server` directive supports `route` and `drain` parameters (since 1.29.6).
* The `server` directive in the `upstream` block supports the `resolve` parameter (since 1.27.3).
* The `resolver` and `resolver_timeout` directives in the `upstream` block (since 1.27.3).
* The `local` parameter of the `keepalive` directive in the `upstream` block (since 1.29.7).
* The `keepalive_min_timeout` directive (since 1.27.4).

### Stream Module
* Virtual servers in the stream module (since 1.25.5).
* The ngx_stream_pass_module (since 1.25.5).
* The `deferred`, `accept_filter`, and `setfib` parameters of the `listen` directive in the stream module (since 1.25.5).

### New Directives & Variables
* The `max_headers` directive to limit the number of request headers (since 1.29.8).
* The `multipath` parameter of the `listen` directive (since 1.29.7).
* The `$request_port` and `$is_request_port` variables (since 1.29.3).
* The `$ssl_sigalg` and `$ssl_client_sigalg` variables (since 1.29.3).
* The `volatile` parameter of the `geo` directive (since 1.29.3).
* Variables support in the `proxy_limit_rate`, `fastcgi_limit_rate`, `scgi_limit_rate`, and `uwsgi_limit_rate` directives (since 1.27.0).
* The `include` directive inside the `geo` block now supports wildcards (since 1.29.8).
* The `so_keepalive` parameter of the `listen` directive on macOS (since 1.29.0).

### Performance & Build
* Startup speedup when using a large number of locations (since 1.25.3).
* Cache line size detection for some architectures (since 1.25.5).
* Support for Homebrew on Apple Silicon (since 1.25.5).

## Installation
Tengine can be downloaded at [http://tengine.taobao.org/download/tengine.tar.gz](http://tengine.taobao.org/download/tengine.tar.gz). You can also checkout the latest source code from GitHub at [https://github.com/alibaba/tengine](https://github.com/alibaba/tengine)

To install Tengine, just follow these three steps:
```bash
./configure
make
sudo make install
```

By default, it will be installed to _/usr/local/nginx_. You can use the __'--prefix'__ option to specify the root directory.
If you want to know all the _'configure'_ options, you should run __'./configure --help'__ for help.

## Documentation
The homepage of Tengine is at [http://tengine.taobao.org/](http://tengine.taobao.org/)
You can access [http://tengine.taobao.org/documentation.html](http://tengine.taobao.org/documentation.html) for more information.

## Contact
[https://github.com/alibaba/tengine/issues](https://github.com/alibaba/tengine/issues)

Dingtalk user group: 23394285

## License

[BSD-2-Clause License](https://github.com/alibaba/tengine/blob/master/LICENSE)

<h1 align="center" style="border-bottom: none">
    <a href="https://tengine.taobao.org" target="_blank"><img alt="Tengine" src="/docs/image/tengine-logo.png"></a>
</h1>
