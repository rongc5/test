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

CURRENT_FILE_PATH = os.path.abspath(__file__)
CURRENT_DIR_PATH = os.path.abspath(os.path.dirname(CURRENT_FILE_PATH) + os.path.sep + ".")
BASE_PATH=os.path.abspath(os.path.dirname(CURRENT_DIR_PATH))
BASE_DATA_PATH=BASE_PATH + '/data/'
DATAPATH = BASE_DATA_PATH + 'cir_holder_dict'
DATAPATH_SPLIT = BASE_DATA_PATH + 'cir_holder_dict_split'
UA_PATH=BASE_DATA_PATH + 'ua_list'
CODE_PATH=BASE_DATA_PATH + 'code_all'

#DATAPATH = './plate'
#DATAPATH_SPLIT = './plate_dict_split'

STOPWORDPATH = BASE_DATA_PATH + 'stopword'
WORD_CUT_DIRCT_PATH = BASE_DATA_PATH + 'dict.txt'
id_all = {}
stopwords = {}

remove_dic = {u'所属概念板块':'', u'备注：此为证监会行业分类':'',  u'所属行业板块':''}
remove_words = [u'有限公司', u'合伙', u'股份', u'公司', u'账户', u'中心', u'优势', u'企业', u'全国', u'发展', u'普通', u'传统', u'管理', u'商业', u'投资', u'股票', u'专用', u'交易', u'资产', u'实业', u'活力', u'股东', u'定期', u'运营', u'资本', u'升级', u'基本', u'工业', u'有限', u'(', u')', u'集团', u'集团', u'控股集团', u'国', u'资金', u'控股', u'责任', u'有限责任', u'委托', u'集团公司', u'中']

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


def load_stockid_holders(id, filename):
    url = 'https://vip.stock.finance.sina.com.cn/corp/go.php/vCI_CirculateStockHolder/stockid/%s/displaytype/30.phtml' % (id[2:])
    #print url
    #url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/cn_bill_download.php?' \
    #      'symbol=%s&num=600&page=1&sort=ticktime&asc=0&volume=40000&amount=0&type=0&day=%s' % (id, date)

    header = {}
    index = 0
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]
    #header['User-Agent'] = 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36'

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
    tmp_str = ''
    start_date_str = ''
    if res.has_key('body') and res['body'].strip():
        soup = bsp(res['body'], "html.parser", from_encoding="gb18030")

        #x = soup.find('table', id="CirculateShareholderTable")
        #ff =  x.find_all('tr')
        #for k in ff:
        #    if k.find_all('td'):
        #        mm = k.find_all('td')
        #        for m in mm:
        #            if m.get_text().startswith('2020') and not tmp_str:
        #        #print mm.text
        #                tmp_str =  m.get_text()
        #                print tmp_str
        #    else:
        #        print k.get_text()

        x = soup.find_all('tr')
        for k in x:
            if k.find_all('div'):
                mm = k.find_all('div')
                #print type(mm)
                #print mm[0].get_text()
                tmp_list = []
                for m in mm:
                    if m.get_text().strip():
                        tmp_list.append(m.get_text())

                #tmp_list = json.dumps(tmp_list, encoding='gb18030', ensure_ascii=False)
                if len(tmp_list) == 2 and not start_date_str and u'截止日期' in tmp_list[0]:
                    #print 'hello world', tmp_list[1]
                    items = tmp_list[1].split('\n')
                    if '-' in items[0]:
                        start_date_str = items[0]
                    elif '-' in items[1]:
                        start_date_str = items[1]
                    elif '-' in items[2]:
                        start_date_str = items[2]
                elif len(tmp_list) == 5:
                    #print tmp_list[1], len(tmp_list[1])
                    tmp_str += tmp_list[1]
                    tmp_str += '\t'
                    if int(tmp_list[0]) >= 10:
                        break


        if tmp_str:
            res_str = '%s\t' % (id)

            res_str += start_date_str
            res_str += '\t'

            res_str += tmp_str
            res_str.strip('\t')

            #print res_str
            #log_write(filename, res_str.encode('utf8'))

            log_write(filename, res_str.encode('gb18030'))
    return 1

