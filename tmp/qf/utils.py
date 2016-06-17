#!/usr/bin/env python
#coding=utf-8

import threading, signal, os

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


def threadFun(signum, frame):

    subPid = int(getSubPid())
    if subPid:
        try:
            os.kill(subPid, signal.SIGKILL)
            truncateSubPid()
        except OSError:
            pass


def getSubPid():
    pid = ""
    try:
        fp = open("subPid", 'r')
        pid = str(fp.read())
        fp.close()
    except:
        pass

    return pid

def setSubPid(subPid):
    fp = open("subPid", 'w+')
    fp.write(str(subPid))
    fp.close()

def truncateSubPid():
    fo = open("subPid", "w+")
    fo.truncate()
    fo.close()


if __name__ == '__main__':
    #str = str.replace("\n", "\\n")
    str1 = "hello world\n"
    str2 = "hello       wor       ld \n"
    print cmp2str(str1, str2)


