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

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()


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


    res = {}
    try:
        curl =  CurlHTTPFetcher()
        curl.ALLOWED_TIME = 2
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

def load_details(days_num, deal_dic):
    if len(deal_dic) == 0:
        return

    day = Day()

    path = '%s/' %(DATAPATH)
    cmd = ['mkdir', '-p', path]
    subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

    for id_day in range(1, days_num + 1):
        date = ''
        lastday = id_day * -1
        while 1:
            date =  '%s' % (day.get_day_of_day(lastday), )
            if get_week_day(date) > 5:
                lastday = lastday - 2
            else:
                break

        file_name = '%s/%s_%s' % (DATAPATH, 'last_single', date.replace('-', ''))

        for key in deal_dic:
            cmd = ['grep', key, file_name]
            res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
            if not os.path.isfile(file_name) or not res_str.strip():
                index = random.randint(1, 5)
                time.sleep(index)
                #load_stockid_detail(date, key, file_name)


def get_details(days_num, deal_dic):
    if len(deal_dic) == 0:
        return

    for key in deal_dic:
        deal_dic[key]['last_single'] = {}

    day = Day()

    for id_day in range(1, days_num + 1):
        date = ''
        lastday = id_day * -1
        while 1:
            date =  '%s' % (day.get_day_of_day(lastday), )
            if get_week_day(date) > 5:
                lastday = lastday - 2
            else:
                break

        file_name = '%s/%s_%s' % (DATAPATH, 'last_single', date.replace('-', ''))
        if not os.path.isfile(file_name):
            continue

        get_stockid_detail(file_name, deal_dic, 'last_single')

    return deal_dic
        #print deal_dic

#成交明细
def get_stockid_detail(file_name, deal_dic, detail_key):

    if not os.path.isfile(file_name):
        return

    file = open(file_name)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\n')
        for key in items:
            subitems = key.split('\t');
            if len(subitems) == 10:
                if subitems[0] in deal_dic:
                    if not deal_dic[subitems[0]][detail_key].has_key('vol_1'):
                        deal_dic[subitems[0]][detail_key]['vol_1'] = []
                        deal_dic[subitems[0]][detail_key]['vol_2'] = []
                        deal_dic[subitems[0]][detail_key]['vol_3'] = []
                        deal_dic[subitems[0]][detail_key]['vol_4'] = []
                        deal_dic[subitems[0]][detail_key]['vol_5'] = []
                        deal_dic[subitems[0]][detail_key]['vol_6'] = []
                        deal_dic[subitems[0]][detail_key]['min_price'] = []
                        deal_dic[subitems[0]][detail_key]['high_price'] = []
                        deal_dic[subitems[0]][detail_key]['total_vol'] = []

                    deal_dic[subitems[0]][detail_key]['vol_1'].append(int(subitems[1]))
                    deal_dic[subitems[0]][detail_key]['vol_2'].append(int(subitems[2]))
                    deal_dic[subitems[0]][detail_key]['vol_3'].append(int(subitems[3]))
                    deal_dic[subitems[0]][detail_key]['vol_4'].append(int(subitems[4]))
                    deal_dic[subitems[0]][detail_key]['vol_5'].append(int(subitems[5]))
                    deal_dic[subitems[0]][detail_key]['vol_6'].append(int(subitems[6]))
                    deal_dic[subitems[0]][detail_key]['total_vol'].append(int(subitems[7]))
                    deal_dic[subitems[0]][detail_key]['min_price'].append(float(subitems[8]))
                    deal_dic[subitems[0]][detail_key]['high_price'].append(float(subitems[9]))

    file.close()
    #print deal_dic
    return

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


