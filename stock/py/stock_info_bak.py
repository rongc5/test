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

def GzipStream(streams):
    "用于处理容启动gzip压缩"
    if streams:
        data = cStringIO.StringIO(streams)
        g = gzip.GzipFile('', 'rb', 9, data)
        return g.read()

#
#成交明细
def get_stockid_detail(id, date):
    url = 'http://market.finance.sina.com.cn/downxls.php?date=%s&symbol=%s' % (date, id)
    res = httpGetContent(url)
    file_object = open('%s_%s_detail' % (id, date), 'w')
    file_object.write(res)
    file_object.close( )

#查看每股财务指标
def get_stockid_mgzb(id):

    url = 'http://comdata.finance.gtimg.cn/data/mgzb/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?mgzb-%s' %(id)

    i = 0
    imax = 3
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

#偿债及资本结构
def get_stockid_czzb(id):

    url = 'http://comdata.finance.gtimg.cn/data/czzb/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?czzb-%s' %(id)

    i = 0
    imax = 3
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

    return id_dic['data']['czzb']


#杜邦分析
def get_stockid_dbfx(id):

    url = 'http://comdata.finance.gtimg.cn/data/dbfx/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/dbfx.html?%s' %(id)

    i = 0
    imax = 3
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

    return id_dic['data']['dbfx']


def get_single_analysis(id):

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

    #print stocklist
    stockdict = {}
    stockdict['s'] = 0
    stockdict['b'] = 0
    items = stocklist.split('^')
    for item in items:
        subitems = item.split('~')
        if len(subitems) >= 6:
            if 's' in subitems[5] or 'S' in subitems[5]:
                stockdict['s'] += int(subitems[3])
            elif 'b' in subitems[5] or 'B' in subitems[5]:
                stockdict['b'] += int(subitems[3])

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
    imax = 10
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
        return {}
    stockdict['end'] = float(stocklist[3])             # 当前收盘价格（可以是当前价格）
    stockdict['high'] = float(stocklist[33])           # 最高价格
    stockdict['low'] = float(stocklist[34])            # 最低价格
    #stockdict['buyvol'] = int(stocklist[7])             # 外盘
    #stockdict['sellvol'] = int(stocklist[8])           # 内盘

    #stockdict['range_price'] = float(stocklist[31])    # 涨跌价格
    stockdict['range_percent'] = float(stocklist[32])  # 涨跌比%

    #stockdict['volume'] = int(stocklist[6])            # 成交量（手）
    #stockdict['total_price'] = int(stocklist[37])      # 成交额（万元）
    #print 'change_rate', stocklist
    if stocklist[38].strip():
        stockdict['change_rate'] = float(stocklist[38]) # 换手率
    else:
        stockdict['change_rate'] = 0
    stockdict['pe'] = float(stocklist[39])          # 市盈率
    stockdict['swing'] = float(stocklist[43])           # 振幅

    stockdict['pb'] = float(stocklist[46])              # 股票市净率
    stockdict['date'] = stocklist[30][:8]               # 时间
    stockdict['block'] = False if stockdict['start'] else True #股票是否停牌
    stockdict['circulation_market_value'] = float(stocklist[44])
    stockdict['total_value'] = float(stocklist[45])
    #stockdict['high_limit'] = float(stocklist[47])
    #stockdict['low_limit'] = float(stocklist[48])
    if 'Etag:' in res['head']:
        key = '%d_%s' % (index, id)
        user_agent_dic[key]  = res['head'].split('Etag:')[1].strip()

    return stockdict

#获取所有股票列表
def get_stock_list():
    #url = 'http://www.shdjt.com/js/lib/astock.js'
    url = 'http://www.ctxalgo.com/api/stocks'
    req_header = ['Cookie:session=eyJfaWQiOnsiIGIiOiJNMkZtTldJME1qSXpNR1UzWWpka01UYzRabUV6TURKbU5qTTJZemcwWWpjPSJ9fQ.DMYGJg.ReVb3XLAerXR30CUrOoeh9kW5ug']
    #req_header = []
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(1)
        else:
            print res['head']
            break

    #print res
    id_dic= json.loads(res['body'])

    file = open('code_all', "w+")
    for key in id_dic:
        file.write(unicodedata.normalize('NFKD', key).encode('ascii','ignore'))
        file.write('\n')
        id_dic[key] = {}
        key = unicodedata.normalize('NFKD', key).encode('ascii','ignore')
        file.flush()

    file.close()
    return id_dic

#解禁列表, http 相应数据不全， chunked 的问题， 后边再处理吧
def get_outDxf_list(start, end):
    url = 'http://stock.finance.qq.com//sstock/list/view/dxf.php?c=0&b=%s&e=%s' % (start, end)
    refer = 'http://finance.qq.com/stock/dxfcx.htm?t=2&mn=%s&mx=%s' %(start, end)
    req_header = []
    req_header.extend(['Referer: %s' % (refer)])
    print url
    res = curl_cmd_get(url)
    #print res
    #value = res.split('=')[1].strip(';\n').decode("utf-8","ignore")
    value = res.decode("gbk").split('=')[1].strip(';\r\n')
    #print value
    id_dic= json.loads(value)

    return id_dic


#去掉停牌等
def get_basic_list(id_dic):
    if not len(id_dic):
        return

    file = open('base_list', "a+")

    basic_dic = {}
    flag = False
    for key in id_dic:
        time.sleep(0.01)
        res = get_stockid_real_time(key)
        if not res.has_key('pe'):
            log_write('err_base_list', id)
            continue

        if res['block']:  #停牌
            continue

        if res['circulation_market_value'] >= 300:
            continue

        if res['total_value'] >= 500:
            continue

        if res['pe'] > 99 or res['pe'] < 0:
            continue

        if res['end'] > 39 or res['end'] <= 2:
            continue

        res['mgzb'] = get_stockid_mgzb(key)
        if len(res['mgzb']) < 1:
            continue

        if '--' not in res['mgzb'][0]['tbmgsy'] and  float(res['mgzb'][0]['tbmgsy']) < 0.1:
            continue

        if '--' not in res['mgzb'][0]['mgxjll'] and float(res['mgzb'][0]['mgxjll']) < 0.05:
            continue

        res['cznl'] = get_stockid_cznl(key)
        if len(res['cznl']) < 1:
            continue
        #if float(res['cznl'][0]['yylr']) < 0:
        #    continue
        #
        #if float(res['cznl'][0]['jlr']) < 0:
        #    continue
        #
        #if float(res['cznl'][0]['yylr']) < 30:
        #    continue


        basic_dic[res['id']] = res

        file.write(res['code'])
        file.write('\t')
        file.write(res['id'])
        file.write('\t')
        file.write(str(res['end']))
        file.write('\t')
        file.write(str(res['pe']))
        file.write('\t')
        file.write(str(res['pb']))
        file.write('\t')
        file.write(str(res['circulation_market_value']))
        file.write('\t')
        file.write(str(res['total_value']))
        file.write('\t')
        file.write(res['mgzb'][0]['tbmgsy'])
        file.write('\t')
        file.write(res['mgzb'][0]['mgxjll'])
        file.write('\t')
        file.write(res['cznl'][0]['mgsy'])
        file.write('\t')
        file.write(res['cznl'][0]['mgxj'])
        file.write('\t')
        file.write(res['cznl'][0]['zysr'])
        file.write('\t')
        file.write(res['cznl'][0]['yylr'])
        file.write('\t')
        file.write(res['cznl'][0]['jlr'])
        file.write('\t')
        file.write(res['date'])

        file.write('\n')
        file.flush()

        #print res
        #break
    file.close()


def load_base_list():
    if not os.path.isfile('code_all'):
        get_stock_list()

    if not os.path.isfile('base_list'):
        id_list = []
        file = open("code_all")
        while 1:
            line = file.readline().strip()
            if not line:
                break
            id_list.append(line)
        file.close()
        get_basic_list(id_list)

    id_list = []
    if os.path.isfile('err_base_list'):
        file = open('err_base_list')
        while 1:
            line = file.readline().strip()
            if not line:
                break
            id_list.append(line)
        file.close()
        os.remove('err_base_list')
        get_basic_list(id_list)

    id_dic = {}
    file = open("base_list")
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')
        if len(items) < 8:
            continue
        tmp_dic = {}
        tmp_dic['code'] = items[0]
        tmp_dic['id'] = items[1]
        tmp_dic['end'] = items[2]
        tmp_dic['pe'] = float(items[3])
        tmp_dic['pb'] = float(items[4])
        tmp_dic['circulation_market_value'] = float(items[5])
        tmp_dic['total_value'] = float(items[6])
        tmp_dic['tbmgsy'] = items[7].replace(',', '')
        tmp_dic['mgxjll'] = items[8].replace(',', '')
        tmp_dic['mgsy'] = items[9].replace(',', '')
        tmp_dic['mgxj'] = items[10].replace(',', '')
        tmp_dic['zysr'] = items[11].replace(',', '')
        tmp_dic['yylr'] = items[12].replace(',', '')
        tmp_dic['jlr'] = items[13].replace(',', '')
        tmp_dic['date'] = items[14]

        id_dic[items[1]] = tmp_dic

    file.close()
    return id_dic

