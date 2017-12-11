#!/usr/bin/env python
#encoding=utf-8


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

__author__ = 'rong'

user_agent_list = [
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 "
        "(KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 "
        "(KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 "
        "(KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 "
        "(KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 "
        "(KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 "
        "(KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 "
        "(KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3"]


user_agent_dic = {}
user_agent_cookie = {}
MAX_RESPONSE_KB = 10*1024

class Day():
    def __init__(self):
        self.year = strftime("%Y", localtime())
        self.mon = strftime("%m", localtime())
        self.day = strftime("%d", localtime())
        self.hour = strftime("%H", localtime())
        self.min = strftime("%M", localtime())
        self.sec = strftime("%S", localtime())

    def today(self):
     '''''
     get today,date format="YYYY-MM-DD"
     '''''
     return date.today()


    def todaystr(self):
     '''
     get date string, date format="YYYYMMDD"
     '''
     return self.year + self.mon + self.day


    def datetime(self):
     '''''
     get datetime,format="YYYY-MM-DD HH:MM:SS"
     '''
     return strftime("%Y-%m-%d %H:%M:%S", localtime())


    def datetimestr(self):
     '''''
     get datetime string
     date format="YYYYMMDDHHMMSS"
     '''
     return self.year + self.mon + self.day + self.hour + self.min + self.sec


    def get_day_of_day(self, n=0):
     '''''
     if n>=0,date is larger than today
     if n<0,date is less than today
     date format = "YYYY-MM-DD"
     '''
     if (n < 0):
      n = abs(n)
      return date.today() - timedelta(days=n)
     else:
      return date.today() + timedelta(days=n)


    def get_days_of_month(self, year, mon):
     '''''
     get days of month
     '''
     return calendar.monthrange(year, mon)[1]


    def get_firstday_of_month(self, year, mon):
     '''''
     get the first day of month
     date format = "YYYY-MM-DD"
     '''
     days = "01"
     if (int(mon) < 10):
      mon = "0" + str(int(mon))
     arr = (year, mon, days)
     return "-".join("%s" % i for i in arr)


    def get_lastday_of_month(self, year, mon):
     '''''
     get the last day of month
     date format = "YYYY-MM-DD"
     '''
     days = calendar.monthrange(year, mon)[1]
     mon = self.addzero(mon)
     arr = (year, mon, days)
     return "-".join("%s" % i for i in arr)


    def get_firstday_month(self, n=0):
     '''''
     get the first day of month from today
     n is how many months
     '''
     (y, m, d) = self.getyearandmonth(n)
     d = "01"
     arr = (y, m, d)
     return "-".join("%s" % i for i in arr)


    def get_lastday_month(self, n=0):
     '''''
     get the last day of month from today
     n is how many months
     '''
     return "-".join("%s" % i for i in self.getyearandmonth(n))


    def getyearandmonth(self, n=0):
     '''''
     get the year,month,days from today
     befor or after n months
     '''
     thisyear = int(self.year)
     thismon = int(self.mon)
     totalmon = thismon + n
     if (n >= 0):
      if (totalmon <= 12):
       days = str(self.get_days_of_month(thisyear, totalmon))
       totalmon = self.addzero(totalmon)
       return (self.year, totalmon, days)
      else:
       i = totalmon / 12
       j = totalmon % 12
       if (j == 0):
        i -= 1
        j = 12
       thisyear += i
       days = str(self.get_days_of_month(thisyear, j))
       j = self.addzero(j)
       return (str(thisyear), str(j), days)
     else:
      if ((totalmon > 0) and (totalmon < 12)):
       days = str(self.get_days_of_month(thisyear, totalmon))
       totalmon = self.addzero(totalmon)
       return (self.year, totalmon, days)
      else:
       i = totalmon / 12
       j = totalmon % 12
       if (j == 0):
        i -= 1
        j = 12
       thisyear += i
       days = str(self.get_days_of_month(thisyear, j))
       j = self.addzero(j)
       return (str(thisyear), str(j), days)


    def addzero(self, n):
     '''''
     add 0 before 0-9
     return 01-09
     '''
     nabs = abs(int(n))
     if (nabs < 10):
      return "0" + str(nabs)
     else:
      return nabs


    def get_today_month(self,n=0):
     '''''
     获取当前日期前后N月的日期
     if n>0, 获取当前日期前N月的日期
     if n<0, 获取当前日期后N月的日期
     date format = "YYYY-MM-DD"
     '''
     (y, m, d) = self.getyearandmonth(n)
     arr = (y, m, d)
     if (int(self.day) < int(d)):
      arr = (y, m, self.day)
     return "-".join("%s" % i for i in arr)

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

