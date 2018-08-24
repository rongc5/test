#!/usr/bin/env python
#coding=utf-8


import time
import subprocess
import os
import optparse

def log_write(filename, str):                                                                                                                                                   
    file = open(filename, 'a') 
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

LOG_NAME='search_res'


HADOOP_BIN='/data/mingz/hadoop-2.8.1/bin/hadoop'
HADOOP_HDFS='hdfs://10.26.26.145:8020'


def do_search():
    parser = optparse.OptionParser()
    parser.add_option("-b", "--bgein",action="store", dest="begin",type="string",default="",
                      help="begin like 20180810")

    parser.add_option("-e", "--end",action="store", dest="end",type="string",default="",
                      help="end like 20180810")

    options, args = parser.parse_args()
    hadoop_dir = '%s%s' % (HADOOP_HDFS, '/rs/response_log')
    cmd = [HADOOP_BIN, 'fs', '-ls', hadoop_dir]
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    #print res_str
    items =  res_str.split(' ')
    sub_items = {}
    for key in items:
        if key.startswith('hdfs://'):
            ss_items = key.split('\n')

            path = ss_items[0]
            date = os.path.basename(path)
            if options.begin and date >= options.begin:
                if options.end:
                    if date <= options.end:
                        sub_items[path] = date
                else:
                    sub_items[path] = date



    print sub_items, len(sub_items)

    for key in sub_items:
        #cmd = [HADOOP_BIN, 'fs', '-cat', key, '|', 'grep', '\"recommType\"\:202', '|', 'grep', '\"feature\"\:0', '|', 'wc -l']
        cmd = "%s fs -cat %s/response.log* | grep '\"recommType\"\:202' | grep '\"feature\"\:0' | wc -l" % (HADOOP_BIN, key)
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE, shell=True)
        #p.wait()
        res_str = sub_items[key]
        res_str += '\t'
        while True:
            out_str = p.stdout.readline()
            if out_str == '' and p.poll() is not None:
                break
            if out_str:
                print out_str.strip(), sub_items[key]

        res_str += out_str
        log_write(LOG_NAME, res_str)


if __name__ == '__main__':
    do_search()




