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
import subprocess
import ast

from time import strftime, localtime
from datetime import timedelta, date
import calendar

import ConfigParser

__author__ = 'rong'

user_agent_list = []
user_agent_dic = {}
user_agent_cookie = {}
MAX_RESPONSE_KB = 10*1024
DATAPATH = '../data/'

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

def get_week_day(date_str, format_str='%Y-%m-%d'):
    day = 0
    if not date_str:
        return day

    try:
        d = time.strptime(date_str, format_str)
        day =d.tm_wday + 1
    except BaseException, e:
        print e.message
        return day

    return day

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

class HTTPError(Exception):
    """Exception that is wrapped around all exceptions that are raised
    by the underlying fetcher when using the ExceptionWrappingFetcher

    @ivar why: The exception that caused this exception
    """
    def __init__(self, why=None):
        Exception.__init__(self, why)
        self.why = why

class CurlHTTPFetcher(object):
    """
    An C{L{HTTPFetcher}} that uses pycurl for fetching.
    See U{http://pycurl.sourceforge.net/}.
    """
    ALLOWED_TIME = 30 # seconds

    def _parseHeaders(self, header_file):
        header_file.seek(0)

        # Remove the status line from the beginning of the input
        unused_http_status_line = header_file.readline().lower ()
        if unused_http_status_line.startswith('http/1.1 100 '):
            unused_http_status_line = header_file.readline()
            unused_http_status_line = header_file.readline()

        lines = [line.strip() for line in header_file]

        # and the blank line from the end
        empty_line = lines.pop()
        if empty_line:
            raise HTTPError("No blank line at end of headers: %r" % (line,))

        headers = {}
        for line in lines:
            try:
                name, value = line.split(':', 1)
            except ValueError:
                raise HTTPError(
                    "Malformed HTTP header line in response: %r" % (line,))

            value = value.strip()

            # HTTP headers are case-insensitive
            name = name.lower()
            headers[name] = value

        return headers

    def fetch(self, url, body=None, headers=None):
        stop = int(time.time()) + self.ALLOWED_TIME
        off = self.ALLOWED_TIME

        if headers is None:
            headers = {}

        if  'User-Agent:' in headers or  'user-agent:' in headers:
            print 'no User-Agent', headers, url, sys._getframe().f_lineno
            return

        header_list = []
        if headers is not None:
            for header_name, header_value in headers.iteritems():
                header_list.append('%s: %s' % (header_name, header_value))

        c = pycurl.Curl()
        try:
            c.setopt(pycurl.NOSIGNAL, 1)

            if header_list:
                c.setopt(pycurl.HTTPHEADER, header_list)

            # Presence of a body indicates that we should do a POST
            if body is not None:
                c.setopt(pycurl.POST, 1)
                c.setopt(pycurl.POSTFIELDS, body)

            while off > 0:
                if not url.startswith('http://') and not url.startswith('https://'):
                    raise HTTPError("Fetching URL not allowed: %r" % (url,))

                data = cStringIO.StringIO()
                def write_data(chunk):
                    if data.tell() > 1024*MAX_RESPONSE_KB:
                        return 0
                    else:
                        #print 'hello', data.getvalue().decode('gbk')
                        return data.write(chunk)

                response_header_data = cStringIO.StringIO()
                c.setopt(pycurl.WRITEFUNCTION, write_data)
                c.setopt(pycurl.HEADERFUNCTION, response_header_data.write)
                c.setopt(pycurl.TIMEOUT, off)
                c.setopt(pycurl.URL, url)

                c.perform()

                response_headers = self._parseHeaders(response_header_data)
                code = c.getinfo(pycurl.RESPONSE_CODE)
                if code in [301, 302, 303, 307]:
                    url = response_headers.get('location')
                    if url is None:
                        raise HTTPError(
                            'Redirect (%s) returned without a location' % code)

                    # Redirects are always GETs
                    c.setopt(pycurl.POST, 0)

                    # There is no way to reset POSTFIELDS to empty and
                    # reuse the connection, but we only use it once.
                else:
                    res = {}
                    res['head'] = response_headers
                    res['status'] = code
                    res['final_url'] = url
                    res['body'] = '%s' % (data.getvalue())
                    return res

                off = stop - int(time.time())

            raise HTTPError("Timed out fetching: %r" % (url,))
        finally:
            response_header_data.close()
            data.close()
            c.close()