def get_single_analysis(id, vol, deal_dic):

    stockdict = {}
    if int(vol) <= 0:
        print vol, 'err'
        return stockdict

    url = 'http://stock.finance.qq.com/sstock/list/view/dadan.php?t=js&c=%s&max=600&p=1&opt=4&o=0' % (id)
    refer = 'http://stockhtm.finance.qq.com/sstock/quotpage/dadan.htm?c=%s' % (id)

    stocklist = ''
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
                try:
                    curl =  CurlHTTPFetcher()
                    curl.ALLOWED_TIME = 2
                    res = curl.fetch(url, None, header)
                except BaseException, e:
                    pass
                    #print '111111, err', e.message, id

                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            else:
                res = curl_cmd_get(url)
                value = res.decode("gbk").split('=')[1].strip(';\r\n')
            #print value
            stocklist = value.split(',')[1]
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

    if not deal_dic.has_key('vol_1'):
        deal_dic['vol_1'] = []
        deal_dic['vol_2'] = []
        deal_dic['vol_3'] = []
        deal_dic['ratio_vol_1'] = []
        deal_dic['ratio_vol_2'] = []
        deal_dic['ratio_vol_3'] = []

    stockdict['vol_1'] = 0
    stockdict['vol_2'] = 0
    stockdict['vol_3'] = 0
    items = stocklist.split('^')
    for item in items:
        subitems = item.split('~')
        if len(subitems) >= 6:
            flag = 1
            if 's' in subitems[5] or 'S' in subitems[5]:
                flag = -1
            elif 'b' in subitems[5] or 'B' in subitems[5]:
                flag = 1
            else:
                continue

            if int(subitems[3]) >= 400:
                stockdict['vol_1'] += int(subitems[3]) * flag

            if int(subitems[3]) >= 1000:
                stockdict['vol_2'] += int(subitems[3]) * flag

            if int(subitems[3]) >= 2000:
                stockdict['vol_3'] += int(subitems[3]) * flag

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


def get_single_analysis3(id, vol, deal_dic):

    stockdict = {}
    if int(vol) <= 0:
        print vol, 'err'
        return stockdict

    url = 'http://stock.gtimg.cn/data/index.php?appn=dadan&action=summary&c=%s' % (id)

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
                    curl =  CurlHTTPFetcher()
                    curl.ALLOWED_TIME = 2
                    res = curl.fetch(url, None, header)
                except BaseException, e:
                    pass
                    #print '111111, err', e.message, id

                value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
            else:
                res = curl_cmd_get(url)
                value = res.decode("gbk").split('=')[1].strip(';\r\n')
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
                    curl =  CurlHTTPFetcher()
                    curl.ALLOWED_TIME = 3
                    res = curl.fetch(url, None, header)
                except BaseException, e:
                    print 'money', e.message, id

                #httpGetContent(favicon, ['Referer: %s' % (url), 'User-Agent: %s' % (user_agent_list[index])])
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
    imax = 2
    stocklist = []
    stockdict = {}

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
            curl =  CurlHTTPFetcher()
            curl.ALLOWED_TIME = 2
            res = curl.fetch(url, None, header)
        except BaseException, e:
            print e.message

        if len(res) < 2:
            print url, sys._getframe().f_lineno, res
            if i > imax:
                break;
            #time.sleep(1)
        else:
            value = res['body'].split('=')[1].strip(';\n')
            stocklist = value.split('~')
            if len(stocklist) < 45:
                print url, sys._getframe().f_lineno, res['body'], stocklist
                print value
                if i > imax:
                    break;
                #time.sleep(1)
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

