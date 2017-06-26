#include "common_def.h"
#include "ws_cli_thread.h"
#include "log_helper.h"
#include "base_thread.h"
#include "base_singleton.h"
#include "user_info.h"


void do_prepare()
{
    user_info *user = new user_info;

    int op = 0;
    printf("角色选择1、咨询师, 2、用户\n:");
    scanf("%d", &user->op);

    printf("请输入公司id\n:");
    scanf("%d", &user->groupid);
    printf("请输入用户id\n:");
    scanf("%d", &user->userid);

    if (op == 1){
        printf("请输入密码id\n:");
        scanf("%s", user->passwd);

        printf("登录中请等待\n");
    }

    base_singleton<user_info>::set_instance(user);
}



int main(int c, char **v)
{
    log_conf conf;
    conf.deal_mode = 1;
    //strcpy(conf.prefix_file_name, "asp");
    LOG_INIT(conf); 

    do_prepare();
    

    LOG_DEBUG("c_thread");
    ws_cli_thread * c_thread = new ws_cli_thread();


    c_thread->init("127.0.0.1", 8088);
    c_thread->start();

    base_thread::join_all_thread();

    return 0;
}

