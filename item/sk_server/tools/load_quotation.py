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
import datetime
import calendar

__author__ = 'rong'

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

def init_stockid(id):
    stockdict = {}

    stockdict['id'] = id
    stockdict['code'] = id[2:]                           # 股票代码
    stockdict['last_closing'] = 0    # 昨日收盘价格
    stockdict['start'] = 0           # 开盘价格
    stockdict['block'] = False
    stockdict['end'] = 0             # 当前收盘价格（可以是当前价格）
    stockdict['high'] = 0           # 最高价格
    stockdict['low'] = 0           # 最低价格
    stockdict['buyvol'] = 0            # 外盘
    stockdict['sellvol'] = 0           # 内盘

    stockdict['range_price'] = 0    # 涨跌价格
    stockdict['range_percent'] = 0  # 涨跌比%

    stockdict['vol'] = 0            # 成交量（手）
    stockdict['total_price'] = 0      # 成交额（万元）

    stockdict['change_rate'] = 0
    stockdict['pe'] = 0         # 市盈率
    stockdict['swing'] = 0          # 振幅

    stockdict['pb'] = 0              # 股票市净率
    stockdict['date'] = ''               # 时间
    stockdict['circulation_market_value'] = 0
    stockdict['total_value'] = 0
    #stockdict['high_limit'] = float(stocklist[47])
    #stockdict['low_limit'] = float(stocklist[48])
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
    stockdict = init_stockid(id)

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
    stockdict['buyvol'] = int(stocklist[7])             # 外盘
    stockdict['sellvol'] = int(stocklist[8])           # 内盘

    #stockdict['range_price'] = float(stocklist[31])    # 涨跌价格
    stockdict['range_percent'] = float(stocklist[32])  # 涨跌比%

    stockdict['vol'] = int(stocklist[36])            # 成交量（手）
    stockdict['total_price'] = int(stocklist[37])      # 成交额（万元）
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

def load_ua_list():
    id_dic = {}
    if not os.path.isfile(UA_PATH):
        return id_dic

    file = open(UA_PATH)
    while 1:
        line = file.readline().strip('\n')
        if not line:
            break
        user_agent_list.append(line)

    file.close()
    print 'user_agent_list:', len(user_agent_list)
    return user_agent_list


def is_tradeday(query_date):
    weekday = datetime.datetime.strptime(query_date, '%Y%m%d').isoweekday()
    if weekday <= 5 and query_date not in holiday_dict:
        return 1
    else:
        return 0


def load_days(days_num):
    day = Day()
    day_list = []

    #date = day.today()
    #date = '%s' % (date)
    #if int(day.hour) >= 16 and is_tradeday(date.replace('-', '')):
            #day_list.append(date)
            #if days_num > 0:
                #days_num = days_num - 1


    lastday = 0
    for id_day in range(days_num):
        date = ''
        lastday = lastday - 1
        while 1:
            date =  '%s' % (day.get_day_of_day(lastday), )
            if is_tradeday(date.replace('-', '')):
                day_list.append(date)
                break
            lastday = lastday - 1

    return  day_list


def load_quotation(deal_dic):
    if len(deal_dic) == 0:
        return


    day_list = []

    day = Day()
    date = day.todaystr()

    if not is_tradeday(date):
        day_list = load_days(1)
        date = day_list[0]
    elif int(day.hour) < 15:
        return


    path = '%s' % (DATAPATH)
    cmd = ['mkdir', '-p', path]
    subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    tmp_name = '%s/quotation_%s' % (path, date)

    for key in deal_dic:
        flag = False

        cmd = ['grep', key, tmp_name]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(tmp_name) or not res_str.strip():
            flag = True

        if not flag:
            continue

        time.sleep(0.02)
        res = get_stockid_real_time(key)

        res_str = str(res['id'])
        res_str += '\t'

        res_str += str(res['start'])
        res_str += '\t'

        res_str += str(res['end'])
        res_str += '\t'

        res_str += str(res['high'])
        res_str += '\t'

        res_str += str(res['low'])
        res_str += '\t'

        res_str += str(res['last_closing'])
        res_str += '\t'

        res_str += str(res['vol'])
        res_str += '\t'

        res_str += str(res['buyvol'])
        res_str += '\t'

        res_str += str(res['sellvol'])
        res_str += '\t'

        res_str += str(res['swing'])
        res_str += '\t'

        res_str += str(res['change_rate'])
        res_str += '\t'

        res_str += str(res['range_percent'])
        res_str += '\t'

        res_str += str(res['total_price'])
        res_str += '\t'

        if res['block']:
            res_str += str(1)
        else:
            res_str += str(0)

        log_write(tmp_name, res_str)
        time.sleep(0.01)


def load_coad_all():
    if not os.path.isfile(CODE_PATH):
        return

    id_list = []
    file = open(CODE_PATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')
        id_all[items[0]] = items[1]
    file.close()

def load_holiday_dict():
    if not os.path.isfile(HOLIDAY_DICT_PATH):
        return

    file = open(HOLIDAY_DICT_PATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')
        if len(items) >= 2:
            holiday_dict[items[0]] = items[1]
    file.close()

user_agent_list = []
user_agent_dic = {}
user_agent_cookie = {}
MAX_RESPONSE_KB = 10*1024
CURRENT_FILE_PATH = os.path.abspath(__file__)
CURRENT_DIR_PATH = os.path.abspath(os.path.dirname(CURRENT_FILE_PATH) + os.path.sep + ".")
BASE_PATH=os.path.abspath(os.path.dirname(CURRENT_DIR_PATH))
BASE_DATA_PATH=BASE_PATH + '/data/'
DATAPATH = BASE_DATA_PATH + 'quotation'
QUOTATION_DICT_PATH_TMP =BASE_DATA_PATH + 'quotation_dict_tmp'
QUOTATION_DICT_PATH = BASE_DATA_PATH + 'quotation_dict'
UA_PATH=BASE_DATA_PATH + 'ua_list'
CODE_PATH=BASE_DATA_PATH + 'code_all'
HOLIDAY_DICT_PATH = BASE_DATA_PATH + 'holiday_dict'

id_all = {}
holiday_dict = {}

if __name__ == '__main__':
    load_ua_list()
    load_coad_all()
    load_holiday_dict()
    load_quotation(id_all)

    day_list = load_days(5)
    day_arr = []
    for day in day_list:
        day_arr.append(day.replace('-', ''))
    #print day_arr


    for day  in day_arr:
        single_path = '%s/quotation_%s' % (DATAPATH, day)
        log_write(QUOTATION_DICT_PATH_TMP, os.path.abspath(single_path))

    if os.path.isfile(QUOTATION_DICT_PATH_TMP):
        cmd = ['mv', QUOTATION_DICT_PATH_TMP, QUOTATION_DICT_PATH]
        subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

