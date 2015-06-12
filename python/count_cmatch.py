#/usr/bin/env python
#encoding=gbk


#=============================================================================
#     FileName: count_cmatch.py
#         Desc:
#       Author: zm
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2015-05-21 11:08:15
#      History:
#=============================================================================

import sys


class Task:
    def __init__(self):
        self.ds_ret = {}
        self.asp_ret = {}

    def get_ds_ret(self):
        for line in sys.stdin:
            line = line.strip()
            if not line:
                continue
            if 'ds ret cmatch' not in line or 'ad_src_no' not in line:
                continue;

            line = line[line.find('ds ret cmatch') + 1:]
            line = line.strip()
            items = line.split(":")
            cmatch = items[0]
            ad_src_no = items[2]
            values = {}
            if self.ds_ret.has_key(cmatch):
                values = self.ds_ret[cmatch]
                if values.has_key(ad_src_no):
                    number = values[ad_src_no] + 1
                    values[ad_src_no] = number
                    self.ds_ret[cmatch] = values
                else:
                    values[ad_src_no] = 1
                    self.ds_ret[cmatch] = values
            else:
                values[ad_src_no] = 1
                self.ds_ret[cmatch] = values



    def get_asp_ret(self):
        for line in sys.stdin:
            line = line.strip()
            if not line:
                continue

            if 'cmatch_ad_src_type' not in line:
                continue

            items = line.split(" ")
            for item in items:
                item = item.strip()
                if 'cmatch_ad_src_type' not in item:
                    continue

                sub_items = item.split("=")
                cmatch_src = sub_items[1]
                break;

            if cmatch_src:
                sub_items = cmatch_src.split(";")
                for sub in sub_items:
                    temp = sub.split(",")
                    cmatch = temp[0]
                    ad_src_no =  temp[1]
                    values = {}
                    if self.asp_ret.has_key(cmatch):
                        values = self.asp_ret[cmatch]
                        if values.has_key(ad_src_no):
                            number = values[ad_src_no] + 1
                            values[ad_src_no] = number
                            self.asp_ret[cmatch] = values
                        else:
                            values[ad_src_no] = 1
                            self.asp_ret[cmatch] = values
                    else:
                        values[ad_src_no] = 1
                        self.asp_ret[cmatch] = values

    def out_put_ds_ret_str(self):
        print self.ds_ret

    def out_put_asp_ret_str(self):
        print self.asp_ret

    def count(self):
        pass




if __name__ == '__main__':
    task = Task()
    #task.get_ds_ret()
    #task.out_put_ds_ret_str()

    task.get_asp_ret()
    task.out_put_asp_ret_str()