def get_single_analysis2(date, id, sum_vol, deal_dic):
    #url = 'http://market.finance.sina.com.cn/downxls.php?date=%s&symbol=%s' % (date, id)

    url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/cn_bill_download.php?' \
          'symbol=%s&num=600&page=1&sort=ticktime&asc=0&volume=40000&amount=0&type=0&day=%s' % (id, date)

    #print url

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]

    cookie_key = '%d_sina' % (index,)
    if user_agent_cookie.has_key(cookie_key):
        header['Cookie'] = user_agent_cookie[cookie_key]

    curl =  CurlHTTPFetcher()
    curl.ALLOWED_TIME = 2

    res = {}
    try:
        res = curl.fetch(url, None, header)
    except BaseException, e:
        print e.message

    if res.has_key('head') and res['head'].has_key('set-cookie'):
        user_agent_cookie[cookie_key] = res['head']['set-cookie'].split(';')[0]


    stockdict = {}
    stockdict['vol_1'] = 0
    stockdict['vol_2'] = 0
    stockdict['vol_3'] = 0

    res_str = ''
    if res.has_key('body') and res['body'].strip():
        items = res['body'].split('\n')
        #print res['body']
        for key in items:
            subitems = key.split(',');
            if len(subitems) == 7:
                if not subitems[4].isdigit():
                    continue
                vol = int(subitems[4])

                flag = 1
                if u'买盘' in  subitems[6].decode('gbk'):
                    flag = 1
                elif u'卖盘' in subitems[6].decode('gbk'):
                    flag = -1
                else:
                    continue

                vol = vol /100

                if vol >= 400:
                    stockdict['vol_1'] += vol * flag

                if vol >= 1000:
                    stockdict['vol_2'] += vol * flag

                if vol >= 2000:
                    stockdict['vol_3'] += vol * flag

        if not deal_dic.has_key('vol_1'):
            deal_dic['vol_1'] = []
            deal_dic['vol_2'] = []
            deal_dic['vol_3'] = []
            deal_dic['ratio_vol_1'] = []
            deal_dic['ratio_vol_2'] = []
            deal_dic['ratio_vol_3'] = []

        if len(deal_dic['vol_1']) >= 10:
            deal_dic['vol_1'].pop(0)
            deal_dic['vol_2'].pop(0)
            deal_dic['vol_3'].pop(0)
            deal_dic['ratio_vol_1'].pop(0)
            deal_dic['ratio_vol_2'].pop(0)
            deal_dic['ratio_vol_3'].pop(0)

        if len(deal_dic['vol_1']) >=1 and abs(deal_dic['vol_1'][-1] - stockdict['vol_1']) < 400:
            return stockdict

        print stockdict, id
        deal_dic['vol_1'].append(stockdict['vol_1'])
        deal_dic['vol_2'].append(stockdict['vol_2'])
        deal_dic['vol_3'].append(stockdict['vol_3'])

        deal_dic['ratio_vol_1'].append(round(stockdict['vol_1'] *1.0 / sum_vol, 4))
        deal_dic['ratio_vol_2'].append(round(stockdict['vol_2'] *1.0 / sum_vol, 4))
        deal_dic['ratio_vol_3'].append(round(stockdict['vol_3'] *1.0 / sum_vol, 4))

    return stockdict

    #print stockdict
    #if not res_str.strip():
    #    print url

