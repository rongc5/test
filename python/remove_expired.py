#!/usr/bin/env python
#encoding=gbk

__author__ = 'mingz'

from time import strftime, localtime
from datetime import timedelta, date
import os


#dir:filed
#expired_info = [{'dir':'readcurrent', 'field':'readcurrent', 'expired': -7,
 #                'tr':'/data/mingz/bin/tr5',
 #                'redis2_ns':'10.26.24.66:7000', 'redis_tp':'1'}]

expired_info = [{'dir':'user_type', 'field':'user_type', 'expired': -1,
                 'tr':'/data/mingz/bin/tr5',
                 'redis2_ns':'10.26.24.66:7000', 'redis_tp':'1'}, {'dir':'readcurrent', 'field':'readcurrent', 'expired': -7,
				'tr':'/data/mingz/bin/tr5','redis2_ns':'10.26.24.66:7000', 'redis_tp':'1'}]
base_dir = '/data/mingz/rs9'

class Day():
    def __init__(self):
        self.year = strftime("%Y", localtime())
        self.mon = strftime("%m", localtime())
        self.day = strftime("%d", localtime())
        self.hour = strftime("%H", localtime())
        self.min = strftime("%M", localtime())
        self.sec = strftime("%S", localtime())

    def today(self):
     '''''
     get today,date format="YYYY-MM-DD"
     '''''
     return date.today()


    def todaystr(self):
     '''
     get date string, date format="YYYYMMDD"
     '''
     return self.year + self.mon + self.day


    def datetime(self):
     '''''
     get datetime,format="YYYY-MM-DD HH:MM:SS"
     '''
     return strftime("%Y-%m-%d %H:%M:%S", localtime())


    def datetimestr(self):
     '''''
     get datetime string
     date format="YYYYMMDDHHMMSS"
     '''
     return self.year + self.mon + self.day + self.hour + self.min + self.sec


    def get_day_of_day(self, n=0):
     '''''
     if n>=0,date is larger than today
     if n<0,date is less than today
     date format = "YYYY-MM-DD"
     '''
     if (n < 0):
      n = abs(n)
      return date.today() - timedelta(days=n)
     else:
      return date.today() + timedelta(days=n)


    def get_days_of_month(self, year, mon):
     '''''
     get days of month
     '''
     return calendar.monthrange(year, mon)[1]


    def get_firstday_of_month(self, year, mon):
     '''''
     get the first day of month
     date format = "YYYY-MM-DD"
     '''
     days = "01"
     if (int(mon) < 10):
      mon = "0" + str(int(mon))
     arr = (year, mon, days)
     return "-".join("%s" % i for i in arr)


    def get_lastday_of_month(self, year, mon):
     '''''
     get the last day of month
     date format = "YYYY-MM-DD"
     '''
     days = calendar.monthrange(year, mon)[1]
     mon = self.addzero(mon)
     arr = (year, mon, days)
     return "-".join("%s" % i for i in arr)


    def get_firstday_month(self, n=0):
     '''''
     get the first day of month from today
     n is how many months
     '''
     (y, m, d) = self.getyearandmonth(n)
     d = "01"
     arr = (y, m, d)
     return "-".join("%s" % i for i in arr)


    def get_lastday_month(self, n=0):
     '''''
     get the last day of month from today
     n is how many months
     '''
     return "-".join("%s" % i for i in self.getyearandmonth(n))


    def getyearandmonth(self, n=0):
     '''''
     get the year,month,days from today
     befor or after n months
     '''
     thisyear = int(self.year)
     thismon = int(self.mon)
     totalmon = thismon + n
     if (n >= 0):
      if (totalmon <= 12):
       days = str(self.get_days_of_month(thisyear, totalmon))
       totalmon = self.addzero(totalmon)
       return (self.year, totalmon, days)
      else:
       i = totalmon / 12
       j = totalmon % 12
       if (j == 0):
        i -= 1
        j = 12
       thisyear += i
       days = str(self.get_days_of_month(thisyear, j))
       j = self.addzero(j)
       return (str(thisyear), str(j), days)
     else:
      if ((totalmon > 0) and (totalmon < 12)):
       days = str(self.get_days_of_month(thisyear, totalmon))
       totalmon = self.addzero(totalmon)
       return (self.year, totalmon, days)
      else:
       i = totalmon / 12
       j = totalmon % 12
       if (j == 0):
        i -= 1
        j = 12
       thisyear += i
       days = str(self.get_days_of_month(thisyear, j))
       j = self.addzero(j)
       return (str(thisyear), str(j), days)


    def addzero(self, n):
     '''''
     add 0 before 0-9
     return 01-09
     '''
     nabs = abs(int(n))
     if (nabs < 10):
      return "0" + str(nabs)
     else:
      return nabs


    def get_today_month(self,n=0):
     '''''
     获取当前日期前后N月的日期
     if n>0, 获取当前日期前N月的日期
     if n<0, 获取当前日期后N月的日期
     date format = "YYYY-MM-DD"
     '''
     (y, m, d) = self.getyearandmonth(n)
     arr = (y, m, d)
     if (int(self.day) < int(d)):
      arr = (y, m, self.day)
     return "-".join("%s" % i for i in arr)