#1.0 版本不必支持chunked,
def httpGetContent(url, req_header=None, version = '1.1'):

    #print "==>", req_header, url, "<=="

    buf = cStringIO.StringIO()
    response_header = cStringIO.StringIO()
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEFUNCTION, buf.write)
    c.setopt(c.CONNECTTIMEOUT, 100)
    c.setopt(c.TIMEOUT, 300)
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


def GzipStream(streams):
    "用于处理容启动gzip压缩"
    if streams:
        data = cStringIO.StringIO(streams)
        g = gzip.GzipFile('', 'rb', 9, data)
        return g.read()

#查看每股财务指标
def get_stockid_mgzb(id):

    url = 'http://comdata.finance.gtimg.cn/data/mgzb/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?mgzb-%s' %(id)

    i = 0
    imax = 3
    id_dic = {}
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

            id_dic= json.loads(value)
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

    return id_dic['data']['mgzb']

#查看每股盈利能力
def get_stockid_ylnl(id):

    url = 'http://comdata.finance.gtimg.cn/data/ylnl/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?ylnl-%s' %(id)

    i = 0
    imax = 3
    id_dic = {}
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

            id_dic= json.loads(value)
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

    return id_dic['data']['ylnl']

#查看每股成长能力
def get_stockid_cznl(id):

    url = 'http://comdata.finance.gtimg.cn/data/cznl/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?cznl-%s' %(id)

    i = 0
    imax = 3
    id_dic = {}
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

            id_dic= json.loads(value)
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

    return id_dic['data']['cznl']

def get_single_analysis(id, vol):
    stockdict = {}
    if int(vol) <= 0:
        print vol, 'err'
        return stockdict


    url = 'http://stock.finance.qq.com/sstock/list/view/dadan.php?t=js&c=%s&max=400&p=1&opt=1&o=0' % (id)
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

    #print stocklist

    stockdict['vol_1'] = 0
    stockdict['vol_2'] = 0
    stockdict['vol_3'] = 0
    stockdict['vol_4'] = 0
    items = stocklist.split('^')
    for item in items:
        subitems = item.split('~')
        if len(subitems) >= 6:
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

    stockdict['ratio_vol_1'] = stockdict['vol_1'] *1.0 / vol
    stockdict['ratio_vol_2'] = stockdict['vol_2'] *1.0 / vol
    stockdict['ratio_vol_3'] = stockdict['vol_3'] *1.0 / vol
    stockdict['ratio_vol_4'] = stockdict['vol_4'] *1.0 / vol

    return stockdict


