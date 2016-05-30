#!/usr/bin/env python
#coding=utf-8



def trimStr(str):
    str.replace(" ", "")
    str.replace("\r", "")
    str.replace("\t", "")
    str.replace("\n", "")
    return str

def cmp2str(str1, str2):
    str1 = trimStr(str1)
    str2 = trimStr(str2)

    return id(str1) == id(str2)

