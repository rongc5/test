#include <iostream>
#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/md2.h>


using namespace std;

//unsigned char *MD5(const unsigned char *d, unsigned long n,
                         //unsigned char *md)
//{
    //return md;
//}

namespace MYSPACE
{
    typedef unsigned char * (*mdx_pt)(const unsigned char *d, size_t n, unsigned char *md);

    struct ST{
    
        ST(){
            ctx = ::MD5;
        }

        mdx_pt ctx;
    };
}


int main(int argc, char *argv[])
{
    MYSPACE::ST st;

    return 0;
}
