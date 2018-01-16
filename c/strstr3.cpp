#include <iostream>  
#include <string>  
using namespace std;  
//c语言实现strstr  
  
const char* isSub(const char* str, const char *subs){  
    //特殊情况  
    if(!*subs)  
        return str;  
  
    const char* tmp=str;  
    while (*tmp!='\0')  
    {  
        //用于每次将父串向后移动一个字符  
        const char* tmp1=tmp;  
        //记录子串地址  
        const char* sub1=subs;  
  
        while (*sub1!='\0'&&*tmp1!='\0')  
        {  
            //若不相等则跳出，将父串后移一个字符  
            if (*sub1!=*tmp1)  
                break;  
            //若相等且子串下一个字符是末尾则是这个父串的子串  
            if (*sub1==*tmp1&&*(sub1+1)=='\0')  
                return tmp;  
            //若相等则继续比较下一个字符  
            if (*sub1==*tmp1)  
            {  
                sub1++;  
                tmp1++;  
            }  
        }  
        tmp++;  
    }  
    return NULL;  
}  
  
int main(){  
    char* str1="ababcdddb";  
    char* str="";  
    const char *res=isSub(str1,str);  
    if (res!=NULL)  
    {  
        cout << res << endl;  
    }  
    else  
        cout << "null" << endl;  
    //cout << isSub(str1,str) << endl;  
  
    return 0;  
}  
