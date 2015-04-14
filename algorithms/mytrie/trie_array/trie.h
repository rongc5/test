#ifndef __TRIE_H__
#define __TRIE_H__

#define SAMPLE 36

struct trie_node{
    char key;
    struct trie_node *branch[SAMPLE];
    int isvalid;
    int count;
};



#endif
