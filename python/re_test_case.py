#!/usr/bin/env python
#encoding=utf-8


import urllib2
import json, os


BASE_URL = 'http://10.26.24.86:6011/recomm.json?recommType'
REDIS_CLI = 'redis-3.0.6/redis-cli'
REDIS1_NS = '10.26.25.15:6645'
REDIS1_AUTH = 'Easou_RDS2017'
REDIS2_NS='10.26.24.66:7000'

def redis_cmd_get(redis_cli, redis_ns, auth, cmd):
    ip = ''
    port = ''
    ip = redis_ns.split(':')[0]
    port = redis_ns.split(':')[1]

    str_cmd = ''
    if not auth.strip():
        str_cmd = "%s --raw -h %s -p %s -c '%s'" % (redis_cli, ip, port, cmd)
    else:
        str_cmd = "%s --raw -a %s -h %s -p %s -c '%s'" % (redis_cli, auth, ip, port, cmd)
    res = os.popen(cmd).read()
    return res

def test_201(base_url, search_dic):
    if len(search_dic):
        return False

    url = '%s=201&reqGid=%s&uid=%s&udid=%s&start=%s&count=%s&cid=%s' % (base_url, search_dic['reqGid'], search_dic['uid'],
                                                                        search_dic['udid'], search_dic['start'], search_dic['count'], search_dic['cid'])
    print url
    res_dic = {}
    try:
        f = urllib2.urlopen(url)
        res_dic = json.loads(f.read())
    except Exception, e:
        print e
        return False

    redis_cmd_get(REDIS_CLI, REDIS2_NS, '', 'hget cu_%s user_type' % (search_dic['uid']))









if __name__ == '__main__':
    test_201_1(BASE_URL)