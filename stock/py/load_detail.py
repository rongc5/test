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


def load_stockid_detail(date, id, filename):
    url = 'http://market.finance.sina.com.cn/downxls.php?date=%s&symbol=%s' % (date, id)

    print url
    #url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/cn_bill_download.php?' \
    #      'symbol=%s&num=600&page=1&sort=ticktime&asc=0&volume=40000&amount=0&type=0&day=%s' % (id, date)

    filename = '%s/%s' % (filename, id)

    if os.path.isfile(filename):
        print filename, url
        return 0

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
        #print res
    except BaseException, e:
        print e.message

    if res.has_key('head') and res['head'].has_key('set-cookie'):
        user_agent_cookie[cookie_key] = res['head']['set-cookie'].split(';')[0]

    res_str = ''
    if res.has_key('body') and res['body'].strip():
        items = res['body'].split('\n')
        #print res['body']
        for key in items:
            subitems = key.split('\t');
            if len(subitems) == 6:
                if not subitems[4].isdigit():
                    continue

                if u'买盘' in  subitems[5].decode('gbk'):
                    flag = 'B'
                elif u'卖盘' in subitems[5].decode('gbk'):
                    flag = 'S'
                elif u'中性盘' in subitems[5].decode('gbk'):
                    flag = 'M'
                else:
                    continue

                res_str = '%s\t%s\t%s\t%s' % (subitems[0], subitems[1], subitems[3], flag)
                log_write(filename, res_str)
    return 1

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


def get_day_type(query_date):
    url = 'http://tool.bitefu.net/jiari/?d=' + query_date
    curl =  CurlHTTPFetcher()
    curl.ALLOWED_TIME = 2

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]


    try:
        res = curl.fetch(url, None, header)
        day_type = int(res['body'])
        return day_type
    except BaseException, e:
        return -1

    return -1

def is_tradeday(query_date):
    weekday = datetime.datetime.strptime(query_date, '%Y%m%d').isoweekday()
    if weekday <= 5 and get_day_type(query_date) == 0:
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


def load_details(days_num, deal_dic):
    if len(deal_dic) == 0:
        return

    day_list = load_days(days_num)
    print day_list

    for date in day_list:
        path = '%s/%s' % (DATAPATH, date.replace('-', ''))
        cmd = ['mkdir', '-p', path]
        subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

        for key in deal_dic:
            if load_stockid_detail(date, key, path):
                index = random.randint(1, 5)
                time.sleep(index)


def load_coad_all():
    if not os.path.isfile('code_all'):
        return

    id_list = []
    file = open("code_all")
    while 1:
        line = file.readline().strip()
        if not line:
            break
        id_all[line] = line
    file.close()

user_agent_list = []
user_agent_dic = {}
user_agent_cookie = {}
MAX_RESPONSE_KB = 10*1024
DATAPATH = './data/details'
id_all = {}

if __name__ == '__main__':
    load_ua_list()
    load_coad_all()
    load_details(1, id_all)
