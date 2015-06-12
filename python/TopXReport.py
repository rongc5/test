#coding=utf-8
#!/usr/bin/python
##
# @file TopXReport.py
# @brief
# @author zhangming
# @version 1.2.0
# @date 2013-12-30



import os, sys, time, MySQLdb, logging, datetime, xlwt, logging.handlers, calendar, string


class myExcel:
    def __init__(self, bookname, sheetname):
        self.bookname = bookname
        self.sheetname = sheetname
        self.wd = xlwt.Workbook()
        self.sd = self.wd.add_sheet(self.sheetname, cell_overwrite_ok=True)
        self.style = style = xlwt.XFStyle()
        font = xlwt.Font()
        font.name = 'SimSun'
        #font.name = 'Times New Roman'
        font.bold = False
        self.style.font = font


        self.i = 0
        self.j = 0
    def write(self, direction, value, count=1):
        if direction == 0:
            self.i += count
        else:
            self.j += count

        #print self.i, self.j, value, count

        self.sd.write(self.i, self.j, value, self.style)
        """direction == 0  means Vertical direction
           direction == 1 means horizontal direction
        """
    def setStartLocation(self, i=0, j=0):
        self.i = i
        self.j = j
    def setILocation(self, i=0):
        self.j = i

    def setJLocation(self, j=0):
        self.j = j

    def setMoveXLocation(self, x=1):
        self.i += x

    def setMoveYLocation(self, y=1):
        self.j += y

    def getLocation(self):
        mylist = [self.i, self.j]
        return mylist

    def over(self):
        #print self.bookname, 'hello'
        self.wd.save(self.bookname)
        #self.wd.save('Topx.xls')
        """save the xls with your init name"""


