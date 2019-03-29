#!/usr/bin/env python
#coding=gbk

__author__ = 'zhangming'


import os, sys

SRC_PATH = '/app/ecom/ad_easou/logs'
DEST_PATH = '/app/ecom/ad_easou/.lognew'
LOG_TMP= '/app/ecom/ad_easou/.tmp'

written_file_dict = {}
need_write_file_dict = {}
dest_file_dict = {}

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

def get_has_written():
    path = os.path.dirname(LOG_TMP)
    file = open(LOG_TMP)
    while 1:
        line = file.readline()
        line = line.strip('\n')
        if not line:
            break

        items = line.split('.')
        if len(items) < 4:
            continue

        written_file_dict[items[3]] = path + '/'+ line

def get_need_write():
    path = os.path.dirname(SRC_PATH)
    file = open(SRC_PATH)
    while 1:
        line = file.readline()
        line = line.strip('\n')
        if not line:
            break

        items = line.split('.')
        if len(items) < 4:
            continue

        if items[3] not in written_file_dict:
            need_write_file_dict[items[3]] = path + '/'+ line

def get_dest_write():
    path = os.path.dirname(DEST_PATH)
    file = open(DEST_PATH)
    while 1:
        line = file.readline()
        line = line.strip('\n')
        if not line:
            break

        items = line.split('.')
        if len(items) < 4:
            continue

        dest_file_dict[items[3]] = path + '/'+ line

def get_time_str(line):
    line = line.strip('NOTICE:')
    if not line:
        return

    items = line.split('ad_easou')
    if len(items) < 2:
        return
    time_str = ''
    time_str = items[0].replace('-', '')
    time_str = time_str.replace(':', '')
    time_str = time_str.replace(' ', '')

    return time_str

def get_dest_file(time_str):
    keys = dest_file_dict.keys()
    keys.sort()
    for key in keys:
        if time_str > key:
            return dest_file_dict[time_str]
    return ''

def do_insert_job(dest_line, filename):
    if not dest_line:
        return

    path = os.path.dirname(filename)
    dest_file = path + '/ad_easou.tmp.log'
    file = open(filename)
    while 1:
        line = file.readline()
        line = line.strip('\n')
        if not line:
            break

        if dest_line <= line:
            log_write(dest_file, dest_line)
        else:
            log_write(dest_file, dest_line)

    os.rename(dest_file, filename)

def do_insert():
    for key in need_write_file_dict:
        file = open(need_write_file_dict[key])
        while 1:
            line = file.readline()
            line = line.strip('\n')
            if not line:
                break

            if line.startswith("NOTICE") and 'show_ver' in line:
                time_str = get_time_str(line)
                time_str = need_write_file_dict[key][:8] + time_str
                dest_file = get_dest_file(time_str)
                if not dest_file:
                    continue

                do_insert_job(line, dest_file)



if __name__ == '__main__':
    pass