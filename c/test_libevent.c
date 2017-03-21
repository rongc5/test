#include <event2/event_struct.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <evhttp.h>

#include <string.h>

void http_request_done(struct evhttp_request *req, void *arg)
{
    printf("send request ok...\n");
    size_t len = evbuffer_get_length(req->input_buffer);
    unsigned char * str = evbuffer_pullup(req->input_buffer, len);
    /*printf("%s\n", str);*/
    char buf[512];
    snprintf(buf, sizeof(buf), "%s", str);
    if(str == NULL)
    {
        printf("len = %d, str == NULL\n", len);
    }
    else
    {
        printf("len = %d, str = %s\n", len, buf);
    }

    event_base_loopbreak((struct event_base*)arg);
}

int main()
{
    struct event_base* base;
    struct evhttp_connection* conn;
    struct evhttp_request* req;

    base = event_base_new();
    //conn = evhttp_connection_new(base, NULL, "192.168.1.109", 8081);
    for (int i = 1; i <= 3; i++) {
    conn = evhttp_connection_new("127.0.0.1", 8088);
    evhttp_connection_set_base(conn, base);

    req = evhttp_request_new(http_request_done, base);
    evhttp_add_header(req->output_headers, "Host", "localhost");
    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/test");
    evhttp_connection_set_timeout(req->evcon, 100);
    event_base_dispatch(base);

    evhttp_connection_free(conn);
    event_base_free(base);
    printf("run over...\n");

    return 0;
}

