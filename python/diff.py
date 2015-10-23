#/usr/bin/env python
#encoding=gbk


import os,sys

file1_dict = {}
file2_dict = {}


def init_dict(file, dict):
    fp = open(file, "r")
    for line in fp:
        line = line.strip()
        if not line:
            continue

        dict[line] = ""
    fp.close()


def get_difference():
    for key1 in file1_dict:
        flag = 0
        for key2 in file2_dict:
            if key1 == key2:
                flag = 1
                break

        if flag:
            continue

        print '%s' % (key1,)

if __name__ == '__main__':
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    init_dict(file1, file1_dict)
    init_dict(file2, file2_dict)
    get_difference()
