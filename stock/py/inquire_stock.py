#!/usr/bin/env python
#coding=utf-8


from mysql import *
import getopt, sys, re, string, time


def selectDDX_date(db, date, isall):
    sql = ''
    if isall:
        if date:
            sql = '''SELECT a.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
    a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
    stock_ddx as a, stock_list as b
    WHERE deal_date = '%s' and a.stock_id = b.stock_id;''' % (date)
        else:
            sql = '''SELECT a.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
    a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
    stock_ddx as a, stock_list as b
    WHERE a.stock_id = b.stock_id;'''
    else:
        if date:
            sql = '''SELECT c.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
stock_ddx as a, stock_list as b, (SELECT DISTINCT stock_id from stock_ddx where deal_action = 'B' and
deal_date = '%s') as `c`
WHERE c.stock_id = b.stock_id and c.stock_id = a.stock_id and a.deal_date = '%s' and c.stock_id not in
(SELECT DISTINCT stock_id from stock_ddx where deal_action = 'S' and deal_date = '%s')
 ORDER BY a.deal_hand DESC;''' % (date, date, date)
        else:
            sql = '''SELECT c.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
stock_ddx as a, stock_list as b, (SELECT DISTINCT stock_id from stock_ddx where deal_action = 'B') as `c`
WHERE c.stock_id = b.stock_id and c.stock_id = a.stock_id and c.stock_id not in
(SELECT DISTINCT stock_id from stock_ddx where deal_action = 'S')
 ORDER BY a.deal_hand DESC;'''

    res = db.select(sql)

    for item in res:
        stock_id = item['stock_id']
        stock_name = item['stock_name'].encode('utf-8')
        deal_price = item['deal_price']
        circulated_market_value = item['circulated_market_value'].encode('utf-8')
        ratios = item['ratios']
        deal_hand = item['deal_hand']
        deal_action = item['deal_action']
        deal_date = item['deal_date']
        deal_time = item['deal_time']

        #过滤掉流通市值大于100亿的股票
        se = re.search('[0-9]{1,}.{0,1}[0-9]{1,}', circulated_market_value)
        market_value = se.group(0)
        if string.atof(market_value) > 100:
            continue

        #过滤掉股价大于20的股票
        #print deal_price, market_value
        if string.atof(deal_price) > 20:
            continue

        #过滤掉ST票
        if 'ST' in stock_name or 'st' in stock_name:
            continue

        #过滤掉市盈率为负或者大于300的股票
        se = re.search('[0-9]{1,}.{0,1}[0-9]{1,}', ratios)
        if not se:
            continue
        ra = se.group(0)
        if not string.atof(ratios) or string.atof(ratios) > 200:
            continue

        sb = '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (stock_id, stock_name, deal_price, circulated_market_value, ratios, deal_hand, deal_action, deal_date, deal_time)
        print sb
    #deal_time = items[0].decode('gbk').encode('utf-8')



def selectDDX_stockid(db, date, stockid):
    if date:
        sql = '''SELECT a.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
stock_ddx as a, stock_list as b
WHERE deal_date = '%s' and a.stock_id = b.stock_id and a.stock_id = %s;''' % (date, stockid)
    else:
        sql = '''SELECT a.stock_id, b.stock_name, a.deal_price, b.circulated_market_value, b.ratios,
a.deal_hand, a.deal_action, a.deal_date, a.deal_time from
stock_ddx as a, stock_list as b
WHERE a.stock_id = b.stock_id and a.stock_id = %s;''' % (stockid)

    res = db.select(sql)

    for item in res:
        stock_id = item['stock_id']
        stock_name = item['stock_name'].encode('utf-8')
        deal_price = item['deal_price']
        circulated_market_value = item['circulated_market_value'].encode('utf-8')
        ratios = item['ratios']
        deal_hand = item['deal_hand']
        deal_action = item['deal_action']
        deal_date = item['deal_date']
        deal_time = item['deal_time']

        sb = '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (stock_id, stock_name, deal_price, circulated_market_value, ratios, deal_hand, deal_action, deal_date, deal_time)
        print sb


def select_stockid_detail(db, date, stockid):
    if date:
        sql = '''SELECT deal_hand, deal_number, deal_action, deal_price, deal_time, deal_date from'''
        sql += '''`%s` where deal_date = %s''' % (stockid, date)
    else:
        sql = '''SELECT deal_hand, deal_number, deal_action, deal_price, deal_time, deal_date from  '''
        sql +='''`%s`''' % (stockid)

    res = db.select(sql)
    for item in res:
        deal_hand = item['deal_hand']
        deal_number = item['deal_number']
        deal_action = item['deal_action']
        deal_price = item['deal_price']
        deal_date = item['deal_date']
        deal_time = item['deal_time']

        sb = '%s\t%s\t%s\t%s\t%s\t%s\t' % (deal_hand, deal_number, deal_action, deal_price, deal_date, deal_time)
        print sb

def usage():
    helpstr = '''-d 20161021
               -s 000935  个股大单情况大单情况
               -t 000935  成交明细
               -h help'''


def main(argv):

    reload(sys)
    sys.setdefaultencoding("utf-8")
    #print len(argv[1:]), argv[1:]


    try:
        opts, args = getopt.getopt(argv[1:], 'ad:hs:t:')
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)


    date = ''
    stockid = ''
    t = 0
    is_all = 0
    for o,a in opts:
        if o in '-d':
            date = a
        elif o in '-h':
            usage()
            return
        elif o in '-s':
            stockid = a
        elif o in '-t':
            stockid = a
            t = 1
        elif o in '-a':
            is_all = 1

    #if not date:
    #    date = time.strftime('%Y%m%d',time.localtime(time.time()))

    db = MySQL('localhost','root','123456')
    db.selectDb('stock')

    if t:
        select_stockid_detail(db, date, stockid)
    elif stockid:
        selectDDX_stockid(db, date, stockid)
    else:
        selectDDX_date(db, date, is_all)


if __name__ == '__main__':
    main(sys.argv)







