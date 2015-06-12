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

import os


asp_pvr = {}
pv_dict = {}

class Task:
    def __init__(self, src_name):
        self.src_name = src_name
        asp_pvr = {}
        self.data_src = ['pd','pt','wvar','cmatch_ad_src_type', 'ds_ret_src_no_list']
        self.req_str = ''

    def get_ad_srcno(self, cmatch_ad_src_type):
        cmatch_ad_src_type = cmatch_ad_src_type.strip()
        if not cmatch_ad_src_type:
            return

        items = cmatch_ad_src_type.split(';')
        for item in items:
            item = item.strip()
            key_str = ''
            if item:
                sub_items = item.split(',')
                if len(sub_items) != 2:
                    continue
                cmatch = sub_items[0]
                if not sub_items[1]:
                    print sub_items
                ad_src_no = int(sub_items[1])
                key_str = '%s %s' % (self.req_str, cmatch)
                if asp_pvr.has_key(key_str):
                    values = asp_pvr[key_str]
                    if ad_src_no >= 11 and ad_src_no <= 13:
                        if not values.has_key('baidu'):
                            values['baidu'] = 1
                        else:
                            values['baidu'] = values['baidu'] + 1
                    elif ad_src_no == 4:
                        if not values.has_key('ds'):
                            values['ds'] = 1
                        else:
                            values['ds'] = values['ds'] + 1
                    asp_pvr[key_str] = values
                else:
                    values = {}
                    if ad_src_no >= 11 and ad_src_no <= 13:
                        values['baidu'] = 1
                        asp_pvr[key_str] = values
                    elif ad_src_no == 4:
                        values['ds'] = 1
                        asp_pvr[key_str] = values


    def do_one_line(self, line):
        line = line.strip()
        if not line:
            return
        if 'NOTICE' not in line:
            return

        data_dict = {}
        items = line.split(" ")
        for item in items:
            item = item.strip(" ")

            sub_items = item.split('=')
            if len(sub_items) !=2:
                continue
            for key in self.data_src:
                if sub_items[0] == key:
                    value = sub_items[1].strip()
                    data_dict[key] = value
                    #if 'cmatch_ad_src_type' == sub_items[0]:
                    #    print sub_items[0], sub_items[1],," ===> ", item, " --< over"
                    break

        if len(data_dict) < len(self.data_src):
            return

        if not int(data_dict['pd']) or  not int(data_dict['pt']):
            return
        self.req_str = ''
        #print "data_dict ==>", data_dict

        t = ""
        if data_dict.has_key('wvar'):
            t = data_dict['wvar']

        self.req_str = '%s %s %s' % (data_dict['pd'], data_dict['pt'], t)
        #print "req_str ===> %s" % (self.req_str,)

        self.set_pv()

        if data_dict.has_key('cmatch_ad_src_type'):
            #print 'cmatch_ad_src_type ==> ', data_dict['cmatch_ad_src_type']
            self.get_ad_srcno(data_dict['cmatch_ad_src_type'])

        if data_dict.has_key('ds_ret_src_no_list'):
            self.get_ds_ret(data_dict['ds_ret_src_no_list'])

    def get_ds_ret(self, ds_ret_src_no_list):
        ds_ret_src_no_list = ds_ret_src_no_list.strip()
        if not ds_ret_src_no_list:
            return

        items = ds_ret_src_no_list.split(';')
        for item in items:
            item = item.strip()
            key_str = ''
            if item:
                sub_items = item.split(':')
                if len(sub_items) != 2:
                    continue
                cmatch = sub_items[0]
                ad_src_no = int(sub_items[1])
                key_str = '%s %s' % (self.req_str, cmatch)
                if asp_pvr.has_key(key_str):
                    values = asp_pvr[key_str]
                    if ad_src_no == 9:
                        if not values.has_key('ds_ret_baidu'):
                            values['ds_ret_baidu'] = 1
                        else:
                            values['ds_ret_baidu'] += 1
                else:
                    values = {}
                    if ad_src_no == 9:
                        values['ds_ret_baidu'] = 1


    def set_pv(self):
        if pv_dict.has_key(self.req_str):
            values = pv_dict[self.req_str]
            values += 1
            pv_dict[self.req_str] = values
        else:
            pv_dict[self.req_str] = 1


    def do(self):
        fp_src = open(self.src_name, "r")
        for line in fp_src:
            self.do_one_line(line)

        fp_src.close()
        print 'file %s do over!!' % (self.src_name, )





def out_string(dst_name):
        fp_dest = open(dst_name, "w")

        for key in asp_pvr:
            out_string = ''
            values = asp_pvr[key]

            show = 0
            #if values.has_key('baidu'):
            #    show += values['baidu']
            #
            #if values.has_key('ds'):
            #    show += values['ds']
            items = key.split(' ')
            key_string = '%s %s %s' %  (items[0], items[1], items[2])
            if not pv_dict.has_key(key_string):
                continue
            show = pv_dict[key_string]

            if values.has_key('ds_ret_baidu') and show:
                out_string = '''%s  pv:%s   baidu:%s    pver:%.5f''' % (key, show, values['ds_ret_baidu'], values['ds_ret_baidu'] * 100.0/show)
                fp_dest.write(out_string+'\n')

            if values.has_key('ds') and show:
                out_string = '''%s  pv:%s   ds:%s   pver:%.5f''' % (key, show, values['ds'], values['ds'] * 100.0/show)
                fp_dest.write(out_string+'\n')
        fp_dest.close()


src_log_name = "asp.log.nt"
dest_log_name = "log.pvr"

def doTask():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)
    for file in dirs:
        if src_log_name in file:
            task = Task(file)
            task.do()

if __name__ == '__main__':
    doTask()
    out_string(dest_log_name)

