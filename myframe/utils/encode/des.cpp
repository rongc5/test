#include "des.h"

#include <string.h>
#include "des.h"
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

static char c0[29] = {57, 49, 41, 33, 25, 17,  9,
							 1,  58, 50, 42, 34, 26, 18,
							 10,  2, 59, 51, 43, 35, 27,
							 19, 11,  3, 60, 52, 44, 36};

static char d0[29] = {63, 55, 47, 39, 31, 23, 15,
							 7,  62, 54, 46, 38, 30, 22,
							 14,  6, 61, 53, 45, 37, 29,
							 21, 13,  5, 28, 20, 12,  4};

static int ShiftCount[16] = {1,1,2,2,2,2,2,2,
									  1,2,2,2,2,2,2,1};

static int IPArr[64] = { 58, 50, 42, 34, 26, 18, 10,  2,
                         60, 52, 44, 36, 28, 20, 12,  4,
                         62, 54, 46, 38, 30, 22, 14,  6,
                         64, 56, 48, 40, 32, 24, 16,  8,
                         57, 49, 41, 33, 25, 17,  9,  1,
                         59, 51, 43, 35, 27, 19, 11,  3,
                         61, 53, 45, 37, 29, 21, 13,  5,
                         63, 55, 47, 39, 31, 23, 15,  7};

static int RIPArr[64] = { 40,  8, 48, 16, 56, 24, 64, 32,
                          39,  7, 47, 15, 55, 23, 63, 31,
                          38,  6, 46, 14, 54, 22, 62, 30,
                          37,  5, 45, 13, 53, 21, 61, 29,
                          36,  4, 44, 12, 52, 20, 60, 28,
                          35,  3, 43, 11, 51, 19, 59, 27,
                          34,  2, 42, 10, 50, 18, 58, 26,
                          33,  1, 41, 9,  49, 17, 57, 25};

static int BKExchgParm[48] =   {14, 17, 11, 24, 1,   5,
								  3,  28, 15,  6, 21, 10,
								  23, 19, 12,  4, 26,  8,
								  16,  7, 27, 20, 13,  2,
								  41, 52, 31, 37, 47, 55,
								  33, 40, 51, 45, 33, 48,
								  44, 49, 39, 50, 34, 53,
								  46, 42, 50, 36, 29, 32};

static int SelArr[48] = { 32,  1,  2,  3,  4,  5,
                          4,   5,  6,  7,  8,  9,
                          8,   9, 10, 11, 12, 13,
                          12, 13, 14, 15, 16, 17,
                          16, 17, 18, 19, 20, 21,
                          20, 21, 22, 23, 24, 25,
                          24, 25, 26, 27, 28, 29,
                          28, 29, 30, 31, 32,  1};

static int EIPArr[32]  = {16,  7, 20, 21,
                          29, 12, 28, 17,
                          1,  15, 23, 26,
                          5,  18, 31, 10,
                          2,   8, 24, 14,
                          32, 27,  3,  9,
                          19, 13, 30,  6,
                          22, 11,  4, 25};

