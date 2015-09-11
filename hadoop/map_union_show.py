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


pv_dict = {}

class Task:
    def __init__(self):
        self.data_src = ['pd','pt','filtered','np','cmatch_ad_src_type']
        self.req_str = ''

    def set_adshow(self, cmatch_ad_src_type):
        cmatch_ad_src_type = cmatch_ad_src_type.strip()
        if not cmatch_ad_src_type:
            return

        if ',' in cmatch_ad_src_type:
            if pv_dict.has_key(self.req_str):
                values = pv_dict[self.req_str]
                if values.has_key('adshow'):
                    values['adshow'] += 1
                else:
                    values['adshow'] = 1
                pv_dict[self.req_str] = values
            else:
                values = {}
                values['adshow'] = 1
                pv_dict[self.req_str] = values


    def set_show(self):
        if pv_dict.has_key(self.req_str):
            values = pv_dict[self.req_str]
            if values.has_key('show'):
                values['show'] += 1
            else:
                values['show'] = 1
            pv_dict[self.req_str] = values
        else:
            values = {}
            values['show'] = 1
            pv_dict[self.req_str] = values

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

        if  int(data_dict['pd']) != 29:
            return
        self.req_str = ''
        #print "data_dict ==>", data_dict

        self.req_str = '%s %s %s' % (data_dict['pt'], data_dict['np'], data_dict['filtered'])
        #print "req_str ===> %s" % (self.req_str,)

        self.set_show()

        if data_dict.has_key('cmatch_ad_src_type'):
            #print 'cmatch_ad_src_type ==> ', data_dict['cmatch_ad_src_type']
            self.set_adshow(data_dict['cmatch_ad_src_type'])

        #if data_dict.has_key('ds_ret_src_no_list'):
            #self.get_ds_ret(data_dict['ds_ret_src_no_list'])


    def out_string(self):
        for key in pv_dict:
            values = pv_dict[key]

            show = 0
            adshow = 0
            if values.has_key('show'):
                show = values['show']

            if values.has_key('adshow'):
                adshow = values['adshow']

            out_string = ''
                        #out_string = 'req:%s_%s_type:%s   show:%d   adshow:%d   pvr:%d''' % (key, cmatch, secondkey, show, values[secondkey],  values[secondkey] * 1000.0/show)
            out_string = '%s %d %d''' % (key, adshow, show)
            print out_string
                #else:
                #    out_string = 'req:%s  show:%d  type:%s  adshow:%d   pvr:%.5f''' % (key_string, show, "0", 0,  0 * 100.0/show)
                #    print out_string


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
    #isFiles = False
    #is_filter = False
    #files = []
    #filter_files = []
    #res_files = []
    #
    #try:
    #    #opts,args = getopt.getopt(sys.argv[1:],"f:")
    #    #for op,value in opts:
    #    #    if op == '-f':
    #    #        #tmp = os.popen("ls "+value).read()
    #    #        #print tmp
    #    #        files = sys.argv[2:]
    #    #        isFiles =  True
    #
    #    args = sys.argv[1:]
    #    index = 0
    #    if '-f' in args:
    #        index = args.index('-f')
    #        files = args[index + 1:]
    #        res_files = files
    #        isFiles =  True
    #
    #    if '-c' in args:
    #        index = args.index('-c')
    #        filter_files = args[index + 1:]
    #        is_filter = True
    #
    #
    #    if is_filter:
    #        res_files = []
    #        files = files[:index - 1]
    #
    #        for file in files:
    #            flag = 0
    #            for file_filter in filter_files:
    #                if file.startswith(file_filter):
    #                    flag = 1
    #                    break
    #            if flag:
    #                continue
    #            res_files.append(file)
    ##print res_files
    #
    #except getopt.GetoptError as e:
    #    print e
    #
    #if isFiles:
    #    doTask2(res_files)
    #    pass
    #else:
    doTask1()


