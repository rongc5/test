#!/usr/bin/env python
#encoding=utf-8


import urllib2
import json, os
import subprocess
import sys
import time
import hashlib

__author__ = 'mingz'

BASE_URL = 'http://10.26.24.86:6011/recomm.json?recommType'
REDIS_CLI = 'redis-3.0.6/redis-cli'
REDIS1_NS = '10.26.25.15:6645'
REDIS1_AUTH = 'Easou_RDS2017'
REDIS2_NS='10.26.24.66:7000'
RECOMM_APPID_EASOU2  =  20001
RECOMM_APPID_EASOU = 10001

RECOMM_ROOT_INTERVAL_TIME = 86400 * 3

RECOMM_UCF_MASK_FLAG = 127
RECOMM_UCF_ITEMINFO_NUM = 36

RECOMM_ROOT_CUT1_RINFO_ITEMS = 128

test_list = [{'uid':'48808231', 'reqGid':''}]

#获取用户类型
#1 免费
#2 准付费
#3 付费
#4 VIP付费



def create_url(search_dic):
    url = ''

    if not len(search_dic):
        return url

    url = '%s=%s' % (BASE_URL, search_dic['recommType'])

    if search_dic.has_key('uid'):
        url += '&uid=%s' % (search_dic['uid'])

    if search_dic.has_key('udid'):
        url += '&udid=%s' % (search_dic['udid'])

    if search_dic.has_key('start') and search_dic['start'] > 0:
        url += '&start=%d' % (search_dic['start'])

    if search_dic.has_key('cid'):
        url += '&cid=%s' % (search_dic['cid'])

    if search_dic.has_key('count') and search_dic['count'] > 0:
        url += '&count=%d' % (search_dic['count'])

    if search_dic.has_key('random') and search_dic['random'] > 0:
        url += '&random=%d' % (search_dic['random'])

    if search_dic.has_key('feature') and search_dic['feature'] > 0:
        url += '&feature=%d' % (search_dic['feature'])

    if search_dic.has_key('charge') and search_dic['charge'] > 0:
        url += '&charge=%d' % (search_dic['charge'])

    if search_dic.has_key('appid'):
        url += '&appid=%s' % (search_dic['appid'])

    print url
    return url


def lod_request_list1(path):
    res_list = []
    if not os.path.isfile(path):
        return res_list

    with open(path, encoding="GB2312") as a:
        res_dic = json.loads(a)
        #print res_dic
        for i in res_dic:
            print i.get('STAT_DATE')

    return res_list


def lod_request_list(path):
    res_list = []
    if not os.path.isfile(path):
        return res_list

    file = open(path)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        res_dic = json.loads(line)
                                                                            #print res_dic
        if res_dic.has_key('aaData'):
            res = res_dic['aaData']
            #print len(res)
            for key in res:
                #print key
                print has(key, 'STAT_DATE'), has(key, 'PHONE_SOFTVERSION'), has(key, 'ACTUSERS'), has(key, 'NEWUSERS'), '-', has(key, 'ALLUSERS'), has(key, 'UPUSERS'), has(key, 'ACTUSER_RATIO'), has(key, 'STARTTIMES')

    file.close()


def lod_request_list2(path):
    res_list = []
    if not os.path.isfile(path):
        return res_list

    file = open(path)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        res_dic = json.loads(line)
                                                                            #print res_dic
        if res_dic.has_key('aaData'):
            res = res_dic['aaData']
            #print len(res)
            for key in res:
                #print key
                print has(key, 'STAT_DATE'), has(key, 'ACTUSERS'), has(key, 'NEWUSERS'), has(key, 'ALLUSERS'), has(key, 'MAUUSERS'), has(key, 'MAUUSER_RATIO'), has(key, 'STARTTIMES'),has(key, 'STARTTIMES_AVG')

    file.close()


def has(key, name):
    if key.has_key(name):
        return key.get(name);
    return '-'


if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf8')
    lod_request_list2(sys.argv[1]);
    pass
    #for line in sys.stdin:
    #    line = line.strip()
    #    if not line:
    #        continue
    #    item = line.split('_')
    #    if len(item) != 2:
    #        continue
    #    if len(get_current_user_cart(item[1])) and not len(get_history_user_cart(item[1])):
    #        print item[1]


    #res_list = []
    #res_list = lod_request_list('response.log')
    #for key in res_list:
    #    if key.has_key('recommType')and key['recommType'] == 201:
    #        test_201(key)
