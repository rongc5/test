#coding=utf-8
#!/usr/bin/python

import sys

while 1:
    line = sys.stdin.readline()
    if not line:
        break
    print line[:-1]
