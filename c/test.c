#include <stdio.h>
#include <string.h>
#define is_ptr(x) __builtin_types_compatible_p(typeof(x), typeof(&(x)[0]))
#define get_u16(X,O)  (*(unsigned short *)((unsigned long)X + O))
#define app_memeql(b, m)   \
        (sizeof(char [1 - 2 * is_ptr(m)]) * 0 + !memcmp((b), (m), sizeof(m) - 1))

int main()
{
    char *m="abcdxyzuvwefghijklmn";
    int ret;   
    ret = app_memeql(m,"bcd");
   
    printf("%d\n",  __builtin_types_compatible_p(typeof("bcd"), typeof(&"bcd"[0])));
    printf("%d\n", !memcmp((m), ("bcd"), sizeof("bcd") - 1));
    printf("%d\n",is_ptr(m));
    printf("%d\n",ret);
    return 0;
}