#获取所有股票列表
def get_stock_list():
    #url = 'http://www.shdjt.com/js/lib/astock.js'
    url = 'http://www.ctxalgo.com/api/stocks'
    curl =  CurlHTTPFetcher()
    while 1:
        header = {}
        index = random.randint(0, len(user_agent_list) -1)
        header['User-Agent'] = 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36'
        header['Cookie'] = 'session=eyJfaWQiOnsiIGIiOiJNMkZtTldJME1qSXpNR1UzWWpka01UYzRabUV6TURKbU5qTTJZemcwWWpjPSJ9fQ.DQvdBA.x1XmlgjogTBgu2R52U1y9mHxqXI'
        res = {}
        try:
            res = curl.fetch(url, None, header)
        except BaseException, e:
            print e.message
       #res = {}
       #res['head'] = ''
       #res['body'] = curl_cmd_get(url)
        if len(res) < 2:
            print url
            #time.sleep(1)
        else:
            print res['head']
            break

    #print res
    #res = {}
    #res['head'] = ''
    #res['body'] = curl_cmd_get(url)
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

    curl =  CurlHTTPFetcher()

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    header['Referer'] = refer
    res = {}
    try:
        res = curl.fetch(url, None, header)
    except BaseException, e:
        print e.message

    #print res
    #value = res.split('=')[1].strip(';\n').decode("utf-8","ignore")
    value = res['body'].decode("gbk").split('=')[1].strip(';\r\n')
    #print value
    id_dic= json.loads(value)

    if os.path.isfile('out_dxf_list'):
        os.remove('out_dxf_list')
    for key in id_dic:
        str = '%s %s' % (key[0], key[2])
        log_write('out_dxf_list', str)

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

        if res.has_key('block') and res['block']:  #停牌
            continue

        if not res.has_key('pe'):
            log_write('err_base_list', key)
            continue

        #if res['circulation_market_value'] >= 390:
        #    continue
        #
        #if res['total_value'] >= 690:
        #    continue

        if res['pe'] > 99 or res['pe'] < 0:
            continue

        if res['end'] > 69 or res['end'] <= 3:
            continue

        res['mgzb'] = get_stockid_mgzb(key)
        if len(res['mgzb']) < 1:
            log_write('err_base_list', key)
            continue

        if '--' not in res['mgzb'][0]['tbmgsy'] and  float(res['mgzb'][0]['tbmgsy']) < 0.2:
            continue

        #if '--' not in res['mgzb'][0]['mgxjll'] and float(res['mgzb'][0]['mgxjll']) < 0.05:
            #continue

        res['cznl'] = get_stockid_cznl(key)
        if len(res['cznl']) < 1:
            log_write('err_base_list', key)
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

def log_single_analysis(id_dic):
    day = Day()
    toady = '%s' % (day.today(),)

    file_name = '%s_%s' % ('last_single', toady.replace('-', ''))
    for key in id_dic:
        cmd = ['grep', key, file_name]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if os.path.isfile(file_name) and  res_str.strip():
            continue

        #time.sleep(0.05)
        res = get_stockid_real_time(key)

        if res.has_key('block') and res['block']:  #停牌
            print key, 'block'
            continue

        if  res.has_key('range_percent'):
            id_dic[key]['range_percent'] = res['range_percent']
            id_dic[key]['swing'] = res['swing']
            id_dic[key]['change_rate'] = res['change_rate']

            id_dic[key]['end'] = res['end']
            id_dic[key]['low'] = res['low']
            id_dic[key]['high'] = res['high']
            id_dic[key]['start'] = res['start']
            id_dic[key]['vol'] = res['vol']

        if not id_dic[key].has_key('vol'):
            continue

        if not id_dic[key].has_key('single'):
            id_dic[key]['single'] = {}

        if len(get_single_analysis3(key, id_dic[key]['vol'], id_dic[key]['single'])) == 0:
            #get_single_analysis2(toady, key, id_dic[key]['vol'], id_dic[key]['single'])
            if len(id_dic[key]['single']) == 0:
                continue

        money = get_money_flow(key)
        if len(money) > 0 and money.has_key('main_force'):
            id_dic[key]['main_force'] = money['main_force']
        else:
            continue


        if id_dic[key]['vol'] > 0:
            res_str = '%s\t%d\t%d\t%d\t%d\t%.2f\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f' % (key, id_dic[key]['single']['vol_1'][-1], id_dic[key]['single']['vol_2'][-1], id_dic[key]['single']['vol_3'][-1], \
            0, id_dic[key]['main_force'], id_dic[key]['vol'], id_dic[key]['start'], id_dic[key]['end'], id_dic[key]['low'], \
            id_dic[key]['high'], id_dic[key]['range_percent'])
            log_write(file_name, res_str)


