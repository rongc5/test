#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys

siddic = {}
sid2dic = {}

def getSid(filename):
    f = open(filename);
    line = f.readline();
    while line:
        items = line.split("\t")
        sid = items[0]
        values = ""
        #if siddic.has_key(sid):
        #    values = siddic[sid]
        #    values += genStr(items)[0]
        #    siddic[sid] = values
        #else:
        if not siddic.has_key(sid):
            values = ''.join(items[1:])
            siddic[sid] = values

        line = f.readline();
    f.close();

def getSid2(filename):
    f = open(filename);
    line = f.readline();
    while line:
        items = line.split("\t")
        sid = items[0]
        values = ""
        #if sid2dic.has_key(sid):
        #    values = sid2dic[sid]
        #    values += genStr(items[1:])
        #    sid2dic[sid] = values
        #else:
        if not sid2dic.has_key(sid):
            values = ''.join(items[1:])
            sid2dic[sid] = values

        line = f.readline();
    f.close();

def print_sidDic():
    for k in siddic:
        if k in sid2dic:
            print "%s\t%s\t%s\t%s" %(k, "1", siddic[k], sid2dic[k])
        else:
            print "%s\t%s\t%s" %(k, "0", siddic[k])


if '__main__' == __name__:
    getSid(sys.argv[1])
    getSid2(sys.argv[2])
    print_sidDic()