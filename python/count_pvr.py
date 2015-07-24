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

import os, sys, getopt


asp_pvr = {}
pv_dict = {}
req_dict = {}

class Task:
    def __init__(self):
        asp_pvr = {}
        self.data_src = ['pd','pt','wvar','cmatch_ad_src_type', 'ds_ret_src_no_list', 'ds_cnt']
        self.req_str = ''

    def get_ad_srcno(self, cmatch_ad_src_type):
        cmatch_ad_src_type = cmatch_ad_src_type.strip()
        if not cmatch_ad_src_type:
            return

        items = cmatch_ad_src_type.split(';')
        items = list(set(items))
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
                ad_src_no = sub_items[1]
                key_str = '%s %s' % (self.req_str, cmatch)
                if asp_pvr.has_key(key_str):
                    values = asp_pvr[key_str]
                    if not values.has_key(ad_src_no):
                        values[ad_src_no] = 1
                    else:
                        values[ad_src_no] += 1
                    asp_pvr[key_str] = values
                else:
                    values = {}
                    values[ad_src_no] = 1
                    asp_pvr[key_str] = values


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
                ad_src_no = sub_items[1]
                key_str = '%s %s' % (self.req_str, cmatch)
                if asp_pvr.has_key(key_str):
                    values = asp_pvr[key_str]
                    if int(ad_src_no) == 9:
                        if not values.has_key(ad_src_no):
                            values[ad_src_no] = 1
                        else:
                            values[ad_src_no] += 1
                        asp_pvr[key_str] = values
                else:
                    values = {}
                    if int(ad_src_no) == 9:
                        values[ad_src_no] = 1
                        asp_pvr[key_str] = values

    def get_ds_cnt(self, ds_cnt):
        ds_cnt = ds_cnt.strip()
        if not ds_cnt:
            return

        if not req_dict.has_key(self.req_str):
            values = set()
            req_dict[self.req_str] = values

        items = ds_cnt.split(';')
        for item in items:
            if item:
                sub_items = item.split(',')
                cmatch = sub_items[0]

                if not cmatch.strip():
                    continue

                values = req_dict[self.req_str]
                if cmatch not in  values:
                    values.add(cmatch)
                    req_dict[self.req_str] = values



    def set_pv(self):
        if pv_dict.has_key(self.req_str):
            values = pv_dict[self.req_str]
            values += 1
            pv_dict[self.req_str] = values
        else:
            pv_dict[self.req_str] = 1

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
        #过滤掉联盟, 联盟pvr 是按mmid, muid 算
        if not int(data_dict['pd']) or  not int(data_dict['pt']) or int(data_dict['pd']) == 29:
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

        #if data_dict.has_key('ds_ret_src_no_list'):
            #self.get_ds_ret(data_dict['ds_ret_src_no_list'])

        if data_dict.has_key('ds_cnt'):
            self.get_ds_cnt(data_dict['ds_cnt'])

    def out_string(self):
        for key in pv_dict:
            show = pv_dict[key]
            if not req_dict.has_key(key):
                continue

            cmatch_list = req_dict[key]
            for cmatch in cmatch_list:
                key_string = '%s %s' %  (key, cmatch)
                if asp_pvr.has_key(key_string):
                    values = asp_pvr[key_string]
                    #out_string = 'req:%s  show:%d  ' % (key_string, show)
                    for secondkey in values:
                        out_string = ''
                        out_string = 'req:%s_%s_type:%s   show:%d   adshow:%d   pvr:%d''' % (key, cmatch, secondkey, show, values[secondkey],  values[secondkey] * 1000.0/show)
                        print out_string
                #else:
                #    out_string = 'req:%s  show:%d  type:%s  adshow:%d   pvr:%.5f''' % (key_string, show, "0", 0,  0 * 100.0/show)
                #    print out_string

    def out_type(self):
           for key in pv_dict:
            show = pv_dict[key]
            if not req_dict.has_key(key):
                continue

            cmatch_list = req_dict[key]
            for cmatch in cmatch_list:
                key_string = '%s %s' %  (key, cmatch)
                if asp_pvr.has_key(key_string):
                    values = asp_pvr[key_string]
                    #out_string = 'req:%s  show:%d  ' % (key_string, show)
                    for secondkey in values:
                        out_string = ''
                        if int(secondkey)  == 0:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是KS广告')
                        elif int(secondkey) == 1:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是搜狗广告')
                        elif int(secondkey) == 2:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是online广告')
                        elif int(secondkey) == 4:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是DS广告')
                        elif int(secondkey) == 11:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是百度文本广告')
                        elif int(secondkey) == 12:
                            out_string = 'req:%s type:%s %s' % (key_string, secondkey, '代表返回的是百度图片广告')
                        if out_string.strip():
                            print out_string

def doTask1():
    task = Task()
    for line in sys.stdin:
        task.do_one_line(line)
    task.out_string()


def doTask2(dirs):
    task = Task()
    for file in dirs:
        fp_src = open(file, "r")
        for line in fp_src:
            task.do_one_line(line)
        fp_src.close()
    task.out_string()

def changeDir():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)

if __name__ == '__main__':
    changeDir()
    isFiles = False
    is_filter = False
    files = []
    filter_files = []
    res_files = []

    try:
        #opts,args = getopt.getopt(sys.argv[1:],"f:")
        #for op,value in opts:
        #    if op == '-f':
        #        #tmp = os.popen("ls "+value).read()
        #        #print tmp
        #        files = sys.argv[2:]
        #        isFiles =  True

        args = sys.argv[1:]
        index = 0
        if '-f' in args:
            index = args.index('-f')
            files = args[index + 1:]
            res_files = files
            isFiles =  True

        if '-c' in args:
            index = args.index('-c')
            filter_files = args[index + 1:]
            is_filter = True


        if is_filter:
            res_files = []
            files = files[:index - 1]

            for file in files:
                flag = 0
                for file_filter in filter_files:
                    if file.startswith(file_filter):
                        flag = 1
                        break
                if flag:
                    continue
                res_files.append(file)


    except getopt.GetoptError as e:
        print e

    if isFiles:
        doTask2(res_files)
        pass
    else:
        doTask1()


