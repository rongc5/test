#include "trie.h"
#include "communal.h"

int trie_init(TRIE *root)
{
    root->child = NULL;
    root->next = root->prev = root;
    root->parent = NULL;
    root->isvalid = 0;
    root->count = 0;
}

int trie_insert(TRIE *root, const char *word)
{
    TRIE *location = root, *e, *head;
    int flag = 0;
     
    while (location && *word != '\0'){
        if (location->child){
            head = location->child;
            e = head;
        }else{
            head = e = NULL;
        }
        flag = 0;

        if (head && head->key == *word){
                location = head;
                flag = 1;
                /*printf("flag:%d, %c\n", flag, *word);*/
        }

        for (e; !flag && e != head; e=e->next)
            if (e->key == *word){
                location = e;
                flag = 1;
            }

        if (!flag){
            e = (TRIE *)malloc(sizeof(TRIE));
            if (!e)
                return -1;

            trie_init(e);

            e->key = *word;
            if (head){
                e->prev = head->prev;
                e->next = head;
                head->prev->next = e;
                head->prev = e;
                /*printf("%c\n", e->key);*/
            }else {
                location->child = e;
            }
            e->parent = location;
            location = e;
        }
        word++;
    }

    if (*word == '\0'){
        e->isvalid = 1;
        e->count += 1;
    }
    
    return 0;
}

TRIE *trie_search(TRIE *root, const char *word)
{
    TRIE *location = root, *e, *head;
    int flag = 0;
     
    while (location->child && *word != '\0'){
        head = location->child;
        if (head)
            e = head->next;
        else
            e = NULL;
        flag = 0;

        if (head && head->key == *word){
                location = head;
                flag = 1;
                e = head;
        }

        for (e; !flag && e != head; e=e->next){
            if (e->key == *word){
                location = e;
                flag = 1;
            }
        }

        /*printf("flag:%d, %c\n", flag, *word);*/
        if (!flag)
            break;

        word++;
    }

    if (*word == '\0' && flag && e->isvalid){
        return e;
    }else 
        return NULL;

}

#ifdef TEST

const char *help = "\t\t\t2:查询\n"
                   "\t\t\t1:插入\n"
                   "\t\t\t3:退出\n";

int main(int argc, char *argv[])
{
    int option;
    char buf[128];

    printf("usage:%s", help);
    TRIE trie, *ptr;
    trie_init(&trie);

    while (1) 
    {
        printf("input your choose:");
        fflush(stdout);
        scanf("%d", &option); 
        /*printf("\n");*/

       switch (option)
       {
           case 1:
              printf("input the word you want to insert:");
              fflush(stdout);
              scanf("%s", buf);
              if (buf[strlen(buf) - 1] == '\n')
                  buf[strlen(buf) - 1] = '\0';
              trie_insert(&trie, buf); 
              /*printf("\n");*/
              break;
            case 2:
              printf("input the word you want to search:");
              fflush(stdout);
              scanf("%s", buf);
              if (buf[strlen(buf) - 1] == '\n')
                  buf[strlen(buf) - 1] = '\0';
              ptr = trie_search(&trie, buf);
              if (!ptr){
                  printf("%s is not exist!\n", buf);
                  continue;
              }
              /*printf("\n");*/
              printf("%s is exist!, number is %d\n", buf, ptr->count);
              break;
            case 3:
              printf("over\n");
              goto OUT; 
       }
    }
OUT:
    /*trie_display(&trie);*/
    /*trie_free(&trie);*/
    /*[>trie_display(&trie);<]*/
    return 0;
}
#endif



