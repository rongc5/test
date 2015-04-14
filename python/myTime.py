#coding=utf-8
#!/usr/bin/python
#author zm
#time 2013.10.21


import os, sys, time, MySQLdb, logging, datetime, xlwt, logging.handlers, calendar, string

class myTime:
    def __init__(self):
        self.stu = time.localtime();

    def getLastDayOfMonth(self):
        """helo world"""

        year = self.stu.tm_year
        month = self.stu.tm_mon
        if month == 1:
            month = 12
            year -= 1
        else :
            month -= 1
        days = calendar.monthrange(year, month)[1]
        date = '%d-%02d-%02d' % (year, month, days)
        return date

    def getFirstDayOfLastMonth(self):

        year = self.stu.tm_year
        month = self.stu.tm_mon

        if month == 1:
            month = 12
            year -= 1
        else :
            month -= 1
        days =calendar.monthcalendar(year, month)[1]
        date = '%d-%02d-%02d' % (year, month, 1)
        return date
    def getLastMonth(self):
        year = self.stu.tm_year
        month = self.stu.tm_mon
        year = '%d%s' % (year, '年'.decode('UTF-8'))
        month = '%d%s' % (month, '月'.decode('UTF-8'))
        month = '%s%s' % (year, month)
        return month

    def getDateOfToday(self):

        date = '%d-%02d-%02d' % (self.stu.tm_year, self.stu.tm_mon, self.stu.tm_mday)
        return date