static int Si[8][64]={{14,  4, 13,  1,  2, 15, 11,  8,
                        3, 10,  6, 12,  5,  9,  0,  7,
                        0, 15,  7,  4, 14,  2, 13,  1,
                       10,  6, 12, 11,  9,  5,  3,  8,
                        4,  1, 14,  8, 13,  6,  2, 11,
                       15, 12,  9,  7,  3, 10,  5,  0,
                       15, 12,  8,  2,  4,  9,  1,  7,
                        4, 11,  1, 14, 10,  0,  6, 13},
                      {15,  1,  8, 14,  6, 11,  3,  4,
                       9,   7,  1, 13, 12,  0,  5, 10,
                       3,  13,  4,  7, 15,  2,  8, 14,
                       12,  0,  1, 10,  6,  9, 11,  5,
                       0,  14,  7, 11, 10,  4, 13,  1,
                       5,   8, 12,  6,  9,  3,  2, 15,
                       13,  8, 10,  1,  3, 15,  4,  2,
                       11,  6,  7, 12,  0,  5, 14,  9},
                      {10,  0,  9, 14,  6,  3, 15,  5,
                       1,  13, 12,  7, 11,  4,  2,  8,
                       13,  7,  0,  9,  3,  4,  6, 10,
                       2,   8,  5, 14, 12, 11, 15,  1,
                       13,  6,  4,  9,  8, 15,  3,  0,
                       11,  1,  2, 12,  5, 10, 14,  7,
                       1,  10, 13,  0,  6,  9,  8,  7,
                       4,  15, 14,  3, 11,  5,  2, 12},
                      {7,  13, 14,  3,  0,  6,  9, 10,
                       1,   2,  8,  5, 11, 12,  4, 15,
                       13,  8, 11,  5,  6, 15,  0,  3,
                       4,   7,  2, 12,  1, 10, 14,  9,
                       10,  6,  9,  0, 12, 11,  7, 13,
                       15,  1,  3, 14,  5,  2,  8,  4,
                       3,  15,  0,  6, 10,  1, 13,  8,
                       9,   4,  5, 11, 12,  7,  2, 14},
                      {2,  12,  4,  1,  7, 10, 11,  6,
                       8,   5,  3, 15, 13,  0, 14,  9,
                       14, 11,  2, 12,  4,  7, 13,  1,
                       5,   0, 15, 10,  3,  9,  8,  6,
                       4,   2,  1, 11, 10, 13,  7,  7,
                       15,  9, 12,  5,  6,  3,  0, 14,
                       11,  8, 12,  7,  1, 14,  2, 13,
                       6,  15,  0,  9, 10,  4,  5,  3},
                      {12,  1, 10, 15,  9,  2,  6,  8,
                       0,  13,  3,  4, 14,  7,  5, 11,
                       10, 15,  4,  2,  7, 12,  9,  5,
                       6,   1, 13, 14,  0, 11,  3,  8,
                       9,  14, 15,  5,  2,  8, 12,  3,
                       7,   0,  4, 10,  1, 13, 11,  6,
                       4,   3,  2, 12,  9,  5, 15, 10,
                       11, 14,  1,  7,  6,  0,  8, 13},
                      {4,  11,  2, 14, 15,  0,  8, 13,
                       3,  12,  9,  7,  5, 10,  6,  1,
                       13,  0, 11,  7,  4,  9,  1, 10,
                       14,  3,  5, 12,  2, 15,  8,  6,
                       1,   4, 11, 13, 12,  3,  7, 14,
                       10, 15,  6,  8,  0,  5,  9,  2,
                       6,  11, 13,  8,  1,  4, 10,  7,
                       9,   5,  0, 15, 14,  2,  3, 12},
                      {13,  2,  8,  4,  6, 15, 11,  1,
                       10,  9,  3, 14,  5,  0, 12,  7,
                       1,  15, 13,  8, 10,  3,  7,  4,
                       12,  5,  6, 11,  0, 14,  9,  2,
                       7,  11,  4,  1,  9, 12, 14,  2,
                       0,   6, 10, 13, 15,  3,  5,  0,
                       2,   1, 14,  7,  4, 10,  8, 13,
                       15, 12,  9,  0,  3,  5,  6, 11}};

static const char* binary(char* ptr)
{
    static char buff[33];
    char* lp = buff;
    int i, k;
	unsigned char ch, mask;

	for(i = 0;i < 4; i++) {
        ch = ptr[i];
        mask = (unsigned char)0x80;
        for(k = 0;k<8;k++) {
            if( ch&mask )
                *lp++= '1';
            else
                *lp++= '0';
            mask>>=1;
        }
	}
    *lp = 0;

    return buff;
}


static void LShift(char*  buff)
{
	char c;
    int i;
	c = buff[0];

    for(i = 1;i<28;i++) {
        buff[i-1] = buff[i];
	}
    buff[27] = c;
}

void rTrim(char * strIn)
{
    int iLen = strlen(strIn) - 1;
    
	for (;iLen > 0;iLen--)
	{
		if (' ' == *(strIn+iLen))
		{			
			continue;
		}
		else
		{
			strIn[iLen+1] = '\0'; 
			break;         
		}				
	}
}

static void NLShift(char*  buff,int count)
{
	while(count--) LShift( buff );
}


EncodeDes::EncodeDes()
{

}

EncodeDes::~EncodeDes()
{

}

void EncodeDes::des_strtohex(unsigned char *src, unsigned char *dst, int len)
{
	int i;

	for(i = 0; i < len; i++)
	{
		int j = src[i];
		sprintf((char*)&dst[i*2], "%x", j >> 4);
		j = j & 0x0f;
		sprintf((char*)&dst[i*2+1], "%x", j);
	}
}

