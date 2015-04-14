#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

static int  province_level_len;
static int city_level_len;
static int country_level_len;


static char province_level_buf[64] = "";
static char city_level_buf[64] = "";

#define PROVINCE 999 
#define CITY    888
#define COUNTRY 777


typedef struct{
    int  flaglevel;
    char cityid[8];
    char region[64];
    char allname[128];
    struct list_head entry;
}CIDINFO;

static CIDINFO g_list;

static FILE *fp = NULL;

void init_dlist(list_head *head)
{
    CIDINFO *ptr = list_entry(head, CIDINFO, entry); 
    memset(ptr, 0, sizeof(*ptr));
    head->next = head;
    head->prev = head;
}

FILE * getfile(void)
{
    fp = fopen("./data/CityId.txt", "r+");
    if (fp == NULL) {
        printf("%s\n", strerror(errno));
        return NULL;
    }
    return fp;
}

int recover_file_pointer(FILE *fp)
{
    fseek(fp, 0, SEEK_CUR);

    return 0;
}


CIDINFO * list_search_cityid(char buf[])
{
    list_head *pos = NULL;
    CIDINFO * tmp = NULL;
    CIDINFO *lastrecord = NULL;
    int lastrecordlevel = 0;
    LIST_FOR_EACH(pos, &g_list){
        tmp = list_entry(pos, CIDINFO, entry);
         if (strstr(buf, tmp->region)){
            lastrecord = tmp;
            lastrecordlevel = tmp->flaglevel;
            if (tmp->flaglevel == COUNTRY)
                return tmp;
            continue;
         }

         if (lastrecord != 0 && tmp->flaglevel >= lastrecordlevel)
             return lastrecord;
    }

    return NULL;
}

static int getstringformat(void)
{
    char buf[256];
    char *retptr;
    int i = 0, j, k;
    
    recover_file_pointer(fp);

    for (i; i < 3; i++){
        j = 0, k = 0;
        retptr = fgets(buf, 256, fp);
        if (NULL == retptr) {
            perror("fgets...");
            exit(1);
        }  
        
        while (buf[i++] != ' ')
            ;

        while (buf[i++] == ' ')
            k++;
        
        if (i == 0)
            province_level_len = k;
        else if (i == 1)
            city_level_len = k;
        else if (i == 2)
            country_level_len = k;
    }

}

int truncate_and_save_string(const char *start, char *stop, char *save)
{
    if (start == NULL || stop == NULL || save == NULL)
        return 1;

    while (*start != *stop)
        *save++ = *start++;

    *save = '\0';

    return 0;
}

static int copy_full_name(CIDINFO *ptr, char src[], int tmp)
{
    char *ptmp;
    if (tmp == province_level_len){
        sprintf(province_level_buf, "%s", src);
        sprintf(ptr->allname, "%s", src);
        if (ptmp = strstr(src, "省") || ptmp = strstr(src, "市"))
            truncate_and_save_string(src, ptmp, ptr->region);
        else
            sprintf(ptr->region, "%s", src);
        ptr->flaglevel = PROVINCE;
    }
    else if (tmp == city_level_len){
        sprintf(city_level_buf, "%s", src);
        sprintf(ptr->allname, "%s%s", province_level_buf, src);
        if (ptmp = strstr(src, "市") || ptmp = strstr(src, "区") || ptmp = strstr(src, "县"))
            truncate_and_save_string(src, ptmp, ptr->region);
        else
            sprintf(ptr->region, "%s", src);
        ptr->flaglevel = CITY;
    }
    else if (tmp == country_level_len){
        sprintf(ptr->allname, "%s%s%s", province_level_buf, city_level_buf, src);
        if (ptmp = strstr(src, "市") || ptmp = strstr(src, "区") || ptmp = strstr(src, "县"))
            truncate_and_save_string(src, ptmp, ptr->region);
        else
            sprintf(ptr->region, "%s", src);
        ptr->flaglevel = COUNTRY;
    }
     
    return 0;
}


static int pasebuf(CIDINFO *ptr, char buf[])
{
   int i = 0, tmp;
   char tmpbuf[64];
   while (*buf != ' '){
    ptr->cityid[i++] = *buf++;
   }
   ptr->cityid[i] = '\0';
    
   i = 0;
   while (*buf == ' ')
       i++;

   tmp = i;

   i = 0;
   while (*buf != '\n' && *buf != ' ')
       tmpbuf[i++] = *buf++;
    
   tmpbuf[i] = '\0';

   copy_full_name(ptr, tmpbuf, tmp);

    return 0;
}


static int deal_read_buf(char buf[])
{
    while (1){
        CIDINFO * ptr = (CIDINFO *)malloc(sizeof(CIDINFO));
        if (NULL == ptr){
            perror("malloc...");
            continue;
        }

        break;
    }
        pasebuf(ptr, buf);

       list_add_tail(&g_list->entry, &ptr->entry);

}

static int readfile(FILE *fp)
{
    char buf[256];
    char *retptr;
    
    recover_file_pointer(fp);
    while (1){
        retptr = fgets(buf, 256, fp);
        if (NULL == retptr) {
            if (feof(fp))
                break;
            else {
                perror("fgets...");
                continue;
            }      
        }
        
        deal_read_buf(buf);    
    }

    return 0;
}


static int construct_cityid_cache(void)
{
    init_dlist(&g_list->entry);

    getfile();
    
    getstringformat();

    readfile(fp);

    return 0;
}




