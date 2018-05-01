#ifndef __MYDES__

#define __MYDES__

void rTrim(char * strIn);


class EncodeDes
{
public:
	EncodeDes();
	~EncodeDes();

	int  DESInit(const char *key);
	
	int  EncryptionFun(long len,const char  * inPut,char  * outPut);

	int  DecryptionFun(long len,const char  * inPut,char  * outPut);    

	void  des_strtohex(unsigned char *src, unsigned char *dst, int len);

	void  des_hextostr(unsigned char *src, unsigned char *dst);

	void    SetKey(const char* key);

	void    Decryption(char* iBlock);

	char*   GetSubKey(int i); 

	void    SetDebug(int i);

	void    IP(char* iBlock);

	void    RIP(char* iBlock);

	void    Encryption(char* iBlock);

	void    Transfer(char* l,char* r,char* key);

	void    Transfer2(char* l,char* r,char* key);

	void    E32To48(char* s,char* d);

	void    E48To32(char* s,char* d);

	int     SGetRow(char* s,int no);

	int     SGetColumn(char* s,int no);

	void    EBuildChar(char *ch, char* s,int *pos);

	void    BuildChar(char *ch, char* cn,char* dn,int *pos);

	void    SetSubKeys();	
	
public:
	char 	Key[9];
	char 	SubKey[16][7];
	int     Debug;	
};


#endif