class TopXReport:
    def __init__(self):
        try:
            """log init """
            format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s ->%(message)s'
            log = '%s.log' % ('TopX统计'.decode('UTF-8'))
            handler = logging.handlers.RotatingFileHandler(log, 'a', 256*1024*1024, 0)
            formatter = logging.Formatter(format)
            handler.setFormatter(formatter)
            self.logger = logging.getLogger('test')
            self.logger.addHandler(handler)
            self.logger.setLevel(logging.DEBUG)

            """mysql init"""
            #self.db = MySQLdb.connect(ip, usr, pwd, datebase, port)
            self.db = MySQLdb.connect('123.103.62.131', 'permitdbuser', 'permitdbpwd', 'statistics', 3306)
            self.con = self.db.cursor()
            #self.con.select_db('statistics')
            charset = 'SET NAMES utf8'
            self.con.execute(charset)

            self.epgType = {'不分类':0, '娱乐':56, '体育':57, '财经':59, '少儿':60, '科教':69, '影视':83, '生活':84}

            self.programTables= {1:'TOP_Program_TW_Ratings', 2:'TOP_Program_Week_Ratings', 3:'TOP_Program_TM_Ratings'
            ,4:'TOP_Program_Month_Ratings', 5:'TOP_Program_TY_Ratings', 6:'TOP_Program_Year_Ratings', 7:'TOP_Program_Date_Ratings'}

            self.channleTables={1:'TOP_Channel_TW_Ratings', 2:'TOP_Channel_Week_Ratings', 3:'TOP_Channel_TM_Ratings'
            ,4:'TOP_Channel_Month_Ratings', 5:'TOP_Channel_TY_Ratings', 6:'TOP_Channel_Year_Ratings', 7:'TOP_Channle_Date_Ratings'}

            self.programfilters = ['天气', '气象', '前情提要', '新闻']
            self.channlefilters = ['CCTV']

            self.ReportLevel = 0

            self.logger.debug("init complete")

        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
            self.logger.info("init failure")
            sys.exit(0)
    def setExcel(self):
        if self.ReportLevel:
            self.bookname = '%s(%s%s%s).xls' % ('TopX省级统计'.decode('UTF-8'), self.start, ' 至 '.decode('UTF-8'), self.end)
        else:
            self.bookname = '%s(%s%s%s).xls' % ('TopX全国统计'.decode('UTF-8'), self.start, ' 至 '.decode('UTF-8'), self.end)

        self.sheetname = '%s' % ('TopX')
        self.excel = myExcel(self.bookname, self.sheetname)

        self.excel.setJLocation(0)
        self.excel.write(1, '时间段'.decode('UTF-8'), 0)
        self.excel.write(1, self.start)
        self.excel.write(1, ' 至 '.decode('UTF-8'))
        self.excel.write(1, self.end)

        self.excel.setMoveXLocation(1)
        sql = '%s %s %s %s' % ('时间段'.decode('UTF-8'), self.start, ' 至 '.decode('UTF-8'), self.end)
        self.logger.debug(sql)


    def setTopx(self, topx=20):
        self.x = topx

    def setTopTables(self, i):
        self.pTable = self.programTables[i]

        self.cTable = self.channleTables[i]

    def getDefaultPeriodDate(self):
        sql = "select max(start)  from %s" % (self.pTable)
        self.con.execute(sql)
        rows = self.con.fetchall()
        self.start = '%s' % (rows[0][0])

        sql = "select max(end)  from %s" % (self.pTable)
        self.con.execute(sql)
        rows = self.con.fetchall()
        self.end = '%s' % (rows[0][0])

    def setPeriodDate(self, start, end):
        self.start = start
        self.end = end

    def setCityid(self, cityid):
        self.cityid = cityid

    def IsProgramFilter(self, a):
        a = '%s' % (a)
        for m in self.programfilters:
            #print m, a, a.find(m)
            if a.find(m) != -1:
                return 1
        return 0

    def setReportLevel(self, ReportLevel):
        self.ReportLevel = ReportLevel

    def IsChannleFilter(self, a):
        a = '%s' % (a)
        for m in self.channlefilters:
            if a.find(m) != -1:
                return 1
        return 0

    def getNetMAcAmount(self):
        sql = "select count(distinct a.mac) from statistics.t_devChannelInfo a, haierdb.udev_basic b where a.mac = b.mac"

        self.con.execute(sql)

        rows = self.con.fetchall()

        #print rows[0][0]
        self.excel.setJLocation(0)
        self.excel.write(1, '总的mac数量:'.decode('UTF-8'), 0)
        self.excel.write(1, rows[0][0])

        self.excel.setMoveXLocation(1)

        sql = '%s rows:%s' % (sql, rows[0][0])
        self.logger.debug(sql)

    def getActivMAcAmount(self):
        sql = "select count(distinct a.mac) from statistics.t_devChannelInfo a, haierdb.udev_basic b where a.mac = b.mac and b.isactiv = 1"
        self.con.execute(sql)
        rows = self.con.fetchall()

        self.excel.setJLocation(0)
        self.excel.write(1, '去工厂mac数量:'.decode('UTF-8'), 0)
        self.excel.write(1, rows[0][0])

        self.excel.setMoveXLocation(1)

        sql = '%s rows:%s' % (sql, rows[0][0])
        self.logger.debug(sql)

    def getDynamicMAcAmountByDate(self):
        sql = '''select count(distinct a.mac) from statistics.t_UserWatchProgramInfo a, haierdb.udev_basic b where a.mac = b.mac and b.isactiv = 1  \
               and a.epg_date BETWEEN '%s' and '%s'; ''' % (self.start, self.end)
        self.con.execute(sql)
        rows = self.con.fetchall()

        self.excel.setJLocation(0)
        self.excel.write(1, '本时段活跃mac数量:'.decode('UTF-8'), 0)
        self.excel.write(1, rows[0][0])

        self.excel.setMoveXLocation(1)

        sql = '%s rows:%s' % (sql, rows[0][0])
        self.logger.debug(sql)

    def getDynamicMAcAmountByTime(self):
        if self.cityid:
            sql = '''select count(distinct a.mac) from statistics.t_UserWatchProgramInfo a, haierdb.udev_basic b where a.mac = b.mac \
                and b.isactiv = 1  and b.citycode = '%d' and a.epg_stime = '%s' and a.epg_etime = '%s'; ''' % (self.cityid, self.start, self.end)
        else:
            sql = '''select count(distinct a.mac) from statistics.t_UserWatchProgramInfo a, haierdb.udev_basic b where a.mac = b.mac \
                and b.isactiv = 1  and a.epg_stime = '%s' and a.epg_etime = '%s'; ''' % (self.start, self.end)

        self.con.execute(sql)
        rows = self.con.fetchall()

        self.excel.setJLocation(0)
        self.excel.write(1, '本时段活跃mac数量:'.decode('UTF-8'), 0)
        self.excel.write(1, rows[0][0])

        self.excel.setMoveXLocation(1)

        sql = '%s rows:%s' % (sql, rows[0][0])
        self.logger.debug(sql)

    def load_user_info(self):
        if self.cityid:
            sql = '''select a.id, a.epg_channleid, a.epg_programid, a.epg_propertyid, UNIX_TIMESTAMP(a.epg_stime),  \
            UNIX_TIMESTAMP(a.epg_etime), UNIX_TIMESTAMP(a.dev_stime), UNIX_TIMESTAMP(a.dev_etime), b.citycode = '%d' \
            from statistics.t_UserWatchProgramInfo a, haierdb.udev_basic b where a.mac = b.mac and b.isactiv = 1 \
            and a.epg_stime = '%s' and a.epg_etim = '%s';''', (self.cityid, self.start, self.end)
        else:
            sql = '''select a.id, a.epg_channleid, a.epg_programid, a.epg_propertyid, UNIX_TIMESTAMP(a.epg_stime),  \
            UNIX_TIMESTAMP(a.epg_etime), UNIX_TIMESTAMP(a.dev_stime), UNIX_TIMESTAMP(a.dev_etime) \
            from statistics.t_UserWatchProgramInfo a, haierdb.udev_basic b where a.mac = b.mac and b.isactiv = 1 \
            and a.epg_stime = '%s' and a.epg_etim = '%s';''', (self.start, self.end)

    def getChannleTopInfo(self):
        sql = '''select b.channlename,  a.Ratings, a.MartShr from %s a, haierdb.epg_channle b  where a.cityid = %d and \
        a.epg_channleid = b.channleid  and a.start = '%s' and a.end = '%s' ORDER BY a.Ratings DESC, a.MartShr \
        desc limit %d ''' % (self.cTable, self.cityid, self.start, self.end, self.x)

        #print sql
        self.con.execute(sql)
        num = self.con.rowcount

        sql = '%s rows:%s' % (sql, num)
        self.logger.debug(sql)

        rows = self.con.fetchall()

        self.excel.setJLocation(0)
        if self.ReportLevel == 0:
            sql= '%s%d' % ( '全国卫视频道top'.decode('UTF-8'), self.x)
        elif self.ReportLevel == 1:
            sql= '%s%d' % ( '省级卫视频道top'.decode('UTF-8'), self.x)
        self.excel.write(1, sql, 0)
        self.excel.write(1, '频道名'.decode('UTF-8'))
        self.excel.write(1, '收视率'.decode('UTF-8'))
        self.excel.write(1, '市场占有率'.decode('UTF-8'))
        self.excel.setMoveXLocation(1)

        j = 1
        for row in rows:
            if self.ReportLevel == 1 and self.IsChannleFilter(row[0]):
                continue
            self.excel.setJLocation(0)
            self.excel.write(1, j, 0)
            self.excel.write(1, row[0].decode('UTF-8'))
            self.excel.write(1, row[1])
            self.excel.write(1, row[2])
            #print row[0], row[1], row[2]
            j += 1
            if (j > self.x):
                break
            self.excel.setMoveXLocation(1)

        self.excel.setMoveXLocation(3)

    def getProgramInfo(self):
        for m in self.epgType:
            if self.epgType[m] == 0:
                sql = '''SELECT  a.channlename, t2.epg_name, t1.Ratings, t1.MartShr from haierdb.epg_channle a, \
                (select epg_channleid, epg_programid, Ratings, MartShr from %s where cityid = %d and \
                start = '%s' and end = '%s' ORDER BY Ratings DESC,MartShr desc limit 3000) as t1, \
                (select  epg_name, epg_channleid, epg_programid from statistics.epg_program_syn where \
                epg_date BETWEEN '%s' and '%s' GROUP BY epg_channleid, epg_programid) as t2 where \
                a.channleid = t1.epg_channleid and t2.epg_channleid =  t1.epg_channleid and \
                t2.epg_programid = t1.epg_programid ORDER BY t1.Ratings DESC,t1.MartShr desc''' % (self.pTable, self.cityid, self.start, self.end, self.start, self.end)
                print m, self.epgType[m]
            else:
                sql = '''SELECT  a.channlename, t2.epg_name, t1.Ratings, t1.MartShr from haierdb.epg_channle a, \
                (select epg_channleid, epg_programid, Ratings, MartShr from %s where cityid = %d and start = '%s' \
                and end = '%s' ORDER BY Ratings DESC,MartShr desc limit 3000) as t1, (select  epg_name, epg_channleid, \
                epg_programid, epg_propertyid from statistics.epg_program_syn where epg_date BETWEEN '%s' and '%s' \
                GROUP BY epg_channleid, epg_programid) as t2 where  a.channleid = t1.epg_channleid and \
                t2.epg_channleid = t1.epg_channleid and t2.epg_programid = t1.epg_programid and LOCATE('%d',t2.epg_propertyid)>0 \
                ORDER BY t1.Ratings DESC,t1.MartShr desc ''' % (self.pTable, self.cityid, self.start, self.end, self.start, self.end, self.epgType[m])
                print m, self.epgType[m]

            self.logger.debug(sql)
            self.con.execute(sql)
            rows = self.con.fetchall()

            if self.ReportLevel == 0:
                sql= '%s%s%d' % (m.decode('UTF-8'), '全国卫视节目top'.decode('UTF-8'), self.x)
            elif self.ReportLevel == 1:
                sql= '%s%s%d' % (m.decode('UTF-8'), '省级卫视节目top'.decode('UTF-8'), self.x)
            self.excel.setJLocation(0)
            self.excel.write(0, sql, 0)
            self.excel.write(1, '频道名'.decode('UTF-8'))
            self.excel.write(1, '节目名'.decode('UTF-8'))
            self.excel.write(1, '收视率'.decode('UTF-8'))
            self.excel.write(1, '市场占有率'.decode('UTF-8'))
            self.excel.setMoveXLocation(1)

            k = 1
            for row in rows:
                if self.IsProgramFilter(row[1]):
                    continue
                if self.ReportLevel == 1 and self.IsChannleFilter(row[0]):
                    continue
                self.excel.setJLocation(0)
                self.excel.write(1, k, 0)
                self.excel.write(1, row[0].decode('UTF-8'))
                self.excel.write(1, row[1].decode('UTF-8'))
                self.excel.write(1, row[2])
                self.excel.write(1, row[3])
                self.excel.setMoveXLocation(1)
                k += 1
                if (k > self.x):
                    break
                #print row[0],row[1], row[2], row[3]

            self.excel.setMoveXLocation(3)

    def over(self):
        self.con.close()
        self.db.close()
        self.excel.over()
        sql = '%s%s' % (self.bookname, 'generate success!')
        self.logger.debug(sql)

def main():
    topx = TopXReport()
    topx.setTopx(50)
    topx.setCityid(0)
    topx.setTopTables(7)
    #topx.getDefaultPeriodDate()
    topx.setPeriodDate("2014-01-30", "2014-01-30")
    topx.setReportLevel(0)
    topx.setExcel()

    topx.getNetMAcAmount()
    topx.getActivMAcAmount()
    topx.getDynamicMAcAmountByDate()
    topx.getChannleTopInfo()
    topx.getProgramInfo()
    topx.over()


if __name__ == '__main__':
    main()
