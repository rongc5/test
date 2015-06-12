#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct stu{
    int a:3;
    int b:2;
    int c:1;
}STU;

int main(void)
{
    STU st;

    st.a = 6;
    st.b = 2;
    st.c = 1;

    printf("%d  %d  %d\n", st.a, st.b, st.c);

    return 0;
}
