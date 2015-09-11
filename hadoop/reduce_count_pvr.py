#/usr/bin/env python
#encoding=gbk


#=============================================================================
#     FileName: count_pvr.py
#         Desc:
#       Author: zm
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2015-05-22 11:00:09
#      History:
#=============================================================================

import os, sys


class Task:
    def __init__(self):
        self.asp_pvr = {}

    def do_one_line(self, line):
        line = line.strip()
        if not line:
            return

        items = line.split()
        if len(items) < 4:
            return

        key_string = '%s %s' % (items[0], items[1])
        values = {}
        if not self.asp_pvr.has_key(key_string):
            self.asp_pvr[key_string] = values


        values = self.asp_pvr[key_string]

        if not values.has_key('show'):
            values['show'] = int(items[2])
        else:
            values['show'] += int(items[2])

        if not values.has_key('ad_show'):
             values['ad_show'] = int(items[3])
        else:
             values['ad_show'] += int(items[3])

        self.asp_pvr[key_string] = values


    def out_string(self):
        for key in self.asp_pvr:
            values = self.asp_pvr[key]


            if not values.has_key('show'):
                continue
            else:
                show = values['show']


            if not values.has_key('ad_show'):
                continue
            else:
                ad_show = values['ad_show']

            out_string = ''
            out_string = 'cmatch:%s show:%d   adshow:%d   pvr:%d''' % (key, show, ad_show, ad_show* 1000.0/show)
            print out_string



def doTask1():
    task = Task()
    for line in sys.stdin:
        task.do_one_line(line)
    task.out_string()


def changeDir():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)

if __name__ == '__main__':
    changeDir()
    doTask1()


