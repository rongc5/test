#include <stdio.h>
#include <openssl/aes.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#define AES_KEY_LENGTH 16  // aes-128

std::string String2Hex(const unsigned char *src, size_t len)
{
    std::string dest = ""; 
    for (size_t i = 0; i < len; ++i)
    {   
        char  tmp[3] = {0};
        snprintf(tmp, 3, "%02X", src[i]);
        dest += tmp;
    }   
    return dest;
}

std::string Hex2String(const std::string &hex)                                   
{
    std::string dest = ""; 
    if (hex.length() % 2 != 0)
    {   
        return dest;
    }   

    int len = hex.length();
    std::string newString;
    for(int i=0; i< len; i+=2)
    {   
        std::string byte = hex.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }           
    return newString;
}

std::string PKCS5Padding(const std::string& in) 
{
    std::string out = in; 
    size_t pad_len = AES_BLOCK_SIZE - out.length() % AES_BLOCK_SIZE;
    for (size_t i = 0; i < pad_len; ++i)
    {
        out.push_back(pad_len);
    }

    return out;
}

std::string PKCS5UnPadding(const std::string& in)
{
    std::string out = in;
    if (in.length() % AES_BLOCK_SIZE != 0 && in.length() >= AES_BLOCK_SIZE)
    {
        return "";
    }

    size_t pad_len = in[in.length()-1];

    try {
        out.erase(in.length() - pad_len);
    }
    catch(...) {
        return "";
    }

    return out;
}

std::string AESEncrypt(const std::string& key, const std::string& in)
{
    std::string out;
    if (key.length() != AES_KEY_LENGTH)
    {
        return out;
    }

    std::string src =  PKCS5Padding(in);

    AES_KEY aes_key;
    AES_set_encrypt_key((unsigned char *)key.c_str(), AES_KEY_LENGTH * 8, &aes_key);

    unsigned char *p = (unsigned char *)src.c_str();
    for (int i = 0; i < src.length() / AES_BLOCK_SIZE; ++i)
    {
        unsigned char dec[AES_BLOCK_SIZE] = {'\0'};
        AES_ecb_encrypt(p + i * AES_BLOCK_SIZE, dec, &aes_key, AES_ENCRYPT);
        out.append(String2Hex(dec, AES_BLOCK_SIZE));
    }

    return out;
}

std::string AESDecrypt(const std::string& key, const std::string& in)
{
    std::string out;
    if (key.length() != AES_KEY_LENGTH || in.length() % AES_BLOCK_SIZE != 0)
    {
        return out;
    }

    std::string dec = Hex2String(in);

    AES_KEY aes_key;
    AES_set_decrypt_key((unsigned char *)key.c_str(), AES_KEY_LENGTH * 8, &aes_key);

    unsigned char *p = (unsigned char *)dec.c_str();
    for (int i = 0; i < dec.length() / AES_BLOCK_SIZE; ++i)
    {
        unsigned char src[AES_BLOCK_SIZE + 1] = {'\0'};
        AES_ecb_encrypt(p + i * AES_BLOCK_SIZE, src, &aes_key, AES_DECRYPT);
        out.append((char *)src);
    }

    return PKCS5UnPadding(out);
}

#define DSP_DES_CBC_KEY "wina012300000000"

int main(int argc, char *argv[])
{
    std::string en_key(DSP_DES_CBC_KEY);

    std::string in("code_id=bSkL81c10018&os_ver=4.3&app_ver=1.0.0&android_id=504340280EA00B10&imei=860372032982284&mac=00:db:df:87:5a:7d&sw=375&sh=667&ot=3&ct=4&ip=180.173.15.245&device_type=1&os_type=1&vendor=vivo&model=x9+plus");

    std::string out = AESEncrypt(en_key, in);

    std::string res = AESDecrypt(DSP_DES_CBC_KEY, out);


    printf("%s\n", res.c_str());

    return 0;
}
