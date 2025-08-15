#ifndef __SSL_CONTEXT_H__
#define __SSL_CONTEXT_H__

#include "base_def.h"
#include "ssl_config.h"
#include "base_singleton.h"

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#endif

class ssl_context
{
public:
    ssl_context();
    virtual ~ssl_context();

    static bool init_global();
    static void cleanup_global();

    bool init_server(const ssl_config& config);
    bool init_client(const ssl_config& config);

#ifdef ENABLE_SSL
    SSL_CTX* get_ssl_ctx() { return _ssl_ctx; }
    SSL* create_ssl();
#endif

    bool is_initialized() const { return _initialized; }
    bool is_server() const { return _is_server; }

private:
    bool load_certificates(const ssl_config& config);
    bool set_cipher_list(const std::string& cipher_list);
    bool set_protocols(const std::string& protocols);

#ifdef ENABLE_SSL
    SSL_CTX* _ssl_ctx;
#endif
    bool _initialized;
    bool _is_server;
    ssl_config _config;

    static bool _global_initialized;
};

typedef base_singleton<ssl_context> ssl_context_singleton;

#endif // __SSL_CONTEXT_H__