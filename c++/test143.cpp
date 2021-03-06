#include <string.h>
#include <string>
#include <stdio.h>

using namespace std;

struct obj_id_str {
    obj_id_str()
    {
        _obj_id = 0;
        _thread_id = 0;
    }

    int _obj_id; //消息id
    int _thread_id;//线程id
};


struct _pass_msg_t {
    int _op;
    obj_id_str id;
    int body_len;
};


struct test_t {
    int len;
    char buf[1024];
};


int main(int argc, char *argv[])
{
    
    string str;

    _pass_msg_t head;
    head.body_len = 99;
    char buf[512] = {0};
    memcpy(buf, &head._op, sizeof(head._op));
    memcpy(buf+sizeof(head.id), &head.id, sizeof(head.id));
    memcpy(buf+sizeof(head._op) + sizeof(head.id), &head.body_len, sizeof(head.body_len));
    _pass_msg_t * phead =  (_pass_msg_t *)buf;
    printf("%d\t%d\t%d\n", phead->body_len, strlen(buf), sizeof(head));
    memcpy((void*)str.c_str(), &head, sizeof(head));
    //memcpy((void*)str.c_str(), buf, strlen(buf) + 1);
    
    phead = (_pass_msg_t *) str.c_str();
    printf("%d\t%d\n", phead->body_len, str.size());

    {
        string str;
        str.append("hello world");
        test_t t;
        t.len = str.size();
        memcpy(t.buf, str.c_str(), str.size());

        printf("%d\t%s\t%d\n", t.len, t.buf, strlen(t.buf));


        memset(buf, 0, sizeof(buf));

        memcpy(buf, &t, sizeof(t));

        test_t * ptr = (test_t *) buf;

        printf("%d\t%s\t%d\n", ptr->len, ptr->buf, strlen(ptr->buf));



        
    }

    return 0;
}
