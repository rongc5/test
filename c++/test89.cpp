#ifdef __GNUC__                       
#include <ext/hash_map>               
#else                                 
#include <hash_map>                   
#endif                                
                                      
                                      
namespace std                         
{                                     
using namespace __gnu_cxx;            
}                                     
                                      
using  namespace std;                 
int main()                            
{                                     
                                      
//std::hash_map<int,int> hm;          
hash_map<int,int> hm;                 
                                      
return 0;                             
}

