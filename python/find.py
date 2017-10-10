#/usr/bin/env python
#coding=utf-8

import sys, redis




def do():

    r = redis.StrictRedis(IP, PORT, 0)
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        items = line.split(" ")
        if len(items) != 2:
            continue

        if items[1] > 2:
            continue
        res = r.hget(items[0], items[1])
        if





PORT = 8888
IP="223.99.226.165"
if __name__ == '__main__':
    #daemon.daemonize()
    do()