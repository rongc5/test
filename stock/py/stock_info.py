#!/usr/bin/env python
#encoding=utf-8


import json
import time
import pycurl
import cStringIO
import gzip, string
import unicodedata
import os

from time import strftime, localtime
from datetime import timedelta, date
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

#1.0 版本不必支持chunked,
def httpGetContent(url, req_header=[], version = '1.1'):

    buf = cStringIO.StringIO()
    response_header = cStringIO.StringIO()
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEFUNCTION, buf.write)
    c.setopt(c.CONNECTTIMEOUT, 10)
    c.setopt(c.TIMEOUT, 10)
    c.setopt(pycurl.MAXREDIRS, 5)
    c.setopt(pycurl.FOLLOWLOCATION, 1)

    #print url
    c.setopt(pycurl.USERAGENT, 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36')
    c.setopt(pycurl.ENCODING, 'gzip, deflate')
    c.setopt(pycurl.TCP_NODELAY, 1)
    if '1.1' in version:
        add_headers = ['Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
             'Connection:keep-alive','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6','Cache-Control:max-age=0',
             'DNT:1','Upgrade-Insecure-Requests:1','Accept-Charset: utf-8']
    else:
        add_headers = ['Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
             'Connection:close','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6','Cache-Control:max-age=0']
        c.setopt(pycurl.HTTP_VERSION, pycurl.CURL_HTTP_VERSION_1_0)
    if len(req_header):
        add_headers.extend(req_header)


    #for key in add_headers:
    #    print key
    #print add_headers
    c.setopt(c.HTTPHEADER, add_headers)
    c.setopt(pycurl.HTTPGET, 1)
    c.setopt(c.HEADERFUNCTION, response_header.write)
    res = {}

    try:
        c.perform()
        str_head = '%s' % (response_header.getvalue())
        str_body = '%s' % (buf.getvalue())
        res['head'] = str_head
        #print res['head']
        if 'Content-Encoding' in str_body and 'gzip' in str_body:
            res['body'] = GzipStream(str_body)
        else:
            res['body'] = str_body
        #print str_head, str_body
    except pycurl.error, error:
        errno, errstr = error
        print 'An error occurred: ', errstr
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
def get_stockid_mgzb(id, req_header=[]):
    url = 'http://comdata.finance.gtimg.cn/data/mgzb/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?mgzb-%s' %(id)
    req_header.extend(['Referer: %s' % (refer)])
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]

            try:
                id_dic= json.loads(value.strip(';\n'),  encoding="GB2312")
                if len(id_dic) < 2:
                    continue
                break
            except Exception,e:
                print 'hello', value.strip(';\n'), e
                time.sleep(0.2)


    return id_dic['data']['mgzb']

#查看每股盈利能力
def get_stockid_ylnl(id, req_header=[]):
    url = 'http://comdata.finance.gtimg.cn/data/ylnl/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?ylnl-%s' %(id)
    req_header.extend(['Referer: %s' % (refer)])
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]
            id_dic= json.loads(value.strip(';\n'), encoding="GB2312")
            if len(id_dic) < 2:
                continue
            break

    return id_dic['data']['ylnl']

#查看每股成长能力
def get_stockid_cznl(id, req_header=[]):
    url = 'http://comdata.finance.gtimg.cn/data/cznl/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?cznl-%s' %(id)
    req_header.extend(['Referer: %s' % (refer)])
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]
            try:
                id_dic= json.loads(value.strip(';\n'), encoding="GB2312")
                if len(id_dic) < 2:
                    continue
                break
            except Exception,e:
                print e
                time.sleep(0.2)

    return id_dic['data']['cznl']

#偿债及资本结构
def get_stockid_czzb(id, req_header=[]):
    url = 'http://comdata.finance.gtimg.cn/data/czzb/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/cwfx.html?czzb-%s' %(id)
    req_header.extend(['Referer: %s' % (refer)])
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]
            id_dic= json.loads(value.strip(';\n'), encoding="GB2312")
            if len(id_dic) < 2:
                continue
            break

    return id_dic['data']['czzb']


#杜邦分析
def get_stockid_dbfx(id, req_header=[]):
    url = 'http://comdata.finance.gtimg.cn/data/dbfx/%s' % (id)
    refer = 'http://stock.finance.qq.com/corp1/dbfx.html?%s' %(id)
    req_header.extend(['Referer: %s' % (refer)])
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]
            id_dic= json.loads(value.strip(';\n'))
            if len(id_dic) < 2:
                continue
            break

    return id_dic['data']['dbfx']


def get_money_flow(id, req_header=[]):
    url = 'http://qt.gtimg.cn/q=ff_%s' % (id)
    refer = 'http://finance.qq.com/stock/sother/test_flow_stock_quotpage.htm'
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(1)
        else:
            value = res['body'].split('=')[1]
            stocklist = value.split('~')
            if len(stocklist) < 10:
                print url
                time.sleep(1)
                continue
            break

    print stocklist
    stockdict = {}
    stockdict['main_force'] = float(stocklist[3])

    if 'Etag:' in res['head']:
        stockdict['Etag']  = res['head'].split('Etag:')[1].strip()

