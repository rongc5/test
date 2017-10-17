#!/usr/bin/env python
#encoding=utf-8


import json
import time
import pycurl
import cStringIO
import gzip, string
import unicodedata
import os


__author__ = 'rong'


def httpGetContent(url, req_header=[]):

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
    add_headers = ['Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
             'Connection:keep-alive','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6','Cache-Control:max-age=0',
             'DNT:1','Upgrade-Insecure-Requests:1']
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
        res['body'] = str_body
        #print 'hello', str
    except pycurl.error, error:
        errno, errstr = error
        print 'An error occurred: ', errstr
    c.close()
    buf.close()
    response_header.close()
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
    httpGetContent(favicon, 'Referer: %s' % (url))
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


#去掉停牌等
def get_basic_list(id_dic = {}):
    #print id_dic
    selected_dic = {}

    file = open('base_list', "w+")

    header = []
    for key in id_dic:
        print header
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

        if res['end'] > 40 or res['end'] <= 2:
            continue

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
        file.write(str(res['swing']))
        file.write('\t')
        file.write(str(res['range_percent']))
        file.write('\t')
        file.write(res['date'])

        file.write('\n')
        file.flush()

        print res
        time.sleep(1)
    file.close()


def do_search():

    if not os.path.isfile('base_list'):
        id_dic = {}
        if not os.path.isfile('code_all'):
            id_dic = get_stock_list()
        else:
            file = open("code_all")
            while 1:
                line = file.readline().strip()
                if not line:
                    break
                id_dic[line] = {}

        get_basic_list(id_dic)

    id_dic = {}
    file = open("base_list")
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split(' ')
        tmp_dic = {}
        tmp_dic['code'] = items[0]
        tmp_dic['id'] = items[1]
        tmp_dic['end'] = items[2]
        tmp_dic['pe'] = float(items[3])
        tmp_dic['pb'] = float(items[4])
        tmp_dic['circulation_market_value'] = float(items[5])
        tmp_dic['total_value'] = float(items[6])
        tmp_dic['swing'] = float(items[7])
        tmp_dic['range_percent'] = float(items[8])
        tmp_dic['date'] = float(items[9])

        id_dic[tmp_dic['id']] = tmp_dic





if __name__ == '__main__':
    do_search()
    #for key in id_dic:
    #    print key, id_dic[key]
    #get_stockid_real_time('sz002859')
    #get_stockid_detail('2017-10-13', 'sz002859')