def get_single_analysis3(id, vol, deal_dic):

    stockdict = {}
    if int(vol) <= 0:
        print vol, 'err'
        return stockdict

    url = 'http://stock.gtimg.cn/data/index.php?appn=dadan&action=summary&c=%s' % (id)

    curl =  CurlHTTPFetcher()
    curl.ALLOWED_TIME = 2
    stocklist = ''
    i = 0
    imax = 1
    while 1:
        try:
            if i + 1 <= imax:
                header = {}
                index = random.randint(0, len(user_agent_list) -1)
                header['User-Agent'] = user_agent_list[index]
                res = {}
                try:
                    res = curl.fetch(url, None, header)
                except BaseException, e:
                    pass
                    #print '111111, err', e.message, id

                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            #print value
            stocklist = value
            break
        except Exception,e:
            #print url, value, e
            stocklist = ''
            i = i+1
            if i >= imax:
                log_write('errcode', id)
                break

    if i > imax:
        return  {}

    if not stocklist.strip():
        return {}

    #print stocklist


    #items = stocklist.split('^')
    try:
        #print type(stocklist)
        items = ast.literal_eval(stocklist)
        #print '111111'

        #>=300
        stockdict['vol_1'] = int(float(items[2 + 3][4])) - int(float(items[2 + 2][5]))
        #>=1000
        stockdict['vol_2'] = int(float(items[2 + 7][4])) - int(float(items[2 + 7][5]))
        #>=2000
        stockdict['vol_3'] = int(float(items[2 + 9][4])) - int(float(items[2 + 9][5]))

        #print stockdict, id

    except Exception,e:
        print e
        return {}

    if not deal_dic.has_key('vol_1'):
        deal_dic['vol_1'] = []
        deal_dic['vol_2'] = []
        deal_dic['vol_3'] = []
        deal_dic['ratio_vol_1'] = []
        deal_dic['ratio_vol_2'] = []
        deal_dic['ratio_vol_3'] = []

    if len(deal_dic['vol_1']) >= 10:
        deal_dic['vol_1'].pop(0)
        deal_dic['vol_2'].pop(0)
        deal_dic['vol_3'].pop(0)
        deal_dic['ratio_vol_1'].pop(0)
        deal_dic['ratio_vol_2'].pop(0)
        deal_dic['ratio_vol_3'].pop(0)

    if len(deal_dic['vol_1']) >=1 and abs(deal_dic['vol_1'][-1] - stockdict['vol_1']) < 400:
        return stockdict

    deal_dic['vol_1'].append(stockdict['vol_1'])
    deal_dic['vol_2'].append(stockdict['vol_2'])
    deal_dic['vol_3'].append(stockdict['vol_3'])

    deal_dic['ratio_vol_1'].append(round(stockdict['vol_1'] *1.0 / vol, 4))
    deal_dic['ratio_vol_2'].append(round(stockdict['vol_2'] *1.0 / vol, 4))
    deal_dic['ratio_vol_3'].append(round(stockdict['vol_3'] *1.0 / vol, 4))

    return stockdict


def get_money_flow(id):
    url = 'http://qt.gtimg.cn/q=ff_%s' % (id)
    refer = 'http://finance.qq.com/stock/sother/test_flow_stock_quotpage.htm'

    favicon = 'http://qt.gtimg.cn/favicon.ico'

    curl =  CurlHTTPFetcher()
    curl.ALLOWED_TIME = 3
    random.seed(int(time.time()))
    stocklist = []
    i = 0
    imax = 1
    while 1:
        try:
            if i + 1 <= imax:

                header = {}
                index = random.randint(0, len(user_agent_list) -1)
                header['User-Agent'] = user_agent_list[index]
                header['Referer'] = refer
                res = {}
                if user_agent_dic.has_key(index):
                    key = '%d_%s' % (index, id)
                    header['If-None-Match']= (user_agent_dic[key])

                try:
                    res = curl.fetch(url, None, header)
                except BaseException, e:
                    print 'money', e.message, id

                #httpGetContent(favicon, ['Referer: %s' % (url), 'User-Agent: %s' % (user_agent_list[index])])
                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            stocklist = value.split('~')
            if len(stocklist) < 8:
                #print url, res['head'], value
                if 'must-revalidate' in res['head']:
                    break
                #time.sleep(random.randint(1, 3))
                continue
            break
        except Exception,e:
            #print url, value, e
            if res.has_key('head'):
                #print res['head']
                if 'must-revalidate' in res['head']:
                    break

            i = i+1
            if i >= imax:
                log_write('errcode', id)
                break
            #time.sleep(random.randint(1, 3))

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
    curl =  CurlHTTPFetcher()
    curl.ALLOWED_TIME = 2

    res = {}
    deal_dic = {}

    while 1:
        header = {}
        index = random.randint(0, len(user_agent_list) -1)
        header['User-Agent'] = user_agent_list[index]
        header['Referer'] = refer
        #if user_agent_dic.has_key(index):
        #    req_header.extend(['If-None-Match: %s' % (user_agent_dic[index])])
        i = i + 1
        try:
            res = curl.fetch(url, None, header)
        except BaseException, e:
            print e.message

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
        return stockdict
    stockdict['end'] = float(stocklist[3])             # 当前收盘价格（可以是当前价格）
    stockdict['high'] = float(stocklist[33])           # 最高价格
    stockdict['low'] = float(stocklist[34])            # 最低价格
    #stockdict['buyvol'] = int(stocklist[7])             # 外盘
    #stockdict['sellvol'] = int(stocklist[8])           # 内盘

    #stockdict['range_price'] = float(stocklist[31])    # 涨跌价格
    stockdict['range_percent'] = float(stocklist[32])  # 涨跌比%

    stockdict['vol'] = int(stocklist[36])            # 成交量（手）
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

    #print stockdict
    return stockdict

