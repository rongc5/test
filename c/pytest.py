#!/usr/bin/python2.6
import os, sys
def add(a,b):
    print "in python function add"
    print "a = " + str(a)
    print "b = " + str(b)
    print "ret = " + str(a+b)
    return a + b

if __name__ == '__main__':
    if len(sys.argv) < 2:
        exit(0)
    add(int(sys.argv[1]), int(sys.argv[2]))
