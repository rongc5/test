#!/usr/bin/env python
#coding=utf-8


from mysql import *
import getopt, sys, re


def createTable(db, stockid):
    sql = ''
    sql = '''CREATE TABLE IF NOT EXISTS `%s` (
                `Id` INT(11) NOT NULL AUTO_INCREMENT,
                `deal_price` varchar(8) NOT NULL COMMENT '成交价格',
                `deal_hand` int(11) default 0 COMMENT '成交的手数',
                `deal_number` int(11) default 0 COMMENT '成交的笔数',
                `deal_time` varchar(8) NOT NULL COMMENT '成交的时间',
                `deal_action` char(1)  NOT NULL COMMENT '买 或者卖的标识 B or S',
                `deal_date` date NOT NULL COMMENT '成交的日期',
                `insert_time` DATETIME NOT NULL COMMENT '插入时间',
                 PRIMARY KEY (`Id`)
                ) ; ''' % (stockid)
    db.exesql(sql)


def insertTable(db, stockid, date,line):
    items = line.split('\t')

    if len(items) < 5:
        return
    deal_time = items[0].decode('gbk').encode('utf-8')
    deal_price = items[1].decode('gbk').encode('utf-8')
    deal_hand = items[2].decode('gbk').encode('utf-8')
    deal_number = items[3].decode('gbk').encode('utf-8')
    deal_action = items[4].decode('gbk').encode('utf-8').upper()

    sql =  '''insert into `%s`''' % (stockid)
    sql += '''(deal_price, deal_hand, deal_number, deal_time, deal_action, deal_date, insert_time)
    values('%s', '%s', '%s', '%s', '%s', '%s', now());''' % (deal_price, deal_hand, deal_number, deal_time, deal_action, date)


    #param = (stockid, deal_price, deal_hand, deal_number, deal_time, deal_action, date)
    #db.execute(sql, param)
    db.exesql(sql)

def delete(db, stockid, date):
    sql = ''

    sql = '''delete from `%s` where deal_date = %s;''' % (stockid, date)

    db.exesql(sql)

def truncate(db, stockid):
    sql = ''

    sql = '''truncate table `%s`;''' % (stockid)

    db.exesql(sql)

def checkIsrightful(line):
    res = re.search('[S|B]$', line)
    if res:
        return True
    return False

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


    db = MySQL('localhost','root','123456')
    db.selectDb('stock')



    #truncate(db)
    flag = 1
    date = ''
    stockid = ''
    for line in open(filename):
        line = line.strip()
        if not line:
            continue

        if flag:
            res = re.search('^[0-9]{6,}', line)
            if res:
                res = re.findall('[0-9]{6,}', line)
                if len(res) < 2:
                    return
                date = res[0]
                stockid = res[1]
                createTable(db, stockid)
                delete(db, stockid,date)

                flag = 0
                continue

        if checkIsrightful(line):
            insertTable(db, stockid, date,line)



if __name__ == '__main__':
    main(sys.argv)