#实时行情
def get_stockid_real_time(id, req_header=[]):
    #url = 'http://qt.gtimg.cn/q=%s' % (id)
    #favicon = 'http://qt.gtimg.cn/favicon.ico'
    url = 'http://sqt.gtimg.cn/utf8/q=%s' % (id)
    favicon = 'http://sqt.gtimg.cn/favicon.ico'
    while 1:
        res = httpGetContent(url, req_header)
        if len(res) < 2:
            print url
            time.sleep(1)
        else:
            value = res['body'].split('=')[1]
            stocklist = value.split('~')
            if len(stocklist) < 48:
                print url
                time.sleep(1)
                continue
            break
    #不知道是不是反爬虫， 先请求吧
    #httpGetContent(favicon, 'Referer: %s' % (url))
    print stocklist
    stockdict = {}
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
    stockdict['change_rate'] = float(stocklist[38]) # 换手率
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
        stockdict['Etag']  = res['head'].split('Etag:')[1].strip()

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

#解禁列表, http 相应数据不全， 后边再处理吧
#day = Day()
#
#    start_day = '%s' % (day.get_day_of_day(-1))
#    end_day = '%s' % (day.get_day_of_day(1))
#    print start_day, end_day
#    get_outDxf_list(start_day.replace('-', ''), end_day.replace('-', ''))
def get_outDxf_list(start, end, req_header=[]):
    url = 'http://stock.finance.qq.com//sstock/list/view/dxf.php?c=0&b=%s&e=%s' % (start, end)
    refer = 'http://finance.qq.com/stock/dxfcx.htm?t=2&mn=%s&mx=%s' %(start, end)
    req_header.extend(['Referer: %s' % (refer)])
    print url
    while 1:
        res = httpGetContent(url, req_header, '1.0')
        if len(res) < 2:
            print url
            time.sleep(0.2)
        else:
            value = res['body'].split('=')[1]
            print value
            id_dic= json.loads(value)
            if len(id_dic) < 2:
                continue
            break

    #print res['head']
    print id_dic
    return id_dic


#去掉停牌等
def get_basic_list():
    #print id_dic
    id_dic = []
    if not os.path.isfile('code_all'):
        id_dic = get_stock_list()
    else:
        file = open("code_all")
        while 1:
            line = file.readline().strip()
            if not line:
                break
            id_dic.append(line)
        file.close()


    file = open('base_list', "w+")

    basic_dic = {}
    header = []
    flag = False
    for key in id_dic:
        if 'sz002133' in key:
            flag = True
        if not flag:
            continue

        #print header
        #time.sleep(0.2)
        res = get_stockid_real_time(key, header)

        if len(res) < 1:
            continue

        if 'Etag' in res:
            header = ['If-None-Match: %s' % (res['Etag'])]

        if res['block']:  #停牌
            continue

        #res['name'] = unicodedata.normalize('NFKD', id_dic[key]).encode('ascii','ignore')

        if res['circulation_market_value'] >= 300:
            continue

        if res['total_value'] >= 500:
            continue

        if res['pe'] > 90 or res['pe'] < 0:
            continue

        if res['end'] > 20 or res['end'] <= 2:
            continue

        res['mgzb'] = get_stockid_mgzb(key)


        if '--' not in res['mgzb'][0]['tbmgsy'] and  float(res['mgzb'][0]['tbmgsy']) < 0.2:
            continue

        if '--' not in res['mgzb'][0]['mgxjll'] and float(res['mgzb'][0]['mgxjll']) < 0.1:
            continue

        res['cznl'] = get_stockid_cznl(key)
        #if float(res['cznl'][0]['mgsy']) < 30:
        #    continue
        #
        #if float(res['cznl'][0]['zysr']) < 30:
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
        file.write(res['cznl'][0]['zysr'])
        file.write('\t')
        file.write(res['cznl'][0]['yylr'])
        file.write('\t')
        file.write(res['date'])

        file.write('\n')
        file.flush()

        print res
    file.close()


def load_base_list():
    if not os.path.isfile('base_list'):
        get_basic_list()

    id_dic = {}
    file = open("base_list")
    while 1:
        line = file.readline().strip()
        if not line:
            break
        #print 'hello world'
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
        tmp_dic['tbmgsy'] = items[7]
        tmp_dic['mgxjll'] = items[8]
        tmp_dic['mgsy'] = items[9]
        tmp_dic['zysr'] = items[9]
        tmp_dic['yylr'] = items[10]
        tmp_dic['date'] = items[11]

        id_dic[items[1]] = tmp_dic

    file.close()



#A股就是个坑， 技术指标低位了， 仍然可以再砸
#技术指标高位了， 有资金接盘仍然可以涨, 高位始终是危险
#压力如铁桶，支撑如窗户纸
#有连续的大单介入才介入， 低位大资金都不介入， 肯定有猫腻
#业绩好的，下跌， 大资金不介入， 肯定有什么利空， 业绩可以变脸
#买二--买五是大单， 而买1是小单， 下跌也不买， 明显还没有跌倒位
#尾盘再考虑是否介入, 要看下DMA, macd, kdj等
if __name__ == '__main__':
    load_base_list()
    #for key in id_dic:
    #    print key, id_dic[key]
    #get_stockid_real_time('sz002859')
    #get_stockid_detail('2017-10-13', 'sz002859')

