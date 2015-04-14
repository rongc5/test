#ifndef __TRIE_H__
#define __TRIE_H__

typedef struct trie_tree{
  int isvalid;
  char key;
  int count;
  struct trie_tree *prev, *next, *parent, *child;
}TRIE;


#endif
