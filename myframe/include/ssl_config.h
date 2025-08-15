#ifndef __SSL_CONFIG_H__
#define __SSL_CONFIG_H__

#include "base_def.h"

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

struct ssl_config
{
    ssl_config()
    {
        _enable = false;
        _cert_file.clear();
        _key_file.clear();
        _ca_file.clear();
        _protocols = "TLSv1.2:TLSv1.3";
        _cipher_list = "HIGH:!aNULL:!MD5";
        _verify_peer = false;
        _verify_depth = 9;
    }

    void init()
    {
        _enable = false;
        _cert_file.clear();
        _key_file.clear();
        _ca_file.clear();
        _protocols = "TLSv1.2:TLSv1.3";
        _cipher_list = "HIGH:!aNULL:!MD5";
        _verify_peer = false;
        _verify_depth = 9;
    }

    bool _enable;
    std::string _cert_file;
    std::string _key_file;
    std::string _ca_file;
    std::string _protocols;
    std::string _cipher_list;
    bool _verify_peer;
    int _verify_depth;
};

#endif // __SSL_CONFIG_H__