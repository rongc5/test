#!/usr/bin/env python
#coding=utf-8

import MySQLdb
import time
import subprocess
import os


def log_write(filename, str):                                                                                                                                                   
    file = open(filename, 'a') 
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

LOG_NAME='search_admin.log'

OperationalError = MySQLdb.OperationalError
class MySQL:
    def __init__(self,host,user,password,port=3306,charset="utf8"):
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
            log_write(LOG_NAME, "%s Mysql Error %d: %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[0], e.args[1]))

    def __del__(self):
        self.close()

    def escape_string(self, str):
	return self.conn.escape_string(str)
    def selectDb(self,db):
        try:
            self.db = db
            self.conn.select_db(db)
        except MySQLdb.Error as e:
            log_write(LOG_NAME, "%s Mysql Error %d: %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[0], e.args[1]))

    def checkIsAlive(self):
        try:
            sql = "select NOW()"
            self.cur.execute(sql)
            results = self.fetchAll()
        except MySQLdb.Error, e:
            self.reConnect()
            log_write(LOG_NAME, "%s Mysql Error %d: %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[0], e.args[1]))

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
            log_write(LOG_NAME, "%s Mysql Error %s: %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[1], sql))
        return results

    def exesql(self, sql):
        try:
            self.checkIsAlive()
            self.cur.execute(sql)
            self.commit()
        except MySQLdb.Error as e:
            log_write(LOG_NAME, "%s Mysql Error %s: %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[1], sql))
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
            log_write(LOG_NAME, "%s Mysql Error %s" % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), e.args[1]))

HADOOP_BIN='/data/mingz/hadoop-2.8.1/bin/hadoop'
HADOOP_HDFS='hdfs://10.26.24.165:9090'

if __name__ == '__main__':
    sql = MySQL('10.26.20.20','root','1Q2W3E4R')
    now = time.strftime('%H%M%S',time.localtime(time.time()))
    file_name = 'wjad_%d' % (int(time.time()))
    sql.selectDb('search_admin')
    res = sql.select('select gid, name, author, cp, cover_flag, chapter_flag from charge_data_recommend_weijuan;')
    #print res

    for key in res:
        res_str = 'i_%s\twjad\t%s\t%s' % (key['gid'], key['cover_flag'], key['chapter_flag'])
        log_write(file_name, res_str)
        #print res_str
    today = time.strftime('%Y-%m-%d',time.localtime(time.time()))
    hadoop_dir = '%s%s/%s/%s' % (HADOOP_HDFS, '/rs/iteminfo', today, file_name)
    cmd = [HADOOP_BIN, 'fs', '-mkdir', '-p', hadoop_dir]
    print cmd
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

    if  os.path.isfile(file_name):
        cmd = [HADOOP_BIN, 'fs', '-put', file_name, hadoop_dir]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]

        src = '%s/%s' % (hadoop_dir, file_name)
        dst = '%s/%s' % (hadoop_dir, 'part-00000')
        cmd = [HADOOP_BIN, 'fs', '-mv', src, dst]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
		
        dst = '%s/%s' % (hadoop_dir, '_SUCCESS')
        cmd = [HADOOP_BIN, 'fs', '-tochz', dst]
        res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]