#价跌主力增仓个股
def get_force_in(id_dic):
    url = 'http://stock.gtimg.cn/data/view/flow.php?t=7&d=1'
    refer = 'http://finance.qq.com/zjlx/zjlx_modules/price.htm?t=down&change_color_0'

    curl =  CurlHTTPFetcher()

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    header['Referer'] = refer
    res = {}
    try:
        res = curl.fetch(url, None, header)
        value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
        items = value.split('^')
        for item in items:
            subitems = item.split('~')
            id = subitems[0].strip('\'')
            if not id_dic.has_key(id):
                id_dic[id] = {}
                id_dic[id]['id'] = id

            id_dic[id]['force'] = float(subitems[3])
            id_dic[id]['tag'] = 'in'

    except BaseException, e:
        print e.message

    return id_dic

#主力增仓个股
def get_force_add(id_dic):
    url = 'http://stock.gtimg.cn/data/view/zldx.php?t=1'
    refer = 'http://finance.qq.com/zjlx/zjlx_modules/flow_stock.htm?t=zl_in&change_color_0'

    curl =  CurlHTTPFetcher()

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    header['Referer'] = refer
    res = {}
    try:
        res = curl.fetch(url, None, header)
        value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
        items = value.split('^')
        for item in items:
            subitems = item.split('~')
            id = subitems[0].strip('\'')
            if not id_dic.has_key(id):
                id_dic[id] = {}
                id_dic[id]['id'] = id

            id_dic[id]['force'] = float(subitems[3])
            id_dic[id]['tag'] = 'add'

    except BaseException, e:
        print e.message

    return id_dic


#主力增仓个股
def get_force_add5(id_dic):
    url = 'http://stock.gtimg.cn/data/view/zldx.php?t=2'
    refer = 'http://finance.qq.com/zjlx/zjlx_modules/flow_stock.htm?t=zl_in&change_color_0'

    curl =  CurlHTTPFetcher()

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    header['Referer'] = refer
    res = {}
    try:
        res = curl.fetch(url, None, header)
        value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
        value = value.split(';')[0]
        items = value.split('^')
        for item in items:
            subitems = item.split('~')
            id = subitems[0].strip('\'')
            if not id_dic.has_key(id):
                id_dic[id] = {}
                id_dic[id]['id'] = id

            id_dic[id]['force5'] = float(subitems[1])
            id_dic[id]['tag'] = 'add5'

    except BaseException, e:
        print e.message

    return id_dic


#主力资金放量
def get_force_vol(id_dic):
    url = 'http://stock.gtimg.cn/data/view/flow.php?t=9&d=1'
    refer = 'http://finance.qq.com/zjlx/zjlx_modules/zlfl.htm?change_color_0'

    curl =  CurlHTTPFetcher()

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    header['Referer'] = refer
    res = {}
    try:
        res = curl.fetch(url, None, header)
        value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
        items = value.split('^')
        for item in items:
            subitems = item.split('~')
            id = subitems[0].strip('\'')
            if not id_dic.has_key(id):
                id_dic[id] = {}
                id_dic[id]['id'] = id

            id_dic[id]['vol'] = float(subitems[2])
            id_dic[id]['tag'] = 'vol'

    except BaseException, e:
        print e.message

    return id_dic

def load_force_list():
    day = Day()
    lastday = 0
    while 1:
        toady =  '%s' % (day.get_day_of_day(lastday), )
        if get_week_day(toady) > 5:
            lastday = lastday - 1
        else:
            break

    file_name = '%s_%s' % ('last_force', toady.replace('-', ''))

    id_dic = {}

    get_force_in(id_dic)
    get_force_add(id_dic)
    get_force_add5(id_dic)
    get_force_vol(id_dic)

    for key in id_dic:
        cmd = ['grep', key, file_name]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if res_str.strip():
            continue

        if not id_dic[key].has_key('force'):
            id_dic[key]['force'] = 0

        if not id_dic[key].has_key('force5'):
            id_dic[key]['force5'] = 0

        if not id_dic[key].has_key('vol'):
            id_dic[key]['vol'] = 0

        str = '%s\t%s\t%s\t%s\t%s' % (key, id_dic[key]['force'], id_dic[key]['force5'], id_dic[key]['vol'], id_dic[key]['tag'])
        log_write(file_name, str)

