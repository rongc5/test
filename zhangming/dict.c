#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define     WORDLEN 64
#define     TRANSLEN 1024
typedef struct data
{
    char name[WORDLEN];
    char trans[TRANSLEN];
}DATA;

void error(const char* msg)
{
    perror(msg);
    exit(-1);
}

char* myfgets(char* s, int size, FILE* fp)
{
    int num=0;
    char c;
    int flag=0;
    do
    {
        int ret=fread(&c, sizeof(char), 1, fp);
        if(1==ret)
        {
            s[num++]=c;

            if(num==size-1)
            {
                break;
            }
        }
        else
        {
            flag=1;
            break;
        }

    }while(c!='\n');

    s[num]='\0';

    if(1==flag)
    {
        return NULL;
    }
    
    return s;
}

int readword(DATA* pd, FILE* fp)
{
    char* ret;
    ret=myfgets(pd->name, WORDLEN, fp);
    if(NULL==ret)
    {
        return -1;
    }
    myfgets(pd->trans, TRANSLEN, fp);

    int len=strlen(pd->name);
    if('\n'==pd->name[len-1])
    {
        pd->name[len-1]='\0';
    }
    
    len=strlen(pd->trans);
    if('\n'==pd->trans[len-1])
    {
        pd->trans[len-1]='\0';
    }

    return 0;
}

DATA* searchword(const char* str, FILE* fp)
{
    static DATA data;
    fseek(fp, 0, SEEK_SET);
    int ret;

    do
    {
        ret=readword(&data, fp);

        if(ret!=-1)
        {
            if(0==strcmp(data.name+1, str))
            {
                return &data;
            }
        }
    }while(ret!=-1);

    return NULL;
}

void display(DATA* pd)
{
    char* del=":@";
    char* p=strtok(pd->trans, del);

    int i=1;

    while(p!=NULL)
    {
        p=strtok(NULL, del);

        if(NULL!=p)
        {
            printf("trans %d:%s\n", i++, p);
        }
    }

}

int main(int argc, char *argv[])
{
    FILE* fp=fopen("dict.txt", "r");
    if(NULL==fp)
    {
        error("in main, fopen error");
    }
    char str[WORDLEN];
    DATA* pd;
    scanf("%s", str);

    while(strcmp(str, "exit")!=0)
    {
        pd=searchword(str, fp);
        if(pd!=NULL)
        {
            display(pd);
        }
        else
        {
            printf("not found...\n");
        }
        scanf("%s", str);
    }
    fclose(fp);

    return 0;
}