def base_select(id_dic):
    remove_ley = []
    for key in id_dic:
        #print id_dic[key]['zysr']
        if '--' not in id_dic[key]['zysr'] and float(id_dic[key]['zysr']) < 0:
            remove_ley.append(key)
            continue

        if '--' not in id_dic[key]['jlr'] and float(id_dic[key]['jlr']) < 0:
            remove_ley.append(key)
            continue

        if '--' not in id_dic[key]['mgsy'] and float(id_dic[key]['mgsy']) < 0:
            remove_ley.append(key)
            continue

        if '--' not in id_dic[key]['mgxj'] and float(id_dic[key]['mgxj'])< 0:
            remove_ley.append(key)
            continue

        if '--' not in id_dic[key]['tbmgsy'] and float(id_dic[key]['tbmgsy']) < 0.2:
            remove_ley.append(key)
            continue

        if float(id_dic[key]['end']) >39:
            remove_ley.append(key)
            continue

        #if id_dic[key]['code'] in ban_dic:
        #    remove_ley.append(key)
        #    continue

    for key in remove_ley:
        id_dic.pop(key)

    return id_dic

def remove_from_banlist(id_dic, ban_list):
    remove_ley = []

    for key in id_dic:
        if id_dic[key]['code'] in ban_list:
            remove_ley.append(key)
            continue

    for key in remove_ley:
        id_dic.pop(key)

    return id_dic

#增长 为true, 下降为false
def get_data_direction(arr):
    if len(arr) < 2 :
        if arr[0] > 0:
            return True
        else:
            return False

    #length = len(arr)
    #count = 0
    #sum = 0;
    #for i in range(length - 1):
    #    if arr[length -i -1] > arr[length -i -2]:
    #        count += 1
    #    sum += arr[length -i -1] - arr[length -i -2]

    if arr[-1] > arr[0] and arr[-1] > 0:
        return True
    else:
        return False

    #if count > 0.6 * (length -1):
    #    return True
    #
    #return False


