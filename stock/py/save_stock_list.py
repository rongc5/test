#!/usr/bin/env python
#coding=utf-8


from mysql import *
import sys


def createTable(self, db):
    sql = '''CREATE TABLE IF NOT EXISTS `stock_list` (
            `Id` INT(11) NOT NULL AUTO_INCREMENT,
            `stock_id` INT(11) NOT NULL,
            `stock_name` char(50) NOT NULL,
            `circulated_market_value` char(50) NOT NULL,
            `circulation_capital_stock` char(50) NOT NULL,
            `ratios` char(16) NOT NULL,
            `industry` char(50) NOT NULL,
            PRIMARY KEY (`stock_id`)
            ) ; '''
    db.exesql(sql)

def insertTable(self, db, line):
    items = line.split('\t')
    if len(items) < 6:
        return

    sql = '''insert into stock_list(stock_id, stock_name, circulated_market_value,circulation_capital_stock, ratios, industry)
    values('%s', '%s', '%s', '%s', '%s', '%s')''' % (items[0], items[1], items[2], items[3], items[4], items[5])
    db.exesql(sql)

def main():
    db = MySQL('localhost','root','123456')
    db.selectDb('stock')

    createTable(db)

    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue

        insertTable(db, line)

    db.close()


if __name__ == '__main__':
    main()