def load_base_list(days_num):
    day = Day()
    toady = '%s' % (day.today(),)

    date_dic = {}
    id_dic = {}
    for id_day in range(1, days_num + 1):
        date = ''
        lastday = id_day * -1
        while 1:
            date =  '%s' % (day.get_day_of_day(lastday), )
            #print lastday, date
            if get_week_day(date) > 5:
                lastday = lastday - 2
            elif lastday in date_dic:
                lastday = lastday - 1;
            else:
                date_dic[lastday] = date
                break

        file_name = '%s_%s' % ('last_force', date.replace('-', ''))
        #print file_name
        if not os.path.isfile(file_name):
            continue

        file = open(file_name)
        #print file_name
        while 1:
            line = file.readline().strip()
            if not line:
                break
            #print line
            items = line.split('\t')
            id_dic[items[0]] = {}
            id_dic[items[0]]['id'] = items[0]
            id_dic[items[0]]['last_single'] = {}

    return id_dic


#增长 为true, 下降为false
def get_data_direction(arr):
    if len(arr) < 2 :
        return False

    if arr[-1] > arr[0]:
        return True
    else:
        return False


def get_positive_ratio(arr):
    length = len(arr)
    if length <= 1:
        return 0
    count = 0
    sum = 0;
    for i in range(length - 1):
        if arr[length -i -1] > arr[length -i -2]:
            count += 1
        sum += 1

    return count *1.0/sum

def log_print_res(search_dic):
    if not len(search_dic):
        return

    day = Day()

    log_write('d_list', 'begin ==========:%s' % (day.datetime()))

    if search_dic.has_key('date'):
        search_dic.pop('date')
    if search_dic.has_key('last_closing'):
        search_dic.pop('last_closing')
    if search_dic.has_key('swing'):
        search_dic.pop('swing')
    if search_dic.has_key('next_time'):
        search_dic.pop('next_time')
    if search_dic.has_key('code'):
        search_dic.pop('code')

    log_write('d_list', json.dumps(search_dic))
    log_write('d_list', '\n')
    log_write('d_list', 'serch over ==========')


def load_ua_list():
    id_dic = {}
    if not os.path.isfile('ua_list'):
        return id_dic

    file = open("ua_list")
    while 1:
        line = file.readline().strip('\n')
        if not line:
            break
        user_agent_list.append(line)

    file.close()
    print 'user_agent_list:', len(user_agent_list)
    return user_agent_list


def do_check_remove(id_dic, sec, cf):
    if not len(id_dic):
        return False

    if 0 == cf.getint(sec, 'is_open'):
        return False

    if cf.has_option(sec, 'zysr_ge') and id_dic.has_key('zysr') and '--' not in id_dic['zysr'] and float(id_dic['zysr']) < cf.getfloat(sec, 'zysr_ge'):
        return True

    if cf.has_option(sec, 'jlr_ge') and id_dic.has_key('jlr') and '--' not in id_dic['jlr'] and float(id_dic['jlr']) < cf.getfloat(sec, 'jlr_ge'):
        return True

    if cf.has_option(sec, 'mgsy_ge') and id_dic.has_key('mgsy') and '--' not in id_dic['mgsy'] and float(id_dic['mgsy']) < cf.getfloat(sec, 'mgsy_ge'):
        return True

    if cf.has_option(sec, 'mgxj_ge') and id_dic.has_key('mgxj') and '--' not in id_dic['mgxj'] and float(id_dic['mgxj'])< cf.getfloat(sec, 'mgxj_ge'):
        return True

    if cf.has_option(sec, 'tbmgsy_ge') and id_dic.has_key('tbmgsy') and  '--' not in id_dic['tbmgsy'] and float(id_dic['tbmgsy']) < cf.getfloat(sec, 'tbmgsy_ge'):
        return True

    if cf.has_option(sec, 'mgxjll_ge') and id_dic.has_key('mgxjll') and '--' not in id_dic['mgxjll'] and float(id_dic['mgxjll']) < cf.getfloat(sec, 'mgxjll_ge'):
        return True

    if cf.has_option(sec, 'change_rate_ge') and id_dic.has_key('change_rate') and float(id_dic['change_rate']) < cf.getfloat(sec, 'change_rate_ge'):
        return True

    if cf.has_option(sec, 'range_percent_ge') and id_dic.has_key('range_percent') and float(id_dic['range_percent']) < cf.getfloat(sec, 'range_percent_ge'):
        return True

    if cf.has_option(sec, 'range_percent_le') and id_dic.has_key('range_percent') and float(id_dic['range_percent']) > cf.getfloat(sec, 'range_percent_le'):
        return True

    return False

