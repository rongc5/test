#!/usr/bin/env python
#coding=utf-8



def trimStr(str):
    str = str.replace(" ", "")
    str = str.replace("\r", "")
    str = str.replace("\t", "")
    str = str.replace("\n", "")
    return str

def cmp2str(str1, str2):
    str1 = trimStr(str1)
    str2 = trimStr(str2)

    #print str1
    #print str2
    return str1 == str2

if __name__ == '__main__':
    #str = str.replace("\n", "\\n")
    str1 = "hello world\n"
    str2 = "hello       wor       ld \n"
    print cmp2str(str1, str2)


