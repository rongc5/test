#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern char **environ;
static pthread_mutex_t env_lock;
static int once = 0;
pthread_once_t s_once = PTHREAD_ONCE_INIT;

void my_once(int *once, void (*init_routine)(void))
{
    if (*once == 0)
    {
        *once = 1;
        init_routine();
    }
    else 
        return;
}

static void mutex_init(void)
{
    pthread_mutex_init(&env_lock, NULL);
}

int getenv_r(const char *name, char *buf, size_t buflen)
{
    int i;
    char *ptr;
 //   my_once(&once, mutex_init);
    pthread_once(&s_once, mutex_init);
    pthread_mutex_lock(&env_lock);
    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(name, environ[i], strlen(name)) == 0 &&
                environ[i][strlen(name)] == '=')
        {
            ptr = environ[i] + strlen(name) + 1;
            strncpy(buf, ptr, buflen);
            pthread_mutex_unlock(&env_lock);
            return 0;
        }
    
    }

    pthread_mutex_unlock(&env_lock);
    return -1;
}

int main(void)
{
    int i = 0;
    char buf[1000];
    getenv_r("PATH", buf, 1000);
    printf("%s\n", buf);
    exit(0);
}
