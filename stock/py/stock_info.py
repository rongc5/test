#!/usr/bin/env python
#encoding=utf-8


import json
import time
import os
import pycurl
import cStringIO
import gzip, string
import unicodedata


__author__ = 'rong'


def httpGetContent(url):
    str = ''
    buf = cStringIO.StringIO()
    header = cStringIO.StringIO()
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEFUNCTION, buf.write)
    c.setopt(c.CONNECTTIMEOUT, 10)
    c.setopt(c.TIMEOUT, 10)

    print url
    c.setopt(pycurl.USERAGENT, 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36')
    c.setopt(pycurl.ENCODING, 'gzip, deflate')
    c.setopt(pycurl.TCP_NODELAY, 1)

    c.setopt(c.HTTPHEADER, ['Accept: */*','Connection:keep-alive','Accept-Language:zh-CN,zh;q=0.8,en;q=0.6'])
    c.setopt(pycurl.HTTPGET, 1)
    c.setopt(c.HEADERFUNCTION, header.write)
    try:
        c.perform()
        str = '%s' % (buf.getvalue())
        #print 'hello', str
    except pycurl.error, error:
        errno, errstr = error
        print 'An error occurred: ', errstr
    c.close()
    buf.close()
    header.close()
    return str



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
def get_stockid_real_time(id):
    url = 'http://qt.gtimg.cn/q=%s' % (id)
    res = httpGetContent(url)
    print res
    value = res.split('=')[1]
    stocklist = value.split('~')
    stockdict = {}
    stockdict['id'] = id
    stockdict['code'] = stocklist[2]                           # 股票代码
    #stockdict['name'] = unicode(stocklist[1], 'gbk')  # 股票名称
    stockdict['last_closing'] = float(stocklist[4])    # 昨日收盘价格
    stockdict['start'] = float(stocklist[5])           # 开盘价格
    stockdict['end'] = float(stocklist[3])             # 当前收盘价格（可以是当前价格）
    stockdict['high'] = float(stocklist[33])           # 最高价格
    stockdict['low'] = float(stocklist[34])            # 最低价格
    stockdict['buyvol'] = int(stocklist[7])             # 外盘 todo 数据对不上
    stockdict['sellvol'] = int(stocklist[8])           # 内盘 todo 数据对不上

    stockdict['range_price'] = float(stocklist[31])    # 涨跌价格
    stockdict['range_percent'] = float(stocklist[32])  # 涨跌比%

    stockdict['volume'] = int(stocklist[6])            # 成交量（手）
    stockdict['total_price'] = int(stocklist[37])      # 成交额（万元）
    stockdict['change_rate'] = float(stocklist[38]) # 换手率
    stockdict['pe'] = float(stocklist[39])          # 市盈率
    stockdict['swing'] = float(stocklist[43])           # 振幅

    stockdict['pb'] = float(stocklist[46])              # 股票市净率
    stockdict['date'] = stocklist[30][:8]               # 时间
    stockdict['block'] = False if stockdict['start'] else True #股票是否停牌
    stockdict['circulation_market_value '] = float(stocklist[44])
    stockdict['total_value '] = float(stocklist[45])
    stockdict['high_limit'] = float(stocklist[47])
    stockdict['low_limit'] = float(stocklist[48])
    return stockdict

#获取所有股票列表
def get_stock_list():
    #url = 'http://www.shdjt.com/js/lib/astock.js'
    url = 'http://www.ctxalgo.com/api/stocks'
    res = httpGetContent(url)
    #print res
    id_dic= json.loads(res)

    file = open('code_all', "w+")
    for key in id_dic:
        file.write(unicodedata.normalize('NFKD', key).encode('ascii','ignore'))
        file.write('\n')

    file.close()
    return id_dic


#去掉停牌等
def get_basic_list():
    id_dic = get_stock_list()
    #print id_dic
    selected_dic = {}

    file = open('base_list', "w+")

    for key in id_dic:
        res = get_stockid_real_time(unicodedata.normalize('NFKD', key).encode('ascii','ignore'))
        if res['block']:  #停牌
            continue

        if res['circulation_market_value '] > 100:
            continue

        if res['pe'] >= 100:
            continue

        if res['low']  > 30:
            continue

        if res['change_rate'] < 1:
            continue

        #res['name'] = id_dic[key].encode("utf-8")
        res['name'] = unicodedata.normalize('NFKD', id_dic[key]).encode('ascii','ignore')

        file.write(res['code'])
        file.write('\t')
        file.write(res['id'])
        file.write('\t')
        file.write(str(res['end']))
        file.write('\n')

        print res
        time.sleep(3)
    file.close()

if __name__ == '__main__':
    #str = get_stock_list()
    #print str
    #id_dic = get_stock_list()
    get_basic_list()
    #for key in id_dic:
    #    print key, id_dic[key]
    #get_stockid_real_time('sz002859')
    #get_stockid_detail('2017-10-13', 'sz002859')

