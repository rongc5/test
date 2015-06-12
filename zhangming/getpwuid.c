#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    char s[512];
    char *buf = s;
    uid_t uid = (uid_t)atoi(v[1]);
    struct passwd * res;
    int ret;

    res = getpwuid(uid);
    if (NULL == res)
    {
        perror("getpwuid");
        exit(1);
    }

    ret = sprintf(buf, "%s", res->pw_name);
    buf += ret;
    ret = sprintf(buf, "%s", res->pw_passwd);
    buf += ret;
    ret = sprintf(buf, "%u", res->pw_uid);
    buf += ret;
    ret = sprintf(buf, "%u", res->pw_gid);
    buf += ret;
    ret = sprintf(buf, "%s", res->pw_gecos);
    buf += ret;
    ret = sprintf(buf, "%s", res->pw_dir);
    buf += ret;
    ret = sprintf(buf, "%s", res->pw_shell);
    
    printf("%s\n", s);

    return 0;

}