def do_search_short():
    day = Day()
    start_day = '%s' % (day.get_day_of_day(-20))
    end_day = '%s' % (day.get_day_of_day(30))
    print start_day.replace('-', ''), end_day.replace('-', '')
    ban_list = get_outDxf_list(start_day.replace('-', ''), end_day.replace('-', ''))
    ban_dic = {}
    for key in ban_list:
        idstr = '%s' % key[0]
        ban_dic[idstr] = ''

    id_dic = load_base_list()
    print 'load_base_list', len(id_dic)
    id_dic = base_select(id_dic)
    print 'after base_select', len(id_dic)
    id_dic = remove_from_banlist(id_dic, ban_dic)
    print 'after ban_dic', len(id_dic)

    search_dic = {}
    remove_ley = []
    countx = 0
    TIME_DIFF = 10
    while 1:
        for key in remove_ley:
            id_dic.pop(key)

        countx += 1
        remove_ley = []
        print 'after remove_ley', len(id_dic)
        for key in id_dic:
            #time.sleep(0.05)

            if not id_dic[key].has_key('next_time'):
                id_dic[key]['next_time'] = 0

            if id_dic[key]['next_time']:
                diff_time = time.time() - id_dic[key]['next_time']
                if diff_time < 0:
                    continue

            flag_one = False
            res = get_stockid_real_time(key)
            if  res.has_key('range_percent'):
                #remove_ley.append(key)

                if res['range_percent'] < -0.5 or  res['range_percent'] > 0.3:
                    remove_ley.append(key)
                    continue

                id_dic[key]['range_percent'] = res['range_percent']

                #if res['swing'] < 2.0:
                #    continue

                id_dic[key]['swing'] = res['swing']

                if res['change_rate'] < 1:
                    continue

                id_dic[key]['change_rate'] = res['change_rate']
                if res['end'] < res['low']:
                    continue

                id_dic[key]['end'] = res['end']
                id_dic[key]['low'] = res['low']
                id_dic[key]['high'] = res['high']
                id_dic[key]['start'] = res['start']
                id_dic[key]['last_closing'] = res['last_closing']


            if id_dic[key].has_key('end') and id_dic[key].has_key('low') and id_dic[key].has_key('start'):
                if id_dic[key]['end'] > id_dic[key]['low'] and abs(id_dic[key]['end'] - id_dic[key]['low']) >= 2* abs(id_dic[key]['end'] - id_dic[key]['start']):
                    flag_one = True

            #if flag_one:
            #    print key, 'one'
            #if not res.has_key('range_percent'):
            #    print res, key

            money = get_money_flow(key)

            if not id_dic[key].has_key('small_force'):
                    id_dic[key]['small_force'] = []

            if money.has_key('small_force'):
                if len(id_dic[key]['small_force']) and abs(id_dic[key]['small_force'][-1] - money['small_force']) >= 50:
                    id_dic[key]['small_force'].append(money['small_force'])
                elif not len(id_dic[key]['small_force']):
                    id_dic[key]['small_force'].append(money['small_force'])

            if not id_dic[key].has_key('main_force'):
                    id_dic[key]['main_force'] = []

            if money.has_key('main_force'):
                if len(id_dic[key]['main_force']) and abs(id_dic[key]['main_force'][-1] - money['main_force']) >= 50:
                    id_dic[key]['main_force'].append(money['main_force'])
                elif not len(id_dic[key]['main_force']):
                    id_dic[key]['main_force'].append(money['main_force'])

            big_data = get_single_analysis(key)
            big_res = 0
            if len(big_data):
                big_res = big_data['b'] - big_data['s']

            if not id_dic[key].has_key('big_res'):
                id_dic[key]['big_res'] = []
            if len(id_dic[key]['big_res']) and abs(id_dic[key]['big_res'][-1] - big_res) >= 200:
                id_dic[key]['big_res'].append(big_res)
            elif not len(id_dic[key]['big_res']):
                id_dic[key]['big_res'].append(big_res)

            flag_two = False
            if len(id_dic[key]['big_res']) and get_data_direction(id_dic[key]['big_res']):
                flag_two = True

            #if len(id_dic[key]['big_res']) and id_dic[key]['big_res'][-1] < -20000:
                #remove_ley.append(key)
                #continue

            flag_three = False
            if len(id_dic[key]['main_force']) and get_data_direction(id_dic[key]['main_force']):
                flag_three = True

            if flag_one and flag_two:
                search_dic[key] = id_dic[key]
                id_dic[key]['next_time'] = 0
            elif flag_two and flag_three:
                search_dic[key] = id_dic[key]
                id_dic[key]['next_time'] = 0
            else:
                if not len(id_dic[key]['big_res']) or not len(id_dic[key]['main_force']):
                    id_dic[key]['next_time'] = 0
                elif len(id_dic[key]['big_res']) and len(id_dic[key]['big_res']) == 1:
                    if id_dic[key]['big_res'][0] > 0:
                        id_dic[key]['next_time'] = 0
                    elif id_dic[key]['big_res'][0] < 0:
                        id_dic[key]['next_time'] = time.time() + TIME_DIFF
                elif len(id_dic[key]['big_res']) and len(id_dic[key]['big_res']) > 1:
                    if id_dic[key]['big_res'][-1] > 0:
                        id_dic[key]['next_time'] = time.time() + TIME_DIFF
                    elif id_dic[key]['big_res'][-1] < 0:
                        if id_dic[key]['big_res'][len(id_dic[key]['big_res']) -2] > id_dic[key]['big_res'][-1]:
                            id_dic[key]['next_time'] = time.time() + 4 *TIME_DIFF
                        else:
                            id_dic[key]['next_time'] = time.time() + 2 *TIME_DIFF



        if len(search_dic):
            log_write('res_list', 'begin ==========:%s' % (day.datetime()))

        for key in search_dic:
            log_write('res_list', json.dumps(search_dic[key]))
            #print 'search res', search_dic[key]
        if len(search_dic):
            log_write('res_list', 'serch over ==========')
        time.sleep(TIME_DIFF)

#A股就是个坑， 技术指标低位了， 仍然可以再砸
#技术指标高位了， 有资金接盘仍然可以涨, 高位始终是危险
#压力如铁桶，支撑如窗户纸, 压力位不放量买就是要跌的节奏
#有连续的大单介入才介入， 低位大资金都不介入， 肯定有猫腻
#业绩好的，下跌， 大资金不介入， 肯定有什么利空， 业绩可以变脸
#买二--买五是大单， 而买1是小单， 下跌也不买， 明显还没有跌到位
#托单， 托而不买， 还有下跌
#业绩增长的， 业绩出来之前已经开始涨了， 出业绩的那天直接出货下跌
#利好消息也是一样， 这叫利好出尽是利空
#不是说长下引线就能买， 高位， 主力先出货， 再用少量资金拉起来吸引
#高位下引线， 股价快到顶了
#跟封盘， 毕竟高位跟风盘不少， 再出货
#macd 鸭子张嘴， 会加速下跌
#涨是需要理由的， 跌不需要
if __name__ == '__main__':
    do_search_short()
