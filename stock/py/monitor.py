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
MAX_RESPONSE_KB = 10*1024

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
    ALLOWED_TIME = 20 # seconds

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
                        print 'hello', data.getvalue().decode('gbk')
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


#成交明细
def get_stockid_detail(date, id):
    url = 'http://market.finance.sina.com.cn/downxls.php?date=%s&symbol=%s' % (date, id)
    print url
    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]

    cookie_key = '%d_sina' % (index,)
    if user_agent_cookie.has_key(cookie_key):
        header['Cookie'] = user_agent_cookie[cookie_key]

    curl =  CurlHTTPFetcher()

    res = {}
    deal_dic = {}
    try:
        res = curl.fetch(url, None, header)
    except BaseException, e:
        print e.why
        return deal_dic

    if res.has_key('head') and res['head'].has_key('set-cookie'):
        user_agent_cookie[cookie_key] = res['head']['set-cookie'].split(';')[0]
        #print user_agent_cookie[cookie_key]

    deal_dic['total_vol'] = 0
    deal_dic['total_force'] = 0
    deal_dic['min_price'] = 0
    deal_dic['high_price'] = 0
    if res.has_key('body') and res['body'].strip():
        try:
            items = res['body'].split('\n')
            for key in items:
                subitems = key.split('\t');
                if len(subitems) == 6:
                    if not subitems[3].isdigit():
                        continue
                    vol = int(subitems[3])
                    force = int(subitems[4])
                    deal_dic['total_vol'] += vol
                    deal_dic['total_force'] += force

                    if float(subitems[1]) < deal_dic['min_price']:
                        deal_dic['min_price'] = float(subitems[1])

                    if float(subitems[1]) > deal_dic['high_price']:
                        deal_dic['high_price'] = float(subitems[1])

                    if u'买盘' in  subitems[5].decode('gbk'):
                        flag = 1
                    elif u'卖盘' in subitems[5].decode('gbk'):
                        flag = -1
                    else:
                        continue

                    if vol < 100:
                        if not deal_dic.has_key('vol_0'):
                            deal_dic['vol_0'] = 0
                            deal_dic['force_0'] = 0

                        deal_dic['vol_0'] += vol * flag
                        deal_dic['force_0'] += force * flag

                    if vol >= 100:
                        if not deal_dic.has_key('vol_1'):
                            deal_dic['vol_1'] = 0
                            deal_dic['force_1'] = 0

                        deal_dic['vol_1'] += vol * flag
                        deal_dic['force_1'] += force * flag


                    if vol >= 200:
                        if not deal_dic.has_key('vol_2'):
                            deal_dic['vol_2'] = 0
                            deal_dic['force_2'] = 0

                        deal_dic['vol_2'] += vol * flag
                        deal_dic['force_2'] += force * flag

                    if vol >= 500:
                        if not deal_dic.has_key('vol_3'):
                            deal_dic['vol_3'] = 0
                            deal_dic['force_3'] = 0

                        deal_dic['vol_2'] += vol * flag
                        deal_dic['force_2'] += force * flag

                    if vol >= 1000:
                        if not deal_dic.has_key('vol_4'):
                            deal_dic['vol_4'] = 0
                            deal_dic['force_4'] = 0

                        deal_dic['vol_4'] += vol * flag
                        deal_dic['force_4'] += force * flag

            deal_dic['total_force'] = deal_dic['total_force'] * 1.0/10000

            if deal_dic.has_key('force_0'):
                deal_dic['force_0'] = deal_dic['force_0'] * 1.0 / 10000
                deal_dic['ratio_force_0'] = deal_dic['force_0'] / deal_dic['total_force']
                deal_dic['ratio_vol_0'] = deal_dic['vol_0'] *1.0 / deal_dic['total_vol']

            if deal_dic.has_key('force_1'):
                deal_dic['force_1'] = deal_dic['force_1'] * 1.0 / 10000
                deal_dic['ratio_force_1'] = deal_dic['force_1'] / deal_dic['total_force']
                deal_dic['ratio_vol_1'] = deal_dic['vol_1'] *1.0 / deal_dic['total_vol']

            if deal_dic.has_key('force_2'):
                deal_dic['force_2'] = deal_dic['force_2'] * 1.0 / 10000
                deal_dic['ratio_force_2'] = deal_dic['force_2'] / deal_dic['total_force']
                deal_dic['ratio_vol_2'] = deal_dic['vol_2'] *1.0 / deal_dic['total_vol']

            if deal_dic.has_key('force_3'):
                deal_dic['force_3'] = deal_dic['force_3'] * 1.0 / 10000
                deal_dic['ratio_force_3'] = deal_dic['force_3'] / deal_dic['total_force']
                deal_dic['ratio_vol_3'] = deal_dic['vol_3'] *1.0 / deal_dic['total_vol']

            if deal_dic.has_key('force_4'):
                deal_dic['force_4'] = deal_dic['force_4'] * 1.0 / 10000
                deal_dic['ratio_force_4'] = deal_dic['force_4'] / deal_dic['total_force']
                deal_dic['ratio_vol_4'] = deal_dic['vol_4'] *1.0 / deal_dic['total_vol']

        except BaseException, e:
            print e.message

        #print deal_dic
        return deal_dic
        #print res['body']


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
    day = Day()
    toady = '%s' % (day.today(),)
    for key in id_dic:
        time.sleep(0.05)

        deal_dic = {}
        deal_dic = get_stockid_detail(toady, key)
        if not deal_dic.has_key('total_vol') or deal_dic['total_vol'] == 0:
            continue

        if not id_dic[key].has_key(toady):
            id_dic[key][day.today] = []

        if len(id_dic[key][toady]) >= 10:
            id_dic[key][toady].pop(0)
        id_dic[key][toady].append(deal_dic)

        print 'monitor', id_dic[key]

if __name__ == '__main__':
    while 1:
        do_check_monitor()
        time.sleep(30)
