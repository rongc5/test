#!/usr/bin/env python
#coding=gbk

import os, sys, time
import random
import shutil

ip_dict = ['10.26.27.125', '10.26.27.126', '10.26.27.127', '10.26.27.128', '10.26.28.39', '10.26.28.40', '10.26.28.43', '10.26.28.44',
           '10.26.27.118', '10.26.27.119']




def do_rename(path):
    if not os.path.isdir(path):
        return

    date = path.replace('-', '')
    files = os.listdir(path)
    for file in files:
        if  os.path.isdir(file):
            continue
        items = file.split('.')
        if len(items) < 3:
            continue

        items[0] = 'ad_easou.log.nt'
        hour = ''
        if len(items[1]) == 1:
            hour = '0%s' % (items[1])
        else:
            hour = items[1]

        minutes = random.randint(10, 30)
        second = random.randint(10, 60)

        items[1] = '%s%s%02d%02d' % (date, hour, minutes, second)
        items[2] = ip_dict[int(items[2]) % len(ip_dict)]

        dest_path = '%s/' % (items[2])
        os.system('mkdir -p %s' % dest_path)
        src_file = '%s/%s' % (path, file)
        dest_file = '%s/%s.%s' % (dest_path, items[0], items[1])
        #os.rename(src_file, dest_file)
        #shutil.move(src_file, dest_file)
        os.system('iconv -c -f utf-8 -t gbk %s > %s' % (src_file, dest_file))
        #shutil.copy2(src_file, dest_file)

if __name__ == '__main__':
    #dirs =['2019-01-27', '2019-01-29', '2019-01-28', '2019-01-30']
    dirs =['2019-01-27', '2019-01-29', '2019-01-28']
    for key in dirs:
        do_rename(key)
