#encoding=UTF-8
#!/usr/bin/python

import os, sys, MySQLdb as MYDB

class mySQL:
    def __init__(self, ip='123.103.62.131', usr='permitdbuser', pwd='permitdbpwd', datebase='statistics', port=3306, charset='utf8'):
        try:
            self.db = MYDB.connect(ip, usr, pwd, datebase, port)
            self.con = self.db.cursor()
            charset = '%s %s' % ('SET NAMES', charset)
            self.con.execute(charset)
            print ip, usr, pwd, datebase, port, charset
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])


if __name__ == '__main__':
    sql = mySQL()



