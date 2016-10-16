#!/usr/bin/env python
#coding=utf-8

import MySQLdb
from myLog import *

OperationalError = MySQLdb.OperationalError
class MySQL:
    def __init__(self, host,user,password,port=3306,charset="utf8"):
        self.host=host
        self.port=port
        self.user=user
        self.password=password
        self.charset=charset
        self.db = ''
        try:
            self.conn=MySQLdb.connect(host=self.host,port=self.port,user=self.user,passwd=self.password)
            self.conn.autocommit(False)
            self.conn.set_character_set(self.charset)
            self.cur=self.conn.cursor()
        except MySQLdb.Error as e:
            print("Mysql Error %d: %s" % (e.args[0], e.args[1]))

    def __del__(self):
        self.close()

    def escape_string(self, str):
	return self.conn.escape_string(str)
    def selectDb(self,db):
        try:
            self.db = db
            self.conn.select_db(db)
        except MySQLdb.Error as e:
            print("Mysql Error %d: %s" % (e.args[0], e.args[1]))

    def checkIsAlive(self):
        try:
            sql = "select NOW()"
            self.cur.execute(sql)
            results = self.fetchAll()
        except MySQLdb.Error, e:
            self.reConnect()

    def fetchAll(self):
        result=self.cur.fetchall()
        #print("fetchAll ===> len = %u", len(result))
        desc =self.cur.description
        d = []
        for inv in result:
            _d = {}
            for i in range(0,len(inv)):
                invStr = str(inv[i])
                if invStr == 'None':
                    invStr = ""
                _d[desc[i][0]] = invStr
            d.append(_d)
        return d

    def select(self, sql):
        results = []
        try:
            self.checkIsAlive()
            self.cur.execute(sql)
            results = self.fetchAll()
            #print results
        except MySQLdb.Error as e:
            print("Mysql Error:%s\nSQL:%s" %(e,sql))
        return results

    def exesql(self, sql):
        try:
            self.checkIsAlive()
            self.cur.execute(sql)
            self.commit()
        except MySQLdb.Error as e:
            print("Mysql Error:%s\nSQL:%s" %(e,sql))
            self.rollback()
            self.reConnect()

    def commit(self):
        self.conn.commit()

    def rollback(self):
        self.conn.rollback()

    def close(self):
        self.cur.close()
        self.conn.close()

    def reConnect(self):
        self.close()
        try:
            self.conn=MySQLdb.connect(host=self.host,port=self.port,user=self.user,passwd=self.password)
            self.conn.autocommit(False)
            self.conn.set_character_set(self.charset)
            self.cur=self.conn.cursor()
            if self.db:
                self.selectDb(self.db)
        except MySQLdb.Error as e:
            print("Mysql Error %d: %s" % (e.args[0], e.args[1]))



if __name__ == '__main__':
    sql = MySQL('localhost','root','123456')

    sql.selectDb('test')
    res = sql.select('select * from epg_channle;')
    print res
