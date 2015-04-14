

typedef type
char *
int

/*typedef enum {
    INT,
    CHAR *
}TYPE;*/

struct abs {
    int type;
    union {
        char *elm;
        int elm;
    };
};

void sum_ch(char * a, char *b, char * c)
{

}

int sum_in(int a, int b)
{
    return a + b;
}

type add(type a, type b)
{
   struct abs res;

   
}

int main(void)
