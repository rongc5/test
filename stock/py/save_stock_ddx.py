#!/usr/bin/env python
#coding=utf-8


from mysql import *
import getopt, sys, re


def is_num_by_except(num):
    try:
        int(num)
        return True
    except ValueError:
#        print "%s ValueError" % num
        return False

def createTable(db):
    sql = ''
    sql = '''CREATE TABLE IF NOT EXISTS `stock_ddx` (
                `Id` INT(11) NOT NULL AUTO_INCREMENT,
                `stock_id` varchar(12) NOT NULL,
                `deal_price` varchar(8) NOT NULL COMMENT '成交价格',
                `deal_hand` int(11) default 0 COMMENT '买入的手数',
                `deal_time` varchar(8) NOT NULL COMMENT '成交的时间',
                `deal_action` char(1)  NOT NULL COMMENT '买 或者卖的标识 B or S',
                `deal_date` date NOT NULL COMMENT '成交的日期',
                `insert_time` DATETIME NOT NULL COMMENT '插入时间',
                 PRIMARY KEY (`Id`)
                ) ; '''
    db.exesql(sql)

def getStockList(db, stock_dic):
    sql = '''select stock_name, stock_id from stock_list;'''
    items = db.select(sql)

    for item in items:
        #print item['stock_name'].decode('utf-8')
        stock_dic[item['stock_name'].decode('utf-8')] = item['stock_id']


def getStockId(stock_name, base_stock_dic):
    if base_stock_dic.has_key(stock_name):
        return base_stock_dic[stock_name]

    for key in base_stock_dic:
        if stock_name in key:
            return base_stock_dic[key]

def insertTable(db, date,line, base_stock_dic, flag):
    items = line.split('\t')
    if len(items) < 3:
        return

    deal_time = items[0].decode('gbk').encode('utf-8')
    stock_name = items[1].decode('gbk').encode('utf-8')
    stock_id = getStockId(stock_name, base_stock_dic)
    if not stock_id:
        print stock_name, '''can't find stock_id'''
        return
    subitems = items[2].split('/')
    deal_price = subitems[0].decode('gbk').encode('utf-8')
    deal_hand = subitems[1].decode('gbk').encode('utf-8')
    sql = '''insert into stock_ddx(stock_id, deal_price, deal_hand, deal_time, deal_action, deal_date, insert_time)
    values(%s, %s, %s, %s, %s, %s, now());'''


    param = (stock_id, deal_price, deal_hand, deal_time, flag, date)
    db.execute(sql, param)

def delete(db, date, flag):
    sql = ''

    sql = '''delete from stock_ddx where deal_date = %s and deal_action = '%s';''' % (date, flag)

    db.exesql(sql)

def truncate(db):
    sql = ''

    sql = '''truncate table stock_ddx;'''

    db.exesql(sql)

def main(argv):

    reload(sys)
    sys.setdefaultencoding("utf-8")
    #print len(argv[1:]), argv[1:]

    try:
        opts, args = getopt.getopt(argv[1:], 'f:')
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)


    filename = ''
    flag = ''
    for o,a in opts:
        if o in '-f':
            filename = a

    res = re.search('[0-9]{4,}', filename)
    if not res:
        return
    date = res.group(0)
    if 'in' in filename:
        flag = 'B'
    elif 'out' in filename:
        flag = 'S'
    else:
        print '文件命名不合乎规范 like 20161019_in.txt'
        return


    db = MySQL('localhost','root','123456')
    db.selectDb('stock')

    createTable(db)

    delete(db, date, flag)
    base_stock_dic = {}
    getStockList(db, base_stock_dic)

    #truncate(db)
    for line in open(filename):
        line = line.strip()
        if not line:
            continue

        insertTable(db, date,line, base_stock_dic, flag)



if __name__ == '__main__':
    main(sys.argv)






