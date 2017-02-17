#ifndef _LOG_MSG_PROCESS_H_
#define _LOG_MSG_PROCESS_H_

#include "common_def.h"
#include "base_msg_process.h"


class log_msg_process:public base_msg_process
{
    public:
        log_msg_process(void *p):base_msg_process(p)
    {
    }

        virtual ~log_msg_process()
        {
        }	

        virtual size_t process_recv_buf(char *buf, size_t len)
        {
            //PDEBUG("recv buf %d\n", len);
            //size_t ret = 0;
            size_t left_len = len;
            while(left_len > 0)
            {
                RECV_MSG_STATUS status = RECV_MSG_HEAD;
                _head_len = sizeof(int);
                size_t msg_body_len = 0;
                if (status == RECV_MSG_HEAD)
                {
                    if (left_len > _head_len)
                    {

                        int *p_len = (int *)buf;
                        msg_body_len = *p_len;

                        PDEBUG("left_len[%d] _head_len[%d] msg_body_len[%d]\n", left_len, _head_len, msg_body_len);
                        status = RECV_MSG_BODY;
                    }
                    else
                    {
                        break;
                    }
                }

                if (status == RECV_MSG_BODY)
                {
                    if (left_len >= _head_len + msg_body_len) {
                        process_s(buf, _head_len + msg_body_len);

                        left_len -= (_head_len + msg_body_len);
                        buf = buf + _head_len + msg_body_len;
                    } else {
                        break;
                    } 
                }				
            }
            //PDEBUG("len[%d]\n", len - left_len);

            return len - left_len;
        }	


        size_t process_s(char *buf, size_t len)
        {
            put_msg(buf, len);
            return len;
        }

        void put_msg(char *buf, size_t len)
        {
            if (!buf || len < _head_len){
                return;
            }


            LogMsg log_msg;
            log_msg.ParseFromArray(buf + _head_len, len - _head_len);

            if (!log_msg.length())
            {
                return;
            }

            int flag = 0;

            if (log_msg.length() && log_msg[log_msg.length() - 1] == '\n')
            {
                flag = 1
            }


            if (log_msg.deal_mode() & 1) {
                check_to_renmae(log_msg.file_name(), log_msg.max_size());
                FILE * fp = fopen(log_msg.file_name(), "a+");
                ASSERT_DO(fp != NULL, printf("file_name: %s\n", log_msg.file_name));

                if (flag)
                    fprintf(fp, "%s", log_msg.str().c_str());
                else 
                    fprintf(fp, "%s\n", log_msg.str().c_str());
                fclose(fp);
            }



            if (log_msg.deal_mode() & 1<<1){
                if (flag)
                    printf("%s", log_msg.str().c_str());
                else 
                    printf("%s\n", log_msg.str().c_str());
            }
        }

        void check_to_renmae(const char *filename, uint32_t max_size)
        {
            if (!filename){
                return;
            }

            char tmp[SIZE_LEN_64];
            char path[SIZE_LEN_128];
            struct stat statBuf;
            stat(filename, &statBuf);

            get_date_str(tmp, sizeof(tmp), "%Y%m%d%H%M%S");
            if (max_size && statBuf.st_size >= _max_size){
                snprintf(path, sizeof(path), "%s.%s", filename, tmp);
                rename(filename, path);
            }
        }

};

#endif

