#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern char **environ;

pthread_mutex_t env_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_key_t key;
pthread_once_t once = PTHREAD_ONCE_INIT;

void key_create(void)
{
    pthread_key_create(&key, free);
}

char * mygetenv(char *name)
{
    int i;
    char *ptr;
    char *buf = NULL;
    
    pthread_once(&once, key_create);

    buf = pthread_getspecific(key);
    if (!buf)
    {
        buf = (char *)malloc(512);
        if (!buf)
        {
            perror("malloc");
            return NULL;
        }
        pthread_setspecific(key, buf);
    }
    pthread_mutex_lock(&env_lock);
    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(name, environ[i], strlen(name)) == 0 &&
                environ[i][strlen(name)] == '=')
        {
            ptr = environ[i] + strlen(name) + 1;
            strcpy(buf, ptr);
            pthread_mutex_unlock(&env_lock);
            return buf;
        }
    
    }

    pthread_mutex_unlock(&env_lock);
    return NULL;
}

int main(void)
{
    int i = 0;
    char *ptr = mygetenv("PATH");

    printf("%s\n", ptr);
    free(ptr);
    exit(0);
}