def get_money_flow2(id):
    url = 'https://gupiao.baidu.com/api/stocks/stockfunds?from=pc&os_ver=1&cuid=xxx&vv=100&format=json&stock_code=%s' % (id)
    favicon = 'https://gupiao.baidu.com/favicon.ico'

    tmp_dic = {}
    i = 0
    imax = 5
    flag =0
    index = 0
    while 1:
        try:
            if i + 1 < imax:
                req_header = []
                index = random.randint(0, len(user_agent_list) -1)
                req_header.extend(['User-Agent: %s' % (user_agent_list[index])])
                if user_agent_cookie.has_key(index):
                    req_header.extend(['Cookie: %s' % (user_agent_cookie[index])])
                    flag = 1

                #httpGetContent(favicon, ['Referer: %s' % (url), 'User-Agent: %s' % (user_agent_list[index])])
                res = httpGetContent(url, req_header)
                value = res.decode("gbk").strip(';\r\n')

            else:
                res['head'] = ''
                res['body'] = curl_cmd_get(url)
                value = res.decode("gbk").strip(';\r\n')
            tmp_dic = json.loads(value)
            if len(tmp_dic) >0:
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
    #stockdict = {}
    #if tmp_dic.has_key(''):
    #    stockdict['main_force'] = float(stocklist[3])
    #    stockdict['small_force'] = float(stocklist[7])

    if res.has_key('head') and 'Set-Cookie:' in res['head']:
        user_agent_cookie[index]  = res['head'].split('Set-Cookie:')[1].split(';')[0].strip()

    if not flag:
        req_header = []
        req_header.extend(['Referer: %s' % (url)])
        req_header.extend(['User-Agent: %s' % (user_agent_list[index])])
        if user_agent_cookie.has_key(index):
                res = req_header.extend(['Cookie: %s' % (user_agent_cookie[index])])
        res = httpGetContent(favicon, req_header)
        if res.has_key('head') and 'Set-Cookie:' in res['head']:
            user_agent_cookie[index]  = res['head'].split('Set-Cookie:')[1].split(';')[0].strip()


def get_money_flow(id):
    url = 'http://qt.gtimg.cn/q=ff_%s' % (id)
    refer = 'http://finance.qq.com/stock/sother/test_flow_stock_quotpage.htm'

    favicon = 'http://qt.gtimg.cn/favicon.ico'

    stocklist = []
    i = 0
    imax = 3
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
            if len(stocklist) < 8:
                #print url, res['head'], value
                if 'must-revalidate' in res['head']:
                    break
                time.sleep(1)
                continue
            break
        except Exception,e:
            #print url, value, e
            if res.has_key('head'):
                print res['head']
                if 'must-revalidate' in res['head']:
                    break

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

#实时行情
def get_stockid_real_time(id):
    #url = 'http://qt.gtimg.cn/q=%s' % (id)
    #favicon = 'http://qt.gtimg.cn/favicon.ico
    url = 'http://web.sqt.gtimg.cn/q=%s' % (id)
    refer = 'http://gu.qq.com/%s/gp' % (id)
    #url = 'http://sqt.gtimg.cn/utf8/q=%s' % (id)
    #favicon = 'http://sqt.gtimg.cn/favicon.ico'
    i = 0
    imax = 3
    stocklist = []
    stockdict = {}
    while 1:
        req_header = []
        index = random.randint(0, len(user_agent_list) -1)
        req_header.extend(['User-Agent: %s' % (user_agent_list[index])])
        req_header.extend(['Referer: %s' % (refer)])
        #if user_agent_dic.has_key(index):
        #    req_header.extend(['If-None-Match: %s' % (user_agent_dic[index])])
        i = i + 1
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url, sys._getframe().f_lineno, res
            if i > imax:
                break;
            time.sleep(1)
        else:
            value = res['body'].split('=')[1].strip(';\n')
            stocklist = value.split('~')
            if len(stocklist) < 45:
                print url, sys._getframe().f_lineno, res['body'], stocklist
                print value
                if i > imax:
                    break;
                time.sleep(1)
                continue
            break
    #不知道是不是反爬虫， 先请求吧
    #httpGetContent(favicon, 'Referer: %s' % (url))
    #print stocklist

    if len(stocklist) < 45:
        return stockdict

    stockdict['id'] = id
    stockdict['code'] = stocklist[2]                           # 股票代码
    #stockdict['name'] = unicode(stocklist[1], 'gbk')  # 股票名称
    stockdict['last_closing'] = float(stocklist[4])    # 昨日收盘价格
    stockdict['start'] = float(stocklist[5])           # 开盘价格
    if stockdict['start'] <= 1:
        stockdict['block'] = True
        r