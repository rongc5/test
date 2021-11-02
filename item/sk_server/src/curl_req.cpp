#include "curl_req.h"
#include "log_helper.h"


curl_req::curl_req()
{
}

int curl_req::init()
{
    //curl_handle_ = curl_easy_init();
    if (!curl_handle_) {
        curl_handle_ = curl_easy_init();
    }

    if (!curl_handle_) {
        LOG_WARNING("curl_easy_init Failed!");
        return -1;
    }

    CURLcode code = CURL_LAST;

    code = curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1L);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_NOSIGNAL Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -2;
    }

    code = curl_easy_setopt(curl_handle_, CURLOPT_ERRORBUFFER, curl_err_msg_);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_ERRORBUFFER Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -3;
    }

    curl_easy_setopt(curl_handle_, CURLOPT_VERBOSE, 0L);

    //    code = curl_easy_setopt(curl_handle_, CURLOPT_PROXY, CURLPROXY_HTTP);
    //    ASSERT_FATAL(CURLE_OK == code, "set CURLOPT_PROXY Failed! code [%d], error [%s]", code, curl_err_msg_);
    code = curl_easy_setopt(curl_handle_, CURLOPT_HTTPGET, 0L);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_HTTPGET Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -4;
    }

    //    code = curl_easy_setopt(curl_handle_, CURLOPT_PORT, (long)online_conf_->port_);
    //    ASSERT_FATAL(CURLE_OK == code, "set CURLOPT_PORT Failed! code [%d], error [%s]", code, curl_err_msg_);
    code = curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, 1000);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_TIMEOUT Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -5;
    }

    code = curl_easy_setopt(curl_handle_, CURLOPT_CONNECTTIMEOUT_MS, 800);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_CONNECTTIMEOUT Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -6;
    }

    code = curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write_callback);
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_WRITEFUNCTION Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -7;
    }

    code = curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (char*)&(this->res_data_buffer_));
    if (CURLE_OK != code) {
        LOG_WARNING("set CURLOPT_WRITEDATA Failed! code [%d], error [%s]", code, curl_err_msg_);
        return -8;
    }


    return 0;
}

void curl_req::destroy()
{
    if (_header) {
        curl_slist_free_all(_header);
        _header = NULL;
    }

    if (curl_handle_) {
        curl_easy_cleanup(curl_handle_);
        curl_handle_ = NULL;
    }

    return ;
}

void curl_req::reset()
{
    if (_header) {
        curl_slist_free_all(_header);
        _header = NULL;
    }   

    if (curl_handle_) {
        curl_easy_reset(curl_handle_);
    }   
}

int curl_req::init_url(std::string url, std::map<std::string, std::string> & headers, int time_out, std::string cafile)
{
    int count = 0;
    while (!curl_handle_ && count < 3)
    {
        destroy();
        //reset();
        if (init() < 0) { 
            count++;
         }
    }

    if (!curl_handle_)
    {
        LOG_WARNING("init Failed after %d times", count);
        return -1;
    }

    CURLcode code = CURL_LAST;

    _header=NULL;
    char t_buf[SIZE_LEN_2048];


    for ( auto ii: headers)
    {
        snprintf(t_buf, sizeof(t_buf), "%s: %s", ii.first.c_str(), ii.second.c_str());
        _header = curl_slist_append(_header, t_buf);
    }

    curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, _header);

    if (time_out)
    {
        code = curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, time_out);
        if (CURLE_OK != code)
            LOG_WARNING("set CURLOPT_TIMEOUT Failed! code [%d], error [%s]", code, curl_err_msg_);
    }

    //客户端忽略CA证书认证 用于https跳过证书认证
    if (start_with(url.c_str(), "https"))
    {
        if (cafile.empty()) 
        {
            curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYPEER, false);
        }
        else 
        {
            curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl_handle_, CURLOPT_CAINFO,  cafile.c_str());
        }
    }

    code = curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    if (code != CURLE_OK) {
        LOG_WARNING("set CURLOPT_URL Failed! code [%d], error [%s]", code, curl_err_msg_);
    }


    return 0;
}

int curl_req::get_data(std::string & res)
{
    if (!curl_handle_)
    {
        LOG_WARNING("curl_handle_ is NULL");
        return -1;
    }

    CURLcode code = CURL_LAST;

    this->res_data_buffer_.len = 0;
    memset(this->res_data_buffer_.buf, 0, sizeof(this->res_data_buffer_.buf));

    code = curl_easy_perform(curl_handle_);
    //if (_header)
    //{
    //curl_slist_free_all(_header);   
    //_header = NULL;
    //}
    if (code != CURLE_OK) { 
        LOG_WARNING("curl_easy_perform() Failed! code [%d], error [%s]", code, curl_err_msg_);
    }
    long ret_code = 0;
    code = curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE , &ret_code);
    if (code != CURLE_OK || ret_code != 200) { 
        LOG_WARNING("curl_easy_getinfo() Failed! code [%d], error [%s], ret code [%ld]", code, curl_err_msg_, ret_code);
    }
    destroy();

    res.append(res_data_buffer_.buf, res_data_buffer_.len);
    reset();

    return 0;
}


size_t curl_req::write_callback(void *ptr, size_t size, size_t nmemb, char *buf)
{
    size_t sz = size * nmemb;
    CurlResBuffer* res_buf = (CurlResBuffer*)buf;
    LOG_DEBUG("sz [%u], buffer cur len [%u]", sz, res_buf->len);
    if (res_buf->len + sz > sizeof(res_buf->buf)) {
        LOG_WARNING("Overflowed: data size [%u] add cur buf len [%u] gt ONLINE_AD_LEN [%u]",
                sz, res_buf->len, sizeof(res_buf->buf));
    } else {
        memcpy(res_buf->buf + res_buf->len, (char*)ptr, sz);
        res_buf->len += sz;
    }

    return sz;
}

