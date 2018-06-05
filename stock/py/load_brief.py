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

from time import strftime, localtime
from datetime import timedelta, date
import calendar
import jieba
import jieba.analyse
import jieba.posseg as pseg




from bs4 import BeautifulSoup as bsp

__author__ = 'rong'

user_agent_list = []
user_agent_dic = {}
user_agent_cookie = {}
MAX_RESPONSE_KB = 10*1024
DATAPATH = './data'
id_all = {}
stopwords = {}

STOPWORDPATH = './data/stopword'

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


def log_write(filename, str, mode='a'):
    file = open(filename, mode)
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

def load_stockid_brief(id, filename):
    url = 'http://vip.stock.finance.sina.com.cn/corp/go.php/vCI_CorpInfo/stockid/%s.phtml' % (id[2:])
    #print url
    #url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/cn_bill_download.php?' \
    #      'symbol=%s&num=600&page=1&sort=ticktime&asc=0&volume=40000&amount=0&type=0&day=%s' % (id, date)

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
        soup = bsp(res['body'], "html.parser", from_encoding="gb18030")

        #x = soup.find('div', id="con02-0")
        #print x.table
        tmp_str = ''

        table = soup.find('table', id='comInfo1')
        rows = table.find_all('td', class_="ccl")

        if len(rows) >= 7:
            tmp_name = '%s/%s' % (filename, 'address_dict')
            tmp_str = '%s\t' % (id)
            tmp_str += rows[4].text
            #print tmp_str
            log_write(tmp_name, tmp_str.encode('gb18030'))

            tmp_name = '%s/%s' % (filename, 'brief_dict')
            tmp_str = '%s\t' % (id)
            tmp_str += rows[5].text
            #print tmp_str
            log_write(tmp_name, tmp_str.encode('gb18030'))

            tmp_name = '%s/%s' % (filename, 'business_scope_dict')
            tmp_str = '%s\t' % (id)
            tmp_str += rows[6].text
            #print tmp_str
            log_write(tmp_name, tmp_str.encode('gb18030'))

    return 1

def load_business_scope_split():
    filename = '%s/%s' % (DATAPATH, 'business_scope_dict')
    if not os.path.isfile(filename):
        return

    split_file = '%s/%s' % (DATAPATH, 'business_scope_dict_split')
    file = open(filename)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        items = line.strip('\n').split('\t')
        if len(items) < 2:
            continue

        flag = False
        cmd = ['grep', items[0], split_file]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(split_file) or not res_str.strip():
            flag = True

        if not flag:
            continue

        res_list = []
        res_list.append(items[0])
        for i in range(len(items)):
            if not i:
                continue

            jieba.analyse.set_stop_words(STOPWORDPATH)
            cut_list = jieba.analyse.extract_tags(items[i], allowPOS=['n'])
            #cut_list = jieba.analyse.textrank(items[i].decode('gb18030'), 2)
            for key in cut_list:
                if key is not None:
                    if key.encode('gb18030') not in res_list:
                        print items[i], key
                        res_list.append(key.encode('gb18030'))

        if not len(res_list):
            continue;

        str1 = '\t'.join(res_list)
        log_write(split_file, str1)

def load_address_split():
    filename = '%s/%s' % (DATAPATH, 'address_dict')
    if not os.path.isfile(filename):
        return

    addr_file = '%s/%s' % (DATAPATH, 'addr_dict')

    split_file = '%s/%s' % (DATAPATH, 'addr_dict_split')
    file = open(filename)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        items = line.strip('\n').split('\t')
        if len(items) < 2:
            continue

        flag = False
        cmd = ['grep', items[0], split_file]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(split_file) or not res_str.strip():
            flag = True

        if not flag:
            continue

        res_list = []
        tmp_list = []
        res_list.append(items[0])
        for i in range(len(items)):
            if not i:
                continue

            cut_list = pseg.cut(items[i].decode('gb18030'))
            #print cut_list
            for key , flag in cut_list:
                if key is not None and key not  in stopwords:
                    if flag =='ns' and key.encode('gb18030') not in tmp_list and len(tmp_list) < 2:
                        tmp_list.append(key.encode('gb18030'))

        for key in tmp_list:
            seg_list = jieba.cut_for_search(key)
            for seg_key in seg_list:
                if seg_key not in stopwords:
                        #print items[i].decode('GBK'), key, flag
                    res_list.append(seg_key.encode('gb18030'))


        if not len(res_list):
            continue;

        str1 = '\t'.join(res_list)
        log_write(split_file, str1)

        str2 = items[0]
        str2 += '\t'
        str2 += '\t'.join(tmp_list)
        log_write(addr_file, str2)

def load_stopwords():
    file = open(STOPWORDPATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        stopwords[line.strip('\r\n').decode('UTF-8')] = line.strip('\r\n').decode('UTF-8')
    file.close()

    return stopwords

def load_brief_dict(id_dic):
    file_name = DATAPATH

    for key in id_dic:
        flag = False
        tmp_name = '%s/%s' % (file_name, 'address_dict')
        cmd = ['grep', key, tmp_name]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(tmp_name) or not res_str.strip():
            flag = True

        if not flag:
            continue

        load_stockid_brief(key, file_name)
        #index = random.randint(1, 6)
        time.sleep(1)

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


if __name__ == '__main__':
    load_ua_list()
    load_coad_all()
    load_stopwords()
    load_brief_dict(id_all)
    load_address_split()
    load_business_scope_split()
    #load_stockid_brief('sz002752', '.')
