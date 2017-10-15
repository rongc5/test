#!/usr/bin/env python
#encoding=gbk

import urllib2
import json
import time


__author__ = 'rong'



#
#成交明细
def get_stockid_detail(id, date):
    url = 'http://market.finance.sina.com.cn/downxls.php?date=%s&symbol=%s' % (date, id)
    res = urllib2.urlopen(url).read()
    file_object = open('%s_%s_detail' % (id, date), 'w')
    file_object.write(res)
    file_object.close( )


#实时行情
def get_stockid_real_time(id):

    user_agent = '"Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Mobile Safari/537.36"'

    headers = { 'User-Agent' : user_agent }

    url = 'http://qt.gtimg.cn/q=%s' % (id)
    req = urllib2.Request(url, headers = headers)
    res=urllib2.urlopen(req).read()
    #res = urllib2.urlopen(url, headers = headers).read()
    value = res.split('=')[1]
    stocklist = value.split('~')
    stockdict = {}
    stockdict['id'] = id.decode("unicode_escape").encode("gbk")
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
    res = urllib2.urlopen(url).read()
    #print res
    id_dic= json.loads(res)
    return id_dic


#去掉停牌等
def get_basic_list():
    id_dic = get_stock_list()

    selected_dic = {}
    for key in id_dic:
        time.sleep(3)
        res = get_stockid_real_time(key)
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

        res['name'] = id_dic[key].decode("unicode_escape")

        print res


if __name__ == '__main__':
    #id_dic = get_stock_list()
    get_basic_list()
    #for key in id_dic:
    #    print key, id_dic[key]
    #get_stockid_real_time('sz002859')
    #get_stockid_detail('2017-10-13', 'sz002859')