void EncodeDes::des_hextostr(unsigned char *src, unsigned char *dst)
{
    unsigned int i=0;
	long k;
    long l;
    char tmp[2];
	
    memset(tmp, 0x00, sizeof(tmp));

    for(; i < strlen((char *)src); i=i+2)
	{
		tmp[0] = src[i];
		k = strtol((char *)tmp, NULL, 16);
		k = k << 4;
		tmp[0] = src[i+1];
		l = strtol((char *)tmp, NULL, 16);
		k = k | l;
		dst[i/2] = k;
	}
}

int EncodeDes::EncryptionFun(long len,const char  * inPut,char  * outPut)
{
    int len1, i;
	char ibuff[9];
	char strBuf[10250];
	char *pos = strBuf;
	memset(strBuf, 0x00, sizeof(strBuf));

	long buf_len = (len % 8) ? (len / 8 + 1) * 8 : len;

    while(len>0) {

        if(len<8)
            memset(ibuff,' ',8);

        len1 = len>8 ? 8:len;

        for( i = 0; i< len1;i++)
            ibuff[i] = *inPut++;

        Encryption(ibuff);

        for( i = 0; i< 8;i++)
            *pos++ = ibuff[i];

        len -=8;
    }
	des_strtohex((unsigned char*)strBuf, (unsigned char*)outPut, buf_len);

	return 0;
}

int EncodeDes::DecryptionFun(long len,const char  * inPut,char  * outPut)
{
	char strBuf[10240];
	char strTmp[10240];
	int i = 0;
	int iLen = 0;
    char ibuff[9];
	char *pos = strBuf;
	char *posTmp = strTmp;

    memset(strTmp, 0x00, sizeof(strTmp));

	memset(strBuf, 0x00, sizeof(strBuf));

	memcpy(strTmp, inPut, len);
	des_hextostr((unsigned char*)strTmp, (unsigned char*)strBuf);

	iLen = len / 2;	
	long outbuf_len = iLen / 8 * 8;
    
    memset(strTmp, 0x00, sizeof(strTmp));

    while(iLen>7) 
    {
        for( i = 0; i< 8;i++)
            ibuff[i] = *pos++;

        Decryption(ibuff);

        for( i = 0; i< 8;i++)
            *posTmp++ = ibuff[i];

        iLen -=8;
    } 
    rTrim(strTmp);

    memcpy(outPut, strTmp, outbuf_len);

	return 0;
}

void EncodeDes::Encryption(char* iBlock)
{
	int i;

    if(Debug ) printf("\nIP INPUT:%s,%s\n", binary(iBlock), binary(&iBlock[4]));
    IP(iBlock);
    if(Debug ) printf("\nIP OUTPUT:%s,%s\n", binary(iBlock), binary(&iBlock[4]));

    for(i = 0;i<1;i++) {
        if(Debug )printf("ITERATION :%d\n", i+1);
        if(Debug )printf("L=%s\n", binary(iBlock));
        if(Debug )printf("R=%s\n", binary(&iBlock[4]));
        Transfer(iBlock,&iBlock[4],SubKey[i]);
        if(Debug )printf("L=%s\n", binary(iBlock));
        if(Debug )printf("R=%s\n\n", binary(&iBlock[4]));
    }
    if(Debug )printf("RIP INPUT:%s,%s\n", binary(iBlock), binary(&iBlock[4]));
    RIP(iBlock);
    if(Debug )printf("RIP OUTPUT:%s,%s\n", binary(iBlock), binary(&iBlock[4]));
}

void EncodeDes::Decryption(char* iBlock)
{
    int i;

    IP(iBlock);

    for(i = 0;i>=0;i--) {
        Transfer2(iBlock,&iBlock[4],SubKey[i]);
    }
    RIP(iBlock);
}

void EncodeDes::IP(char* buff)
{
    int i;
	char buff1[9];

    memcpy(buff1,buff,8);

    for(i = 0;i<8;i++)
        EBuildChar(&buff[i],buff1,&IPArr[8*i]);
}



void EncodeDes::RIP(char* buff)
{
    int i;
	char buff1[9];

    memcpy(buff1,buff,8);

    for(i = 0;i<8;i++)
        EBuildChar(&buff[i],buff1,&RIPArr[8*i]);
}

