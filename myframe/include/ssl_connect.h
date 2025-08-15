#ifndef __SSL_CONNECT_H__
#define __SSL_CONNECT_H__

#include "base_connect.h"
#include "ssl_context.h"

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

enum SSL_HANDSHAKE_STATUS
{
    SSL_HANDSHAKE_NONE = 0,
    SSL_HANDSHAKE_WANT_READ = 1,
    SSL_HANDSHAKE_WANT_WRITE = 2,
    SSL_HANDSHAKE_DONE = 3,
    SSL_HANDSHAKE_ERROR = 4
};

template<class PROCESS>
class ssl_connect : public base_connect<PROCESS>
{
public:
    ssl_connect(int fd, ssl_context* ssl_ctx, bool is_server = true);
    virtual ~ssl_connect();

    virtual int real_recv(char* buf, int len);
    virtual int real_send(const char* buf, int len);

    SSL_HANDSHAKE_STATUS ssl_handshake();
    bool is_handshake_done() const { return _handshake_status == SSL_HANDSHAKE_DONE; }
    SSL_HANDSHAKE_STATUS get_handshake_status() const { return _handshake_status; }

    virtual void close();

private:
    void handle_ssl_error(int ret);
    std::string get_ssl_error_string();

#ifdef ENABLE_SSL
    SSL* _ssl;
#endif
    ssl_context* _ssl_context;
    SSL_HANDSHAKE_STATUS _handshake_status;
    bool _is_server;
};

// 模板实现
template<class PROCESS>
ssl_connect<PROCESS>::ssl_connect(int fd, ssl_context* ssl_ctx, bool is_server) : base_connect<PROCESS>(fd)
{
#ifdef ENABLE_SSL
    _ssl = NULL;
#endif
    _ssl_context = ssl_ctx;
    _handshake_status = SSL_HANDSHAKE_NONE;
    _is_server = is_server;

#ifdef ENABLE_SSL
    if (_ssl_context && _ssl_context->is_initialized()) {
        _ssl = _ssl_context->create_ssl();
        if (_ssl != NULL) {
            SSL_set_fd(_ssl, fd);
            if (_is_server) {
                SSL_set_accept_state(_ssl);
            } else {
                SSL_set_connect_state(_ssl);
            }
        }
    }
#endif
}

template<class PROCESS>
ssl_connect<PROCESS>::~ssl_connect()
{
#ifdef ENABLE_SSL
    if (_ssl != NULL) {
        SSL_shutdown(_ssl);
        SSL_free(_ssl);
        _ssl = NULL;
    }
#endif
}

template<class PROCESS>
SSL_HANDSHAKE_STATUS ssl_connect<PROCESS>::ssl_handshake()
{
#ifdef ENABLE_SSL
    if (_ssl == NULL) {
        _handshake_status = SSL_HANDSHAKE_ERROR;
        return _handshake_status;
    }

    if (_handshake_status == SSL_HANDSHAKE_DONE) {
        return _handshake_status;
    }

    int ret;
    if (_is_server) {
        ret = SSL_accept(_ssl);
    } else {
        ret = SSL_connect(_ssl);
    }

    if (ret == 1) {
        _handshake_status = SSL_HANDSHAKE_DONE;
        return _handshake_status;
    }

    int ssl_error = SSL_get_error(_ssl, ret);
    switch (ssl_error) {
        case SSL_ERROR_WANT_READ:
            _handshake_status = SSL_HANDSHAKE_WANT_READ;
            break;
        case SSL_ERROR_WANT_WRITE:
            _handshake_status = SSL_HANDSHAKE_WANT_WRITE;
            break;
        default:
            _handshake_status = SSL_HANDSHAKE_ERROR;
            break;
    }

    return _handshake_status;
#else
    _handshake_status = SSL_HANDSHAKE_ERROR;
    return _handshake_status;
#endif
}

template<class PROCESS>
int ssl_connect<PROCESS>::real_recv(char* buf, int len)
{
#ifdef ENABLE_SSL
    if (_ssl == NULL || _handshake_status != SSL_HANDSHAKE_DONE) {
        errno = EAGAIN;
        return -1;
    }

    int ret = SSL_read(_ssl, buf, len);
    if (ret > 0) {
        return ret;
    }

    int ssl_error = SSL_get_error(_ssl, ret);
    switch (ssl_error) {
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
            errno = EAGAIN;
            return -1;
        case SSL_ERROR_ZERO_RETURN:
            return 0;
        default:
            errno = EIO;
            return -1;
    }
#else
    return base_connect<PROCESS>::real_recv(buf, len);
#endif
}

template<class PROCESS>
int ssl_connect<PROCESS>::real_send(const char* buf, int len)
{
#ifdef ENABLE_SSL
    if (_ssl == NULL || _handshake_status != SSL_HANDSHAKE_DONE) {
        errno = EAGAIN;
        return -1;
    }

    int ret = SSL_write(_ssl, buf, len);
    if (ret > 0) {
        return ret;
    }

    int ssl_error = SSL_get_error(_ssl, ret);
    switch (ssl_error) {
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
            errno = EAGAIN;
            return -1;
        default:
            errno = EIO;
            return -1;
    }
#else
    return base_connect<PROCESS>::real_send(buf, len);
#endif
}

template<class PROCESS>
void ssl_connect<PROCESS>::close()
{
#ifdef ENABLE_SSL
    if (_ssl != NULL) {
        SSL_shutdown(_ssl);
    }
#endif
    base_connect<PROCESS>::close();
}

template<class PROCESS>
void ssl_connect<PROCESS>::handle_ssl_error(int ret)
{
#ifdef ENABLE_SSL
    // SSL错误处理实现
#endif
}

template<class PROCESS>
std::string ssl_connect<PROCESS>::get_ssl_error_string()
{
#ifdef ENABLE_SSL
    unsigned long err = ERR_get_error();
    if (err == 0) {
        return "No SSL error";
    }

    char err_buf[256];
    ERR_error_string_n(err, err_buf, sizeof(err_buf));
    return std::string(err_buf);
#else
    return "SSL not enabled";
#endif
}

#endif // __SSL_CONNECT_H__