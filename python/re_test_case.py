#!/usr/bin/env python
#encoding=utf-8


import urllib2
import json, os
import subprocess

__author__ = 'mingz'

BASE_URL = 'http://10.26.24.86:6011/recomm.json?recommType'
REDIS_CLI = 'redis-3.0.6/redis-cli'
REDIS1_NS = '10.26.25.15:6645'
REDIS1_AUTH = 'Easou_RDS2017'
REDIS2_NS='10.26.24.66:7000'


test_list = [{'uid':'48808231', 'reqGid':''}]

#获取用户类型
#1 免费
#2 准付费
#3 付费
#4 VIP付费
def get_user_type(uid):
    ip = REDIS2_NS.split(':')[0]
    port = REDIS2_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 'cu_%s' % (uid), 'user_type']
    user_type = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    user_type = user_type.split('\n')[0]

    return user_type


#历史书架
#uid1_key u_48808231
#cid1_key cu_54159287
def get_history_user_cart(uid):
    ip = REDIS2_NS.split(':')[0]
    port = REDIS2_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 'u_%s' % (uid), 'cart_item']
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

    res_list = []
    res_str = res_str.strip('\n')
    if res_str.strip():
        res_list = res_str.split('\t')
    if not len(res_list):
        cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 'cu_%s' % (uid), 'cart_item']
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        res_str = res_str.strip('\n')
        if res_str.strip():
            res_list = res_str.split('\t')

    return res_list

#实时书架
#tid1_key t_54159287
def get_current_user_cart(uid):
    ip = REDIS2_NS.split(':')[0]
    port = REDIS2_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 't_%s' % (uid), 'cart_item']
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    return res_list

#tid3_key t_0EFF2921445097DC49E46156D5F6A18C
def get_current_ucf(uid, udid, field):
    ip = REDIS2_NS.split(':')[0]
    port = REDIS2_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 't_%s' % (uid), field]
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    if not len(res_list):
        cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 't_%s' % (udid), field]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        res_str = res_str.strip('\n')
        if res_str.strip():
            res_list = res_str.split('\t')

    return res_list


def get_history_ucf(uid, udid, field):
    pass

def test_201(search_dic):
    #print search_dic
    if not len(search_dic):
        return False

    url = create_url(search_dic)
    print url
    res_dic = {}
    try:
        f = urllib2.urlopen(url)
        res_dic = json.loads(f.read())
    except Exception, e:
        print e
        return False

    user_type = 0
    if search_dic.has_key('uid'):
        user_type = get_user_type(search_dic['uid'])

    print user_type

    history_cat = []
    current_cart = []
    history_cat = get_history_user_cart(search_dic['uid'])
    current_user_cart = get_current_user_cart(search_dic['uid'])

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
        if res_dic.has_key('guessResGids'):
            res_dic.pop('guessResGids')

        if res_dic.has_key('categoryRecGids'):
            res_dic.pop('categoryRecGids')
        res_list.append(res_dic)

    file.close()
    return res_list




if __name__ == '__main__':
    res_list = []
    res_list = lod_request_list('response.log')
    for key in res_list:
        if key.has_key('recommType')and key['recommType'] == 201:
            test_201(key)