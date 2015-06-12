#include "communal.h"
#include "trie.h"

int init_trie_node(struct trie_node *node)
{
    int i = 0;

    if (!node)
        return -1;

    node->count = 0;
    node->isvalid = 0;
    node->key = 0;
    /*printf("%d\n", sizeof(node->branch)/sizeof(node->branch[0]));*/

    for (i = 0; i < sizeof(node->branch)/sizeof(node->branch[0]);i++)
        node->branch[i] = NULL;

    return 0;
}

int get_index(char *cword)
{
    int index = 0;

    if (*cword >='0' && *cword <='9')
        index = *cword - '0';
    else if (*cword >= 'a' && *cword <= 'z')
        index = *cword -'a' + 10;
    else if (*cword >= 'A' && *cword <= 'Z')
        index = *cword - 'A' + 10;
    else
        index = -1;

    return index;
}

int trie_insert(struct trie_node *root, char *word)
{
    struct trie_node *location = root;
    struct trie_node *e;
    int index, len;

    if (!location)
        return -3;
    
    while (location && *word != '\0'){
        index = get_index(word);
        if (index == -1){
            return -2;
        }

        if (!location->branch[index]){
            e = (struct trie_node *)malloc(sizeof(struct trie_node));
            if (!e)
                continue;
            init_trie_node(e);
            location->branch[index] = e;
        }

        location = location->branch[index];
        location->key = *word;
        /*printf("%c\n", location->key);*/
        word++;
    }
    

    if (*word == '\0')
        location->isvalid = 1;

    if (location->isvalid == 1)
        location->count += 1;

    return 0;
}

struct trie * trie_search(struct trie_node *root, char *word)
{
    struct trie_node *location = root;
    int index;

    if (!location)
        return NULL;

    while (location && *word != '\0'){
        index = get_index(word);
        if (index == -1) 
            return NULL;

        if (location->branch[index] == NULL)
            return NULL;

        location = location->branch[index];
        word++;
    }

    if (*word == '\0' && location && location->isvalid)
        return location;
    else
        return NULL;
}

void trie_display(struct trie_node *root)
{
    struct trie_node *location = root;
    int i;

    if (location){
        printf("%c", location->key);
        if (location->isvalid)
            printf("\n");

        /*printf("%d\n", sizeof(location->branch)/sizeof(location->branch[0]));*/
        
        for (i = 0; i < sizeof(location->branch)/sizeof(location->branch[0]); i++){
            if (location->branch[i]){
                trie_display(location->branch[i]);
            }
        }
    }
}

void trie_free(struct trie_node *root)
{
    struct trie_node *location = root;
    int i;

    if (location){
        for (i = 0; i < sizeof(location->branch)/sizeof(location->branch[0]); i++){
            if (location->branch[i]){
                trie_free(location->branch[i]);
                free(location->branch[i]);
                location->branch[i] = NULL;
            }
        }
    }
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
    struct trie_node trie, *ptr;
    init_trie_node(&trie);

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
    trie_display(&trie);
    trie_free(&trie);
    /*trie_display(&trie);*/
    return 0;
}
#endif
