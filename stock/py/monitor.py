#!/usr/bin/env python
#encoding=utf-8

__author__ = 'Administrator'

import json
import time
import pycurl
import cStringIO
import gzip
import unicodedata
import os
import sys
import random

from time import strftime, localtime
from datetime import timedelta, date
import calendar


user_agent_dic = {}
user_agent_cookie = {}

user_agent_list = [
            "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.0 Safari/536.3",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24"
]

#1.0 版本不必支持chunked,
def httpGetContent(url, req_header=None, version = '1.1'):

    #print "==>", req_header, url, "<=="

    buf = cStringIO.StringIO()
    response_header = cStringIO.StringIO()
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEFUNCTION, buf.write)
    c.setopt(c.CONNECTTIMEOUT, 10)
    c.setopt(c.TIMEOUT, 100)
    c.setopt(pycurl.MAXREDIRS, 5)
    c.setopt(pycurl.FOLLOWLOCATION, 1)

    if req_header is None:
        req_header = []

    flag = 0
    for key in req_header:
        if  'User-Agent:' in key or  'user-agent:' in key:
            flag = 1
            break

    if not flag:
        print 'no User-Agent', req_header, url, sys._getframe().f_lineno
        return

    #if not flag:
    #    c.setopt(pycurl.USERAGENT, 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36')

    c.setopt(pycurl.TCP_NODELAY, 1)
    if '1.1' in version:
        add_headers = ['Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
             'Connection:keep-alive','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6','Cache-Control:max-age=0',
             'DNT:1','Upgrade-Insecure-Requests:1','Accept-Charset: utf-8']
        c.setopt(pycurl.ENCODING, 'gzip, deflate')
    else:
        add_headers = ['Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
             'Connection:close','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6','Cache-Control:max-age=0']
        c.setopt(pycurl.HTTP_VERSION, pycurl.CURL_HTTP_VERSION_1_0)
    if len(req_header):
        add_headers.extend(req_header)

    c.setopt(c.HTTPHEADER, add_headers)
    c.setopt(pycurl.HTTPGET, 1)
    c.setopt(c.HEADERFUNCTION, response_header.write)
    res = {}

    try:
        c.perform()
        str_head = '%s' % (response_header.getvalue())
        str_body = '%s' % (buf.getvalue())
        res['head'] = str_head
        res['body'] = str_body
        #print str_head, str_body
    except pycurl.error, error:
        errno, errstr = error
        print 'An error occurred: ', errstr, url
    c.close()
    buf.close()
    response_header.close()
    #print res
    return res

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()


#主要是针对chunked 模式，没办法搞
def curl_cmd_get(url):
    cmd = "curl '%s'" % (url)
    res = os.popen(cmd).read()
    return res


def get_money_flow(id):
    url = 'http://qt.gtimg.cn/q=ff_%s' % (id)
    refer = 'http://finance.qq.com/stock/sother/test_flow_stock_quotpage.htm'

    favicon = 'http://qt.gtimg.cn/favicon.ico'

    stocklist = []
    i = 0
    imax = 5
    while 1:
        try:
            if i + 1 < imax:
                index = random.randint(0, len(user_agent_list) -1)
                req_header = []
                req_header.extend(['Referer: %s' % (refer)])
                req_header.extend(['User-Agent: %s' % (user_agent_list[index])])
                if user_agent_dic.has_key(index):
                    key = '%d_%s' % (index, id)
                    req_header.extend(['If-None-Match: %s' % (user_agent_dic[key])])

                #httpGetContent(favicon, ['Referer: %s' % (url), 'User-Agent: %s' % (user_agent_list[index])])
                res = httpGetContent(url, req_header)
                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')

            else:
                res['head'] = ''
                res['body'] = curl_cmd_get(url)
                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            stocklist = value.split('~')
            if len(stocklist) < 10:
                print url
                time.sleep(1)
                continue
            break
        except Exception,e:
            #print url, value, e
            if res.has_key('head'):
                print res['head']

            i = i+1
            if i >= imax:
                log_write('errcode', id)
                break
            time.sleep(random.randint(1, 3))

    if i > imax:
        return  {}

    #print stocklist
    stockdict = {}
    if len(stocklist) > 10:
        stockdict['main_force'] = float(stocklist[3])
        stockdict['small_force'] = float(stocklist[7])

    if res.has_key('head') and 'Etag:' in res['head']:
        key = '%d_%s' % (index, id)
        user_agent_dic[key]  = res['head'].split('Etag:')[1].strip()

    return stockdict