def do_check_select(id_dic, sec, cf):
    if not len(id_dic):
        return

    if 0 == cf.getint(sec, 'is_open'):
        return

    #print id_dic
    if cf.has_option(sec, 'zysr_ge') and id_dic.has_key('zysr') and '--' not in id_dic['zysr'] and float(id_dic['zysr']) < cf.getfloat(sec, 'zysr_ge'):
        return

    if cf.has_option(sec, 'jlr_ge') and id_dic.has_key('jlr') and '--' not in id_dic['jlr'] and float(id_dic['jlr']) < cf.getfloat(sec, 'jlr_ge'):
        return

    if cf.has_option(sec, 'mgsy_ge') and id_dic.has_key('mgsy') and '--' not in id_dic['mgsy'] and float(id_dic['mgsy']) < cf.getfloat(sec, 'mgsy_ge'):
        return

    if cf.has_option(sec, 'mgxj_ge') and id_dic.has_key('mgxj') and '--' not in id_dic['mgxj'] and float(id_dic['mgxj'])< cf.getfloat(sec, 'mgxj_ge'):
        return

    if cf.has_option(sec, 'tbmgsy_ge') and id_dic.has_key('tbmgsy') and  '--' not in id_dic['tbmgsy'] and float(id_dic['tbmgsy']) < cf.getfloat(sec, 'tbmgsy_ge'):
        return

    if cf.has_option(sec, 'mgxjll_ge') and id_dic.has_key('mgxjll') and '--' not in id_dic['mgxjll'] and float(id_dic['mgxjll']) < cf.getfloat(sec, 'mgxjll_ge'):
        return

    if cf.has_option(sec, 'end_le') and float(id_dic['end']) > cf.getfloat(sec, 'end_le'):
        return

    if cf.has_option(sec, 'pe_le') and id_dic.has_key('pe') and float(id_dic['pe']) > cf.getfloat(sec, 'pe_le'):
        return

    if cf.has_option(sec,'end_ge_start') and float(id_dic['end']) < float(id_dic['start']):
        return

    if cf.has_option(sec, 'change_rate_ge') and float(id_dic['change_rate']) < cf.getfloat(sec, 'change_rate_ge'):
        return

    if cf.has_option(sec, 'range_percent_ge') and float(id_dic['range_percent']) < cf.getfloat(sec, 'range_percent_ge'):
        return

    if cf.has_option(sec, 'range_percent_le') and float(id_dic['range_percent']) > cf.getfloat(sec, 'range_percent_le'):
        return

    if cf.has_option(sec,'end_ge_low') and float(id_dic['end']) < float(id_dic['low']):
        return

    if cf.has_option(sec,'down_pointer_ge_up_pointer') and float(id_dic['down_pointer']) < float(id_dic['up_pointer']):
        return

    if cf.has_option(sec,'vol_1_asc') and not get_data_direction(id_dic['single']['vol_1']):
        return

    if cf.has_option(sec,'vol_2_asc') and not get_data_direction(id_dic['single']['vol_2']):
        return

    if cf.has_option(sec,'vol_3_asc') and not get_data_direction(id_dic['single']['vol_3']):
        return

    if  cf.has_option(sec, 'vol_1_ge') and id_dic['single']['vol_1'][-1] < cf.getint(sec, 'vol_1_ge'):
        return

    if  cf.has_option(sec, 'vol_2_ge') and id_dic['single']['vol_2'][-1] < cf.getint(sec, 'vol_2_ge'):
        return

    if  cf.has_option(sec, 'vol_3_ge') and id_dic['single']['vol_3'][-1] < cf.getint(sec, 'vol_3_ge'):
        return

    if  cf.has_option(sec, 'ratio_vol_1_ge') and id_dic['single']['ratio_vol_1'][-1] < cf.getfloat(sec, 'ratio_vol_1_ge'):
        return

    if  cf.has_option(sec, 'ratio_vol_2_ge') and id_dic['single']['ratio_vol_2'][-1] < cf.getfloat(sec, 'ratio_vol_2_ge'):
        return

    if  cf.has_option(sec, 'ratio_vol_3_ge') and id_dic['single']['ratio_vol_3'][-1] < cf.getfloat(sec, 'ratio_vol_3_ge'):
        return

    if  cf.has_option(sec, 'ratio_vol_4_ge') and id_dic['single']['ratio_vol_4'][-1] < cf.getfloat(sec, 'ratio_vol_4_ge'):
        return

    if  cf.has_option(sec, 'down_pointer_ge') and id_dic['down_pointer'] < cf.getfloat(sec, 'down_pointer_ge'):
        return

    if  cf.has_option(sec, 'up_pointer_le') and id_dic['up_pointer'] > cf.getfloat(sec, 'up_pointer_le'):
        return

    lastday_num = 0
    if cf.has_option(sec, 'lastday_num'):
        lastday_num = cf.getint(sec, 'lastday_num')
    else:
        lastday_num = 3

    lastday_has_vol_1_ge = False
    lastday_has_vol_2_ge = False
    lastday_has_vol_3_ge = False

    lastday_has_range_percent_ge = False

    lastday_range_percent_ge_ratio=0
    lastday_range_percent_ge_sum = 0
    lastday_has_down_pointer_ge = False
    for id_day in range(lastday_num):
        if cf.has_option(sec, 'lastday_has_vol_1_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('vol_1') and len(id_dic['last_single']['vol_1']) > id_day:
            if id_dic['last_single']['vol_1'][id_day] > cf.getint(sec, 'lastday_has_vol_1_ge'):
                    lastday_has_vol_1_ge = True

        if cf.has_option(sec, 'lastday_has_vol_2_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('vol_2') and len(id_dic['last_single']['vol_2']) > id_day:
            if id_dic['last_single']['vol_2'][id_day] > cf.getint(sec, 'lastday_has_vol_2_ge'):
                    lastday_has_vol_2_ge = True

        if cf.has_option(sec, 'lastday_has_vol_3_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('vol_3') and len(id_dic['last_single']['vol_3']) > id_day:
            if id_dic['last_single']['vol_3'][id_day] > cf.getint(sec, 'lastday_has_vol_3_ge'):
                    lastday_has_vol_3_ge = True

        if cf.has_option(sec, 'lastday_has_range_percent_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('range_percent') and len(id_dic['last_single']['range_percent']) > id_day:
            if id_dic['last_single']['range_percent'][id_day] > cf.getfloat(sec, 'lastday_has_range_percent_ge'):
                    lastday_has_range_percent_ge = True

        if cf.has_option(sec, 'lastday_range_percent_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('range_percent') and len(id_dic['last_single']['range_percent']) > id_day:
            if id_dic['last_single']['range_percent'][id_day] > cf.getfloat(sec, 'lastday_range_percent_ge'):
                    lastday_range_percent_ge_sum += 1

        if cf.has_option(sec, 'lastday_has_down_pointer_ge') and id_dic.has_key('last_single') \
            and id_dic['last_single'].has_key('end') and len(id_dic['last_single']['end']) > id_day:
            down_pointer = round(abs(id_dic['last_single']['end'][id_day] - id_dic['last_single']['low'][id_day]) * 1.0 /abs(id_dic['last_single']['end'][id_day] - id_dic['last_single']['start'][id_day]), 2)
            if down_pointer > cf.float(sec, 'lastday_has_down_pointer_ge'):
                    lastday_has_down_pointer_ge = True

    if cf.has_option(sec, 'lastday_has_vol_1_ge') and id_dic['last_single'].has_key('vol_1') and not lastday_has_vol_1_ge:
        return

    if cf.has_option(sec, 'lastday_has_vol_2_ge') and id_dic['last_single'].has_key('vol_2') and not lastday_has_vol_2_ge:
        return

    if cf.has_option(sec, 'lastday_has_vol_3_ge') and id_dic['last_single'].has_key('vol_3') and not lastday_has_vol_3_ge:
        return

    if cf.has_option(sec, 'lastday_has_range_percent_ge') and id_dic['last_single'].has_key('range_percent') and not lastday_has_range_percent_ge:
        return

    if cf.has_option(sec, 'lastday_range_percent_ge_ratio') and id_dic['last_single'].has_key('range_percent') and cf.has_option(sec, 'lastday_range_percent_ge'):
        lastday_range_percent_ge_ratio = round(lastday_range_percent_ge_sum * 1.0/lastday_num, 2)
        if lastday_range_percent_ge_ratio < cf.getfloat(sec, 'lastday_range_percent_ge_ratio'):
            return

    if cf.has_option(sec, 'lastday_has_down_pointer_ge') and id_dic['last_single'].has_key('end') and not lastday_has_down_pointer_ge:
        return

    print id_dic
    id_dic['sec'] = sec
    log_print_res(id_dic)


def do_search_short():

    load_ua_list()

    day = Day()

    toady = '%s' % (day.today(),)

    #print day.hour
    id_dic = load_base_list(5)

    remove_ley = []
    monitor_mtime = 0
    cfg_mtime = 0
    TIME_DIFF = 20
    cf = ConfigParser.ConfigParser()
    while 1:
        #if int(day.hour) >= 15:
        load_force_list()
        id_dic = load_base_list(5)

        print 'after base_select', len(id_dic)

        for key in remove_ley:
            id_dic.pop(key)

        remove_ley = []
        print 'after remove_ley', len(id_dic)
        for key in id_dic:

            mtime = time.ctime(os.path.getmtime('d.ini'))
            if cfg_mtime != mtime:
                cfg_mtime = mtime
                cf.read('d.ini')

            if do_check_remove(id_dic[key], 'remove', cf):
                remove_ley.append(key)
                continue

            time.sleep(0.01)
            res = get_stockid_real_time(key)

            if res.has_key('block') and res['block']:  #停牌
                print key, 'block'
                remove_ley.append(key)
                continue

            if  res.has_key('range_percent'):
                id_dic[key]['range_percent'] = res['range_percent']
                id_dic[key]['swing'] = res['swing']
                id_dic[key]['change_rate'] = res['change_rate']

                id_dic[key]['end'] = res['end']
                id_dic[key]['low'] = res['low']
                id_dic[key]['high'] = res['high']
                id_dic[key]['start'] = res['start']
                id_dic[key]['last_closing'] = res['last_closing']
                id_dic[key]['vol'] = res['vol']

            if not id_dic[key].has_key('up_pointer'):
                id_dic[key]['up_pointer'] = 0.0

            if not id_dic[key].has_key('down_pointer'):
                id_dic[key]['down_pointer'] = 0.0

            if id_dic[key].has_key('end') and id_dic[key].has_key('low') and id_dic[key].has_key('start'):
                if id_dic[key]['end'] > id_dic[key]['low'] and id_dic[key]['end'] != id_dic[key]['start']:
                    id_dic[key]['down_pointer'] = round(abs(id_dic[key]['end'] - id_dic[key]['low']) * 1.0 /abs(id_dic[key]['end'] - id_dic[key]['start']), 2)

                if id_dic[key]['end'] < id_dic[key]['high'] and id_dic[key]['end'] != id_dic[key]['start']:
                    id_dic[key]['up_pointer'] = round(abs(id_dic[key]['end'] - id_dic[key]['high']) *1.0/abs(id_dic[key]['end'] - id_dic[key]['start']), 2)

            if not id_dic[key].has_key('vol'):
                continue

            if not id_dic[key].has_key('single'):
                id_dic[key]['single'] = {}

            if len(get_single_analysis3(key, id_dic[key]['vol'], id_dic[key]['single'])) == 0:
                #get_single_analysis2(toady, key, id_dic[key]['vol'], id_dic[key]['single'])
                if len(id_dic[key]['single']) == 0:
                    continue
            #print id_dic[key]['single']

            for sec in cf.sections():
                if 'remove' in sec:
                    if do_check_remove(id_dic[key], sec, cf):
                        remove_ley.append(key)
                        break
                else:
                    do_check_select(id_dic[key], sec, cf)

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
#macd 鸭子张嘴， 会加速下跌, 买之前一定要看一下15分钟macd、DMA
#涨是需要理由的， 跌不需要,配股的股就不要进了， 号称散户的周扒皮
#次新和业绩差的能不碰还是不要碰了, 选股还是要选强的
#没有买盘的拉升都是骗人的
#割肉要坚决， 没有什么后悔的, 不看上证、a50 那是不行的
#不要做T, 不看好就跑， 看好就买， 做T, 买了， 想跑跑不了
if __name__ == '__main__':
    do_search_short()
