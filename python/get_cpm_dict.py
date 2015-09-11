#/usr/bin/env python
#encoding=gbk


#=============================================================================
#     FileName: get_cpm_dict.py
#         Desc:
#       Author: zm
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2015-07-22 11:00:09
#      History:
#=============================================================================

import os,getopt,sys

ratio_dict = {'c_230':'0.7'}


class Task:
    def __init__(self, src_file, dst_file):
        self.src_dict = {}
        self.dst_dict = {}
        self.src_file = src_file
        self.dst_file = dst_file

    def do(self):
        self.init_src_dict()
        self.init_dst_dict()
        self.out_res()
    def parse_src_line(self, line):
        items = line.split()
        if (len(items) < 5):
            return
        i = 0
        pd = ''
        pt = ''
        wvar = ''
        cmatch = ''
        ad_type = ''
        for item in items:
            if not item.strip():
                continue
            tmp = ''
            if ':' in item:
                item = item.split(':')
                tmp = item[1]
            else:
                tmp = item

            if (0 == i):
                pd = tmp
                i = i + 1
            elif (1 == i):
                pt = tmp
                i = i + 1
            elif (2 == i):
                wvar = tmp
                i = i + 1
            elif (3 == i):
                cmatch = tmp
                i = i + 1
            elif (4 == i):
                ad_type = tmp
                i = i + 1

            if i > 4:
                break



        if ad_type == '' or cmatch == '':
            return
        if int(ad_type) < 9:
            return

        key = '%s_%s' % (wvar, cmatch)
        if not self.src_dict.has_key(key):
            values = set()
            self.src_dict[key] = values

        values = self.src_dict[key]
        if ad_type not in  values:
            values.add(ad_type)
            self.src_dict[key] = values



    def init_src_dict(self):
        fp = open(self.src_file, "r")
        for line in fp:
            self.parse_src_line(line)
        fp.close()


    def parse_dst_line(self, line):
        items = line.split()
        key = '%s_%s' % (items[1], items[0])
        if not self.dst_dict.has_key(key):
            self.dst_dict[key] = items[4]


    def init_dst_dict(self):
        fp = open(self.dst_file, "r")
        for line in fp:
            self.parse_dst_line(line)
        fp.close()

    def out_res(self):
        for key in self.dst_dict:
            rate = self.dst_dict[key]
            if not self.src_dict.has_key(key):
                continue
            values = self.src_dict[key]

            for type in values:
                out_string = ''
                ratio = 0.8
                if ratio_dict.has_key(key):
                    ratio = float(ratio_dict[key])
                out_string = '%s_type:%s    %s    %d' % (key, type, rate, float(rate) * ratio * 1000)
                print out_string

def changeDir():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)

if __name__ == '__main__':
    changeDir()
    src_file = ''
    dst_file = ''
    try:
        opts,args = getopt.getopt(sys.argv[1:],"s:d:")
        for op,value in opts:
            if op == '-s':
                src_file = value
            elif op == '-d':
                dst_file = value

    except getopt.GetoptError as e:
        print e

    if not src_file or not dst_file:
        print "参数不对"
        sys.exit()

    task = Task(src_file, dst_file)
    task.do()


