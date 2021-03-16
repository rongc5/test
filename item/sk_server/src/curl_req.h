#ifndef __CURL_REQ_H__
#define __CURL_REQ_H__

#include "common_util.h"
#include <curl/curl.h>

class curl_req
{
    public:
        curl_req();

        int init();

        void destroy();

        int init_url(std::string url, std::map<std::string, std::string> & headers, int time_out=1000);

        int get_data(std::string & res);

        static size_t write_callback(void *ptr, size_t size, size_t nmemb, char *buf);

        struct CurlResBuffer {
            uint32_t    len;
            char        buf[SIZE_LEN_1024 * 100];
        };

    private:
        CURL*   curl_handle_;
        char    curl_err_msg_[CURL_ERROR_SIZE];
        CurlResBuffer  res_data_buffer_;
        struct curl_slist * _header;
};





#endif
