#include "addr.h"
#include <execinfo.h>
#include <libunwind.h>
#include <dlfcn.h>
#include <link.h>

#define THREADNUM   10


void sigaction_handler(int s, siginfo_t *info, void* cont)
{
    int ret;
    void *buf[100];

    ret = backtrace(buf, 100);

    backtrace_symbols_fd(buf, ret, 1);

    printf("the err addr:%#x\n", (size_t)info->si_addr);

    void *p = __builtin_return_address(0); 
    printf("%p\n", p);
    p = __builtin_return_address(1); 
    printf("%p\n", p);

    exit(0);
}

void sigaction_handler1(int s, siginfo_t *info, void* cont)
{
    char name[256];
    unw_cursor_t cursor; 
    unw_context_t uc;
    unw_word_t ip, sp, offp;
    //Dl_info info;

    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    while (unw_step(&cursor) > 0) {
        int line = 0;
        name[0] = '\0';
        unw_get_proc_name(&cursor, name, 256, &offp);
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        printf ("name = %s, ip = %#x, sp = %#x\n", name, (long) ip, (long) sp);
     //   if (dladdr((void *)ip, &info)){
       //     printf("%s %#x\n", info.dli_fname, info.dli_fbase);
       // }
    }


    int ret;
    void *buf[100];

    ret = backtrace(buf, 100);

    backtrace_symbols_fd(buf, ret, 1);


    exit(0);

}

static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
    int j;

    printf("name=%s (%d segments)\n", info->dlpi_name,
            info->dlpi_phnum);

    for (j = 0; j < info->dlpi_phnum; j++)
        printf("\t\t header %2d: address=%10p\n", j,
                (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
    return 0;
}





int main(int argc, char *argv[])
{
    int ret, id, i = 0;
    char buf[512];
    thread_pool_t * pools;
    struct sigaction act;

    dl_iterate_phdr(callback, NULL);
    pools = create_pools(THREADNUM);
    if (!pools) {
        perror("create_pools!!\n");
        return 1;
    }

    ret = creat_threads(THREADNUM, pools);
    if (ret) {
        perror("creat_threads!!\n");
        return 1;
    }

    wait_all_threads_ready(pools, THREADNUM);

    //act.sa_sigaction = sigaction_handler1;
    act.sa_sigaction = sigaction_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO ;

    sigaction(SIGSEGV, &act, NULL);


    while(1){ 
        printf("please input your id: ");
        fflush(stdout); 

        ret = read(0, buf, sizeof(buf));
        buf[ret - 1] = '\0';
        id = atoi(buf);

        task_allocation(pools, i%THREADNUM, id);
        i++;
   }

    while (1)
        pause();

    return 0;

}