def load_holder_list(id_dic):
    file_name = DATAPATH

    for key in id_dic:
        flag = False
        cmd = ['grep', key, file_name]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(file_name) or not res_str.strip():
            flag = True

        if not flag:
            continue

        load_stockid_holders(key, file_name)
        #index = random.randint(1, 6)
        time.sleep(1)

def load_holder_split():
    if not os.path.isfile(DATAPATH):
        return

    #jieba.set_dictionary(WORD_CUT_DIRCT_PATH)
    file = open(DATAPATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        items = line.strip('\n').split('\t')
        if len(items) < 2:
            continue

        flag = False
        cmd = ['grep', items[0], DATAPATH_SPLIT]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
        if not os.path.isfile(DATAPATH_SPLIT) or not res_str.strip():
            flag = True

        if not flag:
            continue

        res_list = []

        for i in range(len(items)):
            if len(items[i]) <= 3 or i <= 1:
                res_list.append(items[i])
                continue
            cut_list = jieba.cut_for_search(items[i].decode('gb18030'), HMM=True)
            for key in cut_list:
                if key is not None and key not  in stopwords and key not  in remove_words:
                    if key.encode('gb18030') not in res_list and len(key) > 1:
                        print items[i], key.encode('gb18030')
                        res_list.append(key.encode('gb18030'))
            #cut_list = pseg.cut(items[i].decode('gb18030'), HMM=True)
            ###print cut_list
            #print type(cut_list)
            #for key , flag in cut_list:
                #if key is not None and key not  in stopwords and key not  in remove_words:
                    #if key.encode('gb18030') not in res_list and flag in ["ns", "n", "vn", "nr", "nt", "nz", "eng", "j", "nrt"]:
                        #print items[i], key.encode('gb18030'), flag
                        #res_list.append(key.encode('gb18030'))
                    #print key.encode('gb18030'),  flag
                    #print items[i], key.encode('gb18030'), flag, 'or'
                    #res_list.append(key.encode('gb18030'))
            #jieba.analyse.set_stop_words(STOPWORDPATH)
            #cut_list = jieba.analyse.extract_tags(items[i].decode('gb18030'), allowPOS=["ns", "n", "vn", "nr", "nt", "nz", "eng", "j", "nrt"])
            ##cut_list = jieba.analyse.textrank(items[i].decode('gb18030'), 2)
            #for key in cut_list:
                #if key is not None and key not  in remove_words:
                    #print items[i], key.encode('gb18030')
                    #if key.encode('gb18030') not in res_list:
                        #res_list.append(key.encode('gb18030'))

        if not len(res_list):
            continue;

        str1 = '\t'.join(res_list)
        log_write(DATAPATH_SPLIT, str1)

            ##print(", ".join(cut_list))

def load_stopwords():
    file = open(STOPWORDPATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        stopwords[line.strip('\r\n').decode('UTF-8')] = line.strip('\r\n').decode('UTF-8')
    file.close()

    return stopwords


def add_words():
    file = open(WORD_CUT_DIRCT_PATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        items = line.split(' ')
        print len(items)
        if len(items) < 3:
            continue
        print line
        jieba.add_word(items[0].strip(' '), int(items[1]), items[2])
    file.close()

def load_coad_all():
    if not os.path.isfile(CODE_PATH):
        return

    file = open(CODE_PATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')
        id_all[items[0]] = items[1]
    file.close()


if __name__ == '__main__':
    load_ua_list()
    load_coad_all()
    add_words()
    load_stopwords()

    #load_stockid_holders("sz002458", DATAPATH)

    load_holder_list(id_all)
    load_holder_split()