def get_single_analysis(id, deal_dic):

    url = 'http://stock.finance.qq.com/sstock/list/view/dadan.php?t=js&c=%s&max=100&p=1&opt=1&o=0' % (id)
    refer = 'http://stockhtm.finance.qq.com/sstock/quotpage/dadan.htm?c=%s' % (id)

    stocklist = []
    i = 0
    imax = 5
    while 1:
        try:
            if i + 1 < imax:
                req_header = []
                req_header.extend(['Referer: %s' % (refer)])
                index = random.randint(0, len(user_agent_list) -1)
                req_header.extend(['User-Agent: %s' % (user_agent_list[index])])

                res = httpGetContent(url, req_header)
                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            else:
                res = curl_cmd_get(url)
                value = res.decode("gbk").split('=')[1].strip(';\r\n')
            stocklist = value.split(',')[1]
            break
        except Exception,e:
            #print url, value, e
            i = i+1
            if i >= imax:
                log_write('errcode', id)
                break
            time.sleep(random.randint(1, 3))

    if i > imax:
        return  {}

    if not deal_dic.has_key('vol_1'):
        deal_dic['vol_1'] = []
        deal_dic['vol_2'] = []
        deal_dic['vol_3'] = []
        deal_dic['vol_4'] = []

    #print stocklist
    stockdict = {}
    stockdict['vol_1'] = 0
    stockdict['vol_2'] = 0
    stockdict['vol_3'] = 0
    stockdict['vol_4'] = 0
    items = stocklist.split('^')
    for item in items:
        subitems = item.split('~')
        if len(subitems) >= 6:
            flag = 1
            if 's' in subitems[5] or 'S' in subitems[5]:
                flag = -1
            elif 'b' in subitems[5] or 'B' in subitems[5]:
                flag = 1

            if int(subitems[3]) >= 100:
                stockdict['vol_1'] += int(subitems[3]) * flag

            if int(subitems[3]) >= 200:
                stockdict['vol_2'] += int(subitems[3]) * flag

            if int(subitems[3]) >= 500:
                stockdict['vol_3'] += int(subitems[3]) * flag

            if int(subitems[3]) >= 1000:
                stockdict['vol_4'] += int(subitems[3]) * flag


    if len(deal_dic['vol_1']) >= 10:
        deal_dic['vol_1'].pop(0)
        deal_dic['vol_2'].pop(0)
        deal_dic['vol_3'].pop(0)
        deal_dic['vol_4'].pop(0)

    if len(deal_dic['vol_1']) >=1 and abs(deal_dic['vol_1'][-1] - stockdict['vol_1']) < 400:
        return stockdict

    deal_dic['vol_1'].append(stockdict['vol_1'])
    deal_dic['vol_2'].append(stockdict['vol_2'])
    deal_dic['vol_3'].append(stockdict['vol_3'])
    deal_dic['vol_4'].append(stockdict['vol_4'])

    return stockdict

id_dic = {}
def load_monitor_list():
    if not os.path.isfile('monitor_list'):
        return
    tmp_dic = {}
    file = open("monitor_list")
    while 1:
        line = file.readline().strip('\n')
        if not line:
            break
        tmp_dic[line] = line

    file.close()

    remove_list = []
    for key in id_dic:
        if not tmp_dic.has_key(key):
            remove_list.append(key)

    for key in remove_list:
        id_dic.pop(key)

    for key in tmp_dic:
        if not id_dic.has_key(key):
            id_dic[key] = {'id':key}
    return

def do_check_monitor():
    load_monitor_list()
    for key in id_dic:
        time.sleep(0.05)
        money = get_money_flow(key)

        if not id_dic[key].has_key('main_force'):
                id_dic[key]['main_force'] = []

        if not id_dic[key].has_key('small_force'):
                id_dic[key]['small_force'] = []

        if money.has_key('main_force'):
            if len(id_dic[key]['main_force']) and abs(id_dic[key]['main_force'][-1] - money['main_force']) >= 50:
                id_dic[key]['main_force'].append(money['main_force'])
            elif not len(id_dic[key]['main_force']):
                id_dic[key]['main_force'].append(money['main_force'])

        if money.has_key('small_force'):
            if len(id_dic[key]['small_force']) and abs(id_dic[key]['small_force'][-1] - money['small_force']) >= 50:
                id_dic[key]['small_force'].append(money['small_force'])
            elif not len(id_dic[key]['small_force']):
                id_dic[key]['small_force'].append(money['small_force'])

        if not id_dic[key].has_key('single'):
            id_dic[key]['single'] = {}

        get_single_analysis(key, id_dic[key]['single'])

        print 'monitor', id_dic[key]

if __name__ == '__main__':
    while 1:
        do_check_monitor()
        time.sleep(30)
