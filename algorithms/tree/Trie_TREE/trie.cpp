#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


using namespace std;

const int num_chars = 26;
class Trie{
    public:
        Trie();
        Trie(Trie& tr);
        virtual ~Trie();
        int Trie_search(const char * word, char *entry) const;
        int insert(const char * word, const char *entry);
        int remove(const char *word, char *entry);
    protected:
        struct Trie_node{
            char *data;
            Trie_node *branch[num_chars];
            Trie_node();
        };
        Trie_node * root;
};

Trie::Trie_node::Trie_node()
{
    data = NULL;
    for (int i = 0; i < num_chars; i++)
        branch[i] = NULL;
}
Trie::Trie():root(NULL){}
Trie::~Trie(){}

int Trie::Trie_search(const char * word, char * entry) const
{
    int position = 0;
    char char_code;

    Trie_node *location = root;
    while (location != NULL && *word != 0)
    {
        if (*word >= 'A' && *word <= 'Z')
            char_code = *word - 'A';
        else if (*word >= 'a' && *word <= 'z')
            char_code = *word - 'a';
        else
            return 0;

        location = location->branch[char_code];
        position++;
        word++;
    }

    if (location != NULL && location->data != NULL){
        strcpy(entry, location->data);
        return 1;
    }
    else
        return 0;
}

int Trie::insert(const char *word, const char *entry)
{
    int result = 1, position = 0;
    if (root == NULL)
        root = new Trie_node;
    char char_code;
    Trie_node *location = root;
    while (location != NULL && *word != 0)
    {
        if (*word >= 'A' && *word <= 'Z')
            char_code = *word - 'A';
        else if (*word >= 'a' && *word<= 'z')
            char_code = *word - 'a';
        else
            return 0;

        if (location->branch[char_code] == NULL)
            location->branch[char_code] = new Trie_node;

        location = location->branch[char_code];
        position++;
        word++;
    }

    if (location->data != NULL)
        result = 0;
    else{
        location->data = new char[strlen(entry)+1];
        strcpy(location->data, entry);
    }

    return result;
}

int main(int argc, char *argv[])
{
    Trie t;
    char entry[100];
    t.insert("a", "DET");
    t.insert("abacus","NOUN");     
    t.insert("abalone","NOUN");     
    t.insert("abandon","VERB");     
    t.insert("abandoned","ADJ");    
    t.insert("abashed","ADJ");     
    t.insert("abate","VERB");      
    t.insert("this", "PRON");

    if (t.Trie_search("this", entry))        
        cout<<"'this' was found. pos: "<<entry<<endl;     
    if (t.Trie_search("abate", entry))        
        cout<<"'abate' is found. pos: "<<entry<<endl;     
    if (t.Trie_search("baby", entry))        
        cout<<"'baby' is found. pos: "<<entry<<endl;     
    else      
        cout<<"'baby' does not exist at all!"<<endl;  

    return 0;
}



