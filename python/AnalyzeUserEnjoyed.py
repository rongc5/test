#coding=utf-8
#!/usr/bin/python
#Author zm
#time 2013-10-31 11:24:36

import os, sys, time, MySQLdb as MYDB, logging, datetime, logging.handlers, calendar, string

class UserEnjoyed:
    def __init__(self, date=None):
        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s ->%(message)s'
        log = '%s.log' % ('UserEnjoyed',)
        handler = logging.handlers.RotatingFileHandler(log, 'a', 256*1024*1024, 0)
        formatter = logging.Formatter(format)
        handler.setFormatter(formatter)
        self.logger = logging.getLogger('test')
        self.logger.addHandler(handler)
        self.logger.setLevel(logging.DEBUG)

        self.db = MYDB.connect('123.103.62.131', 'permitdbuser', 'permitdbpwd', 'statistics', 3306)
        self.con = self.db.cursor()
        charset = 'SET NAMES utf8'
        self.con.execute(charset)

        self.myepg = []
        self.mydev = {}
        self.myUserEnjoy = {}
        self.epgType = ['56', '57', '59', '60', '69', '83', '84']

        self.epg_sbuf = ''
        self.epg_ebuf = ''

        if not date:
            self.curdate = date

    def getPrevXdate(self, x):
        timer = long(time.time())
        timer = timer - x*24*60*60
        stu = time.localtime(timer);

        self.curdate = '%d-%02d-%02d' % (stu.tm_year, stu.tm_mon, stu.tm_mday)



    def getDateForrmat(self, mydate):
         if mydate.find('-') != -1 and mydate.find(':') == -1:
            mydate = '%s %s:%s:%s' % (self.curdate, '00', '00', '00')
         elif mydate.find('-') == -1 and mydate.find(':') != -1:
            mydate = '%s %s' % (self.curdate, mydate)
         elif mydate.find('-') == -1 and mydate.find(':') == -1:
            mydate = '%s %s:%s:%s' % (self.curdate, '00', '00', '00')

         #print  'hello', mydate
         return mydate

    def getTimer(self, formatter, mytime):
         #print formatter, mytime
         stu = time.strptime(mytime, formatter)
         timer = time.mktime(stu)
         return  long(timer)

    def load_epg_info(self):
        sql = '''select epg_channleid, epg_programid, epg_stime, epg_etime, SUBSTRING(epg_propertyid, 2, 8) from %s where   epg_date=DATE_FORMAT('%s', '%%Y-%%m-%%d') \
         and epg_stime < epg_etime and  LOCATE('55',epg_propertyid)>0 and LOCATE('76',epg_propertyid)=0;''' % ('epg_program_syn', self.curdate)
        self.con.execute(sql)
        rows = self.con.fetchall()

        print  sql
        self.logger.info(sql)

        stimer = 0
        etimer = 0
        timer = [0, 0]
        for row in rows:
            stime = ''
            etime = ''
            stime = '''%s''' % row[2]
            etime  = '''%s''' % row[3]

            #print  stime, etime, row[2], row[3], row[0], row[1]
            stime = self.getDateForrmat(stime)
            etime = self.getDateForrmat(etime)

            #print  stime, etime, row[2], row[3], row[0], row[1]
            timer[0] = self.getTimer('''%Y-%m-%d %H:%M:%S''', stime)
            timer[1] = self.getTimer('''%Y-%m-%d %H:%M:%S''', etime)

            #print  timer[0], timer[1]
            if timer[0] < stimer or  not stimer:
                #print timer[0], stimer, self.epg_sbuf
                stimer = timer[0]
                self.epg_sbuf = stime
                #print stimer

            if timer[1] > etimer:
                etimer = timer[1];
                self.epg_ebuf = etime
                #print(self.epg_ebuf)

            cid = int(row[0])
            pid = int(row[1])
            propertyid = '%s' % (row[4], )
            mychannle = (cid, pid, stimer, etimer, propertyid)
            #print mychannle
            self.myepg.insert(0, mychannle)

    def load_dev_info(self):
        sql = '''select a.id, a.cid as channelid, a.mac, UNIX_TIMESTAMP(a.start) as stimer,  UNIX_TIMESTAMP(a.end) as etimer from \
        statistics.t_devChannelInfo a, haierdb.udev_basic b where a.start < a.end and a.cid != 0 and  a.mac = b.mac and b.isactiv = 1 \
        and a.start BETWEEN  '%s' and  '%s' UNION select a.id, a.cid as channelid, a.mac, UNIX_TIMESTAMP(a.start) as stimer, \
        UNIX_TIMESTAMP(a.end) as etimer from statistics.t_devChannelInfo a, haierdb.udev_basic b where a.start < a.end and a.cid != 0 \
        and a.mac = b.mac and b.isactiv = 1 and a.end BETWEEN  '%s' and  '%s' ''' % (self.epg_sbuf, self.epg_ebuf, self.epg_sbuf, self.epg_ebuf)

        #print sql

        self.con.execute(sql)
        rows = self.con.fetchall()
        self.logger.info(sql)

        for row in rows:
            id = int(row[0])
            cid = int(row[1])
            mac = str(row[2])
            stimer = long(row[3])
            etimer = long(row[4])
            mydev = (cid, mac, stimer, etimer)
            self.mydev[id] = mydev

    def getUserEnjoy(self):
        self.myepg.sort(lambda x,y:cmp(x[0],y[0]))

        for i in self.mydev:
            mydev = self.mydev[i]
            #mac = "%s" % (mydev[1])

            for k in self.myepg:
                myepg = k
                if mydev[0] < k[0]:
                    continue;
                elif mydev[0] > k[0]:
                    break
                elif mydev[0] == k[0]:
                    if (mydev[2] >= myepg[2] and mydev[2] <= myepg[3]) or \
                        (mydev[3] >= myepg[2] and mydev[3] <= myepg[2]) or \
                        (mydev[2 <= myepg[3] and mydev[3] >= myepg[2]]):
                            #print mydev[0], k[0], mydev[2], mydev[3], myepg[2], myepg[3]
                            for m in self.epgType:
                                epg_propertyid = '%s' % myepg[4]
                                #print epg_propertyid, m
                                if epg_propertyid.find(m) == -1:
                                    continue
                                else:
                                    a = (mydev[1], m)
                                    #print a
                                    if not self.myUserEnjoy.has_key(a):
                                        self.myUserEnjoy[a] = [0, 0, 0]
                                        '''list[1] == 0 means count doesn't changed, list[2] 0 means count ,
                                        list[0] == 0 means mysql table doesn't exist mac
                                        '''
                                    self.myUserEnjoy[a][2] += 1
                                    self.myUserEnjoy[a][1] = 1
                                    break
                    else:
                        continue




    def saveUserEnjoy(self):
        for k, m in self.myUserEnjoy.iteritems():
            if m[0] == 0 and m[2] != 0:
                sql = '''insert into UserEnjoyInfo(mac, epg_propertyid, count, uptime) values('%s', '%s', '%d', NOW())''' % (k[0], k[1], m[2])
            elif m[0] == 1 and m[1] == 1 and m[2] != 0:
                sql = '''UPDATE UserEnjoyInfo set count = '%d', uptime = NOW() where mac = '%s' and epg_propertyid = '%s' ''' % (m[2], k[0], k[1])
            else:
                continue
            print sql
            self.con.execute(sql)

            self.logger.debug(sql)
        self.db.commit()

    def load_UserEnjoy_from_mysql(self):
        sql = '''SELECT mac, epg_propertyid, count from statistics.UserEnjoyInfo'''
        self.con.execute(sql)

        self.logger.info(sql)

        rows = self.con.fetchall()
        for row in rows:
            mac = str(row[0])
            propertyid = str(row[1])
            count = int(row[2])
            a= (mac, propertyid)
            self.myUserEnjoy[a] = [1, 0, count]

    def close(self):
        self.con.close()
        self.db.close()
        self.myepg = {}


def main():
    Analyl = UserEnjoyed()
    Analyl.getPrevXdate(1)
    Analyl.load_UserEnjoy_from_mysql()
    Analyl.load_epg_info()
    Analyl.load_dev_info()
    Analyl.getUserEnjoy()
    Analyl.saveUserEnjoy()
    Analyl.close()


if __name__ == '__main__':
    main()