void EncodeDes::Transfer(char* l,char* r,char* key)
{
    int i;
	char rr[5];
    char buff[7];

    memcpy(rr,r,4);
    E32To48(rr,buff);

    for(i = 0;i<6;i++)
        buff[i] ^= key[i];

    E48To32(buff,rr);
    memcpy(buff,rr,4);

    for(i = 0;i<4;i++)
        EBuildChar(&rr[i],buff,&EIPArr[i*8]);

    for(i = 0;i<4;i++)
        rr[i] ^= l[i];

    memcpy(l,r,4);
    memcpy(r,rr,4);
}

void EncodeDes::Transfer2(char* l,char* r,char* key)
{
    int i;
	char ll[5];
    char buff[7];

    memcpy(ll,l,4);
    E32To48(ll,buff);

    for(i = 0;i<6;i++)
        buff[i] ^= key[i];

    E48To32(buff,ll);
    memcpy(buff,ll,4);

    for(i = 0;i<4;i++)
        EBuildChar(&ll[i],buff,&EIPArr[i*8]);

    for(i = 0;i<4;i++)
        ll[i] ^= r[i];

    memcpy(r,l,4);
    memcpy(l,ll,4);
}

void EncodeDes::E32To48(char* s,char* d)
{
	int i;
    for(i = 0;i<6;i++)
        EBuildChar(&d[i],s,&SelArr[8*i]);
}

void EncodeDes::E48To32(char* s,char* d)
{

    int i,m,n;

    for(i = 0; i< 4; i++) {
        m = SGetRow(s,i*2);
        n = SGetColumn(s,i*2);
        d[i] = Si[i*2][m*16+n] << 4;
        m = SGetRow(s,i*2 + 1);
        n = SGetColumn(s,i*2 + 1);
        d[i] |= Si[i*2+1][m*16+n];
    }
}

int EncodeDes::SGetRow(char* s,int no)
{
    int m1,n1,m2,n2;
    int ret;

	m1 = no*6;
    n1 = m1/8;
    m1 %= 8;
    m2 = no*6+5;
    n2 = m2/8;
    m2 %= 8;
    ret = 0;

    if(s[n1] & (0x80>>m1))
        ret +=2;

    if(s[n2] & (0x80>>m2))
        ret +=1;

    return ret;
}



int EncodeDes::SGetColumn(char* s,int no)
{
    int m,n,i,ret;
    ret = 0;

    for(i = 0;i< 4;i++) {
        m = no*6+i+1;
        n = m/8;
        m %= 8;

        if( s[n] & (0x80>>m) )
            ret |= 0x01<<(3-i);
    }
    return ret;
}

void EncodeDes::EBuildChar(char *ch, char* s,int *pos)
{

	unsigned char mask;
	int i,m,n;

	*ch = 0;

	for(i = 0;i<8;i++) {
        m = pos[i]-1;
		n = m/8;
		m = m%8;
		mask = 0x80>>m;

        if(s[n] & mask) {
			mask = 0x80>>i;
			*ch |=  mask;
		}
	}
}

int EncodeDes::DESInit(const char * key)
{

	memset(Key,' ',8);

	if(key) 
		SetKey(key);
	else
		SetSubKeys();

    Debug = 0;

	return 0;
}

void EncodeDes::SetKey(const char* key)
{
	int i = strlen(key);
    memcpy(Key,key,i>8 ? 8:i);
    SetSubKeys();
}

void EncodeDes::SetSubKeys()
{
	int i,j;
	char cn[29],dn[29];

    memcpy(cn,c0,28);
    memcpy(dn,d0,28);

	for(i = 0;i<16;i++) {
		NLShift(cn,ShiftCount[i]);
		NLShift(dn,ShiftCount[i]);
		for(j= 0;j<6;j++)
			BuildChar(&SubKey[i][j],cn,dn,&BKExchgParm[j*8]);
	}
}

void EncodeDes::BuildChar(char * ch,char* cn,char* dn,int *pos)
{
	unsigned char mask;
	int i,m,n;

	*ch = 0;

	for(i = 0;i<8;i++) {
		  if(pos[i]<= 28)
			m = cn[pos[i]-1];
		else
			m = dn[pos[i]-29];
		n = m/8;
        m %= 8;
		mask = 0x80>>m;

		if(Key[n] & mask) {
			mask = 0x80>>i;
			*ch |=  mask;
		}
	}
}

char*   EncodeDes::GetSubKey(int i)
{ 
	return SubKey[i]; 
}



void EncodeDes::SetDebug(int i) 
{
	Debug = i;
}

