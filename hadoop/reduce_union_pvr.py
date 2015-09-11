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


pv_dict = {}
class Task:

    def do_one_line(self, line):
        line = line.strip()
        if not line:
            return

        items = line.split()
        if len(items) < 4:
            return

        key_string = '%s %s %s' % (items[0], items[1], items[2])
        values = {}
        if not pv_dict.has_key(key_string):
            pv_dict[key_string] = values


        values = pv_dict[key_string]

        if not values.has_key('show'):
            values['show'] = int(items[4])
        else:
            values['show'] += int(items[4])

        if not values.has_key('ad_show'):
             values['ad_show'] = int(items[3])
        else:
             values['ad_show'] += int(items[3])

        pv_dict[key_string] = values


    def out_string(self):
        for key in pv_dict:
            values = pv_dict[key]

            show = 0
            ad_show = 0
            if values.has_key('show'):
                show = values['show']


            if values.has_key('ad_show'):
                ad_show = values['ad_show']

            out_string = ''
            out_string = '%s %d %d' % (key, ad_show, show)
            print out_string



def doTask():
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
    doTask()
