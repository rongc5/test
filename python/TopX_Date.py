'''
#=============================================================================
#     FileName: TopX-Date.py
#         Desc:
#       Author: zhangming
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2013-11-19 12:18:43
#      History:
#=============================================================================
'''
#coding=utf-8
#!/usr/bin/python

import os, sys, time, MySQLdb as MYDB, datetime,string

class GenerateTopXInfo:
    def __init__(self, date=None, cityid=0):
        self.db = MYDB.connect('123.103.62.131', 'permitdbuser', 'permitdbpwd', 'statistics', 3306)
        self.con = self.db.cursor()
        charset = 'SET NAMES utf8'
        self.con.execute(charset)

        if not date:
            self.curdate = date

        if not cityid:
            self.cityid = cityid

        self.ProgramRatings = {}
        self.ChannelRatings = {}

    def getPrevXdate(self, x):
        timer = long(time.time())
        timer = timer - x*24*60*60
        stu = time.localtime(timer);

        self.curdate = '%d-%02d-%02d' % (stu.tm_year, stu.tm_mon, stu.tm_mday)

    def myDivision(self, x, y, n):
        sql =""

        sql = '%s%d.' % (sql, x/y)

        while n > 0:
            u = x%y
            x=u*10
            sql = '%s%d' % (sql, x/y)
            if not u:
                break
            n -= 1

        return sql

    def get_cityid_ActiveNum(self):
        if self.cityid:
            sql = '''SELECT DISTINCT a.mac from statistics.t_devChannelInfo a, haierdb.udev_basic b where a.start < a.end and a.mac=b.mac and \
        b.citycode = %d and b.isactiv = 1 AND  DATE_FORMAT(a.start, '%%Y-%%m-%%d')  BETWEEN DATE_FORMAT('%s', '%%Y-%%m-%%d') and \
        DATE_FORMAT('%s', '%%Y-%%m-%%d') union  SELECT DISTINCT a.mac from statistics.t_devChannelInfo a, haierdb.udev_basic b where \
        a.start < a.end and a.mac=b.mac and b.citycode = %d and b.isactiv = 1 and DATE_FORMAT(a.end, '%%Y-%%m-%%d') \
        BETWEEN DATE_FORMAT('%s', '%%Y-%%m-%%d') and  DATE_FORMAT('%s', '%%Y-%%m-%%d');''' % (self.cityid, self.curdate, self.curdate, self.cityid, self.curdate, self.curdate)
        else:
            sql = '''SELECT DISTINCT a.mac from statistics.t_devChannelInfo a, haierdb.udev_basic b where a.start < a.end and a.mac=b.mac and b.isactiv = 1 \
        and DATE_FORMAT(a.start, '%%Y-%%m-%%d') BETWEEN DATE_FORMAT('%s', '%%Y-%%m-%%d') and DATE_FORMAT('%s', '%%Y-%%m-%%d') union SELECT DISTINCT a.mac from \
        statistics.t_devChannelInfo a, haierdb.udev_basic b where a.start < a.end and a.mac=b.mac and b.isactiv = 1 and \
        DATE_FORMAT(a.end, '%%Y-%%m-%%d') BETWEEN DATE_FORMAT('%s', '%%Y-%%m-%%d') and DATE_FORMAT('%s', '%%Y-%%m-%%d');''' % (self.curdate, self.curdate, self.curdate, self.curdate)

        self.con.execute(sql)
        rows = self.con.fetchall()
        self.activenum = len(rows)
        print self.activenum

    def getProgramId(self):
        sql = '''select  channelid, programid, sum(epg_btime), sum(epg_vtime), sum(epg_allvtime) from statistics.TOP_TVRate_Mid \
            where cityid = %d and epg_date BETWEEN '%s' and '%s' GROUP BY channelid, programid;''' % (self.cityid, self.curdate, self.curdate)
        self.con.execute(sql)
        rows = self.con.fetchall()

        for row in rows:
            cid = '%s' % (row[0], )
            pid = '%s' % (row[1], )
            btime = '%s' % (row[2], )
            vtime = '%s' % (row[3], )
            avtime = '%s' % (row[4], )

            btime = string.atol(btime)
            vtime = string.atol(vtime)
            avtime = string.atol(avtime)

            Ratings = self.myDivision(vtime, btime*self.activenum, 8)
            MartShr = self.myDivision(vtime, avtime, 8)

            key = (cid, pid)
            self.ProgramRatings[key] = [Ratings, MartShr]

    def genProgrameRatings(self):
        for k, m in self.ProgramRatings.iteritems():
            sql = '''insert into TOP_Epg_Date_Ratings(cityid, start, end, channelid, programid, Ratings, MartShr) values(%d, '%s', '%s', %s, %s, '%s', '%s')'''  % (self.cityid, self.curdate, self.curdate, k[0], k[1], m[0], m[1])


            try:
                self.con.execute(sql)
                self.db.commit()
                print sql
            except:
                self.db.rollback()


    def getChannelId(self):
        sql ='''select a.channelid, sum(a.epg_vtime), sum(TIMESTAMPDIFF(MINUTE,b.epg_stime,b.epg_etime)), sum(a.epg_allvtime) from statistics.TOP_TVRate_Mid a, \
        statistics.epg_program_syn b where a.cityid = %d and a.channelid = b.epg_channleid and a.epg_date = b.epg_date and a.epg_date BETWEEN '%s' and '%s' \
        GROUP BY a.channelid;''' % (self.cityid, self.curdate, self.curdate)
        self.con.execute(sql)
        rows = self.con.fetchall()
        for row in rows:
            cid = '%s' % (row[0], )
            btime = '%s' % (row[2], )
            vtime = '%s' % (row[1], )
            avtime = '%s' % (row[3], )

            btime = string.atol(btime)
            vtime = string.atol(vtime)
            avtime = string.atol(avtime)

            Ratings = self.myDivision(vtime, btime*self.activenum, 8)
            MartShr = self.myDivision(vtime, avtime, 8)

            key = (cid,)
            self.ChannelRatings[key] = [Ratings, MartShr]


    def genChannelRatings(self):
        for k, m in self.ChannelRatings.iteritems():
            sql = '''insert into TOP_Channel_Date_Ratings(cityid, start, end, channelid, Ratings, MartShr) values(%d, '%s', '%s', %s, '%s', '%s');'''   % (self.cityid, self.curdate, self.curdate, k[0], m[0], m[1])

            try:
                self.con.execute(sql)
                self.db.commit()
                print sql
            except:
                self.db.rollback()




    def close(self):
        self.con.close()
        self.db.close()


def main():
    top = GenerateTopXInfo()
    top.getPrevXdate(1)
    top.get_cityid_ActiveNum()
    top.getProgramId()
    top.genProgrameRatings()
    top.getChannelId()
    top.genChannelRatings()
    top.close()

if __name__ == '__main__':
    main()