def load_saved_dict(dic, base_path, dst_dic):
    if not len(dic):
        return

    path_dic = []
    day = Day()
    lastday = 0
    for id_day in range(abs(dic['expired'])):
        date = ''

        date =  '%s' % (day.get_day_of_day(lastday), )
        path = '%s/%s/%s'%(base_path, date, dic['dir'])
        path_dic.append(path)

        lastday = lastday - 1

    print path_dic
    for key in path_dic:
        files = os.listdir(key)
        for file in files:
            #print file
            if not os.path.isdir(file) and file.startswith('data_'):
                if key in file:
                    f = open(file)
                else:
                    f = open(key+"/"+file)

                while 1:
                    line = f.readline().strip()
                    if not line:
                        break
                    items = line.split('\t')
                    if len(items):
                        dst_dic[items[0]] = ''
                        #print items[0]
                f.close()

def do_remove_expired(dic, base_path):
    if not len(dic):
        return

    day = Day()
    start_day = day.get_day_of_day(dic['expired'])
    path = '%s/%s/%s'%(base_path, start_day, dic['dir'])
    if not os.path.isdir(path):
        print path, 'not exist'
        return

    id_dic = {}
    load_saved_dict(dic, base_path, id_dic)
    if not len(id_dic):
        return
    
    index = dic['expired']

    while 1:
        start_day = day.get_day_of_day(index)
        path = '%s/%s/%s'%(base_path, start_day, dic['dir'])
        print path

        if not os.path.isdir(path):
            print path, 'not exist'
            break

        index -= 1
        files = os.listdir(path)
        fp = open('/data/mingz/base_list', "w")
        for file in files:
            if not os.path.isdir(file) and file.startswith('data_'):
                if path in file:
                    f = open(file)
                else:
                    f = open(path+"/"+file)

                while 1:
                    line = f.readline().strip()
                    if not line:
                        break
                    items = line.split('\t')
                    if len(items) and items[0] not in id_dic:
                        fp.write('hdel c%s %s' % (items[0], dic['field']))
                        fp.write('\n')
                        #file.write('hdel c%s %s' % (key, 'readhistory'))
                        #file.write('\n')
                        fp.flush()
                f.close()
		
        fp.close()

        cmd = '%s -s %s -p %s -c %s' % (dic['tr'], dic['redis2_ns'], 'base_list', dic['redis_tp'])
        print cmd
        res = os.popen(cmd).read()
        print res
    print len(id_dic)



if __name__ == '__main__':
    for key in expired_info:
        do_remove_expired(key, base_dir)
