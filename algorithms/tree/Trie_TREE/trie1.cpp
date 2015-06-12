#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int trieVal_t;

typedef struct trieNode{
    char key;
    trieNode value;
    struct trieNode *next, *prev, *children, *parent;
}trieNode_t;

trieNode_t * TrieSearch(trieNode_t * root, const char *key);

#define DEBUG

trieNode_t *TrieCreateNode(char key, int data)
{
    trieNode_t *node = NULL;
    node = (trieNode_t *)malloc(sizeof(trieNode_t));
    if (!node){
        printf("malloc failed\n");
        return noade;
    }

    node->key = key;
    node->next = NULL;
    node->children = NULL;
    node->value = data;
    node->parent = NULL;
    node->prev = NULL;

    return node;
}

void TrieAdd(trieNode_t **root, char *key, int data)
{
    trieNode_t *pTrav = NULL;
 
    if(NULL == *root)
    {
        printf("NULL tree\n");
        return;
    }
#ifdef DEBUG
    printf("\nInserting key %s: \n",key);
#endif
    pTrav = (*root)->children;
 
    if(TrieSearch(pTrav, key))
    {
        printf("Duplicate!\n");
        return;
    }
 
    if(pTrav == NULL)
    {
        /*First Node*/
        for(pTrav = *root; *key; pTrav = pTrav->children)
        {
            pTrav->children = TrieCreateNode(*key, 0xffffffff);
            pTrav->children->parent = pTrav;
#ifdef DEBUG
            printf("Inserting: %c\n",pTrav->children->key);
#endif
            key++;
        }
 
        pTrav->children = TrieCreateNode('\0', data);
        pTrav->children->parent = pTrav;
#ifdef DEBUG
        printf("Inserting: %c\n",pTrav->children->key);
#endif
        return;
    }
 
    while(*key != '\0')
    {
        if(*key == pTrav->key)
        {
            key++;
#ifdef DEBUG
            printf("Traversing child: %c\n",pTrav->children->key);
#endif
            pTrav = pTrav->children;
        }
        else
            break;
    }
 
    while(pTrav->next)
    {
        if(*key == pTrav->next->key)
        {
            key++;
            TrieAdd(&(pTrav->next), key, data);
            return;
        }
        pTrav = pTrav->next;
    }
 
    pTrav->next = TrieCreateNode(*key, 0xffffffff);
    pTrav->next->parent = pTrav->parent;
    pTrav->next->prev = pTrav;
 
#ifdef DEBUG
    printf("Inserting %c as neighbour of %c \n",pTrav->next->key, pTrav->key);
#endif
 
    key++;
 
    for(pTrav = pTrav->next; *key; pTrav = pTrav->children)
    {
        pTrav->children = TrieCreateNode(*key, 0xffffffff);
        pTrav->children->parent = pTrav;
#ifdef DEBUG
        printf("Inserting: %c\n",pTrav->children->key);
#endif
        key++;
    }
 
    pTrav->children = TrieCreateNode('\0', data);
    pTrav->children->parent = pTrav;
#ifdef DEBUG
    printf("Inserting: %c\n",pTrav->children->key);
#endif
    return;
}
 
trieNode_t* TrieSearch(trieNode_t *root, const char *key)
{
    trieNode_t *level = root;
    trieNode_t *pPtr = NULL;
 
    int lvl=0;
    while(1)
    {
        trieNode_t *found = NULL;
        trieNode_t *curr;
 
        for (curr = level; curr != NULL; curr = curr->next)
        {
            if (curr->key == *key)
            {
                found = curr;
                lvl++;
                break;
            }
        }
 
        if (found == NULL)
            return NULL;
 
        if (*key == '\0')
        {
            pPtr = curr;
            return pPtr;
        }
 
        level = found->children;
        key++;
    }
}
 
void TrieRemove(trieNode_t **root, char *key)
{
    trieNode_t *tPtr = NULL;
    trieNode_t *tmp = NULL;
 
    if(NULL == *root || NULL == key)
        return;
 
    tPtr = TrieSearch((*root)->children, key);
 
    if(NULL == tPtr)
    {
        printf("Key not found in trie\n");
        return;
    }
 
    while(1)
    {
        if( tPtr->prev && tPtr->next)
        {
            tmp = tPtr;
            tPtr->next->prev = tPtr->prev;
            tPtr->prev->next = tPtr->next;
            free(tmp);
            break;
        }
        else if(tPtr->prev && !(tPtr->next))
        {
            tmp = tPtr;
            tPtr->prev->next = NULL;
            free(tmp);
            break;
        }
        else if(!(tPtr->prev) && tPtr->next)
        {
            tmp = tPtr;
            tPtr->parent->children = tPtr->next;
            free(tmp);
            break;
        }
        else
        {
            tmp = tPtr;
            tPtr = tPtr->parent;
            free(tmp);
        }
    }
 
}