def get_last_singles(days_num, deal_dic):
    if len(deal_dic) == 0:
        return {}
    day = Day()

    date_dic = {}

    for key in deal_dic:
        deal_dic[key]['last_single'] = {}

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

        file_name = '%s_%s' % ('last_single', date.replace('-', ''))
        if not os.path.isfile(file_name):
            continue

        file = open(file_name)
        #print file_name
        while 1:
            line = file.readline().strip()
            if not line:
                break
            items = line.split('\n')
            for key in items:
                subitems = key.split('\t');
                if not subitems[0] in deal_dic:
                    continue

                if len(subitems) > 10:
                    if subitems[0] in deal_dic:


                        if not deal_dic[subitems[0]]['last_single'].has_key('vol_1'):
                            deal_dic[subitems[0]]['last_single']['vol_1'] = []
                            deal_dic[subitems[0]]['last_single']['vol_2'] = []
                            deal_dic[subitems[0]]['last_single']['vol_3'] = []
                            deal_dic[subitems[0]]['last_single']['main_force'] = []
                            deal_dic[subitems[0]]['last_single']['start'] = []
                            deal_dic[subitems[0]]['last_single']['end'] = []
                            deal_dic[subitems[0]]['last_single']['low'] = []
                            deal_dic[subitems[0]]['last_single']['high'] = []
                            deal_dic[subitems[0]]['last_single']['total_vol'] = []
                            deal_dic[subitems[0]]['last_single']['range_percent'] = []

                        deal_dic[subitems[0]]['last_single']['vol_1'].append(int(subitems[1]))
                        deal_dic[subitems[0]]['last_single']['vol_2'].append(int(subitems[2]))
                        deal_dic[subitems[0]]['last_single']['vol_3'].append(int(subitems[3]))
                        deal_dic[subitems[0]]['last_single']['main_force'].append(float(subitems[5]))
                        deal_dic[subitems[0]]['last_single']['total_vol'].append(int(subitems[6]))
                        deal_dic[subitems[0]]['last_single']['low'].append(float(subitems[9]))
                        deal_dic[subitems[0]]['last_single']['high'].append(float(subitems[10]))
                        deal_dic[subitems[0]]['last_single']['start'].append(float(subitems[7]))
                        deal_dic[subitems[0]]['last_single']['end'].append(float(subitems[8]))

                if len(subitems) > 11:
                    deal_dic[subitems[0]]['last_single']['range_percent'].append(float(subitems[11]))

        file.close()


    return deal_dic

def log_print_res(search_dic):
    if not len(search_dic):
        return

    day = Day()

    log_write('res_list', 'begin ==========:%s' % (day.datetime()))

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

    log_write('res_list', json.dumps(search_dic))
    log_write('res_list', '\n')
    log_write('res_list', 'serch over ==========')


def load_monitor_list():
    id_dic = {}
    if not os.path.isfile('monitor_list'):
        return id_dic

    file = open("monitor_list")
    while 1:
        line = file.readline().strip('\n')
        if not line:
            break
        id_dic[line] = {}

    file.close()
    return id_dic

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
    start_day = '%s' % (day.get_day_of_day(-20))
    end_day = '%s' % (day.get_day_of_day(45))
    toady = '%s' % (day.today(),)
    print start_day.replace('-', ''), end_day.replace('-', '')
    ban_list = get_outDxf_list(start_day.replace('-', ''), end_day.replace('-', ''))
    ban_dic = {}
    for key in ban_list:
        idstr = '%s' % key[0]
        ban_dic[idstr] = {}

    #print day.hour
    base_dic = load_base_list()
    print 'load_base_list', len(base_dic)
    base_dic = remove_from_banlist(base_dic, ban_dic)
    print 'after ban_dic', len(base_dic)

    id_dic = base_dic
    remove_ley = []
    monitor_mtime = 0
    cfg_mtime = 0
    TIME_DIFF = 20
    cf = ConfigParser.ConfigParser()
    while 1:
        mtime = time.ctime(os.path.getmtime('monitor_list'))
        if monitor_mtime != mtime:
            monitor_mtime = mtime
            monitor_dic = load_monitor_list()

            for key in monitor_dic:
                if key not in id_dic:
                    id_dic[key] = {}
                    id_dic[key]['id'] = key

            get_last_singles(5, id_dic)

        if int(day.hour) >= 15:
            #print day.hour
            log_single_analysis(base_dic)
            log_single_analysis(monitor_dic)

        print 'after base_select', len(id_dic)

        for key in remove_ley:
            id_dic.pop(key)

        remove_ley = []
        print 'after remove_ley', len(id_dic)
        for key in id_dic:

            mtime = time.ctime(os.path.getmtime('stock.ini'))
            if cfg_mtime != mtime:
                cfg_mtime = mtime
                cf.read('stock.ini')

            if do_check_remove(id_dic[key], 'remove', cf):
                remove_ley.append(key)
                continue

            #time.sleep(0.01)
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
