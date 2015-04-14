#!/usr/bin/python

import os, sys, xdrlib, xlrd, calendar, time

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

    def getDateOfToday(self):

        date = '%d-%02d-%02d' % (self.stu.tm_year, self.stu.tm_mon, self.stu.tm_mday)
        return date

if __name__ == '__main__':
    test = myTime();
    date = test.getFirstDayOfLastMonth()
    print date
    date = test.getLastDayOfMonth()
    print date
    date = test.getDateOfToday()
    print date
    print test.getLastDayOfMonth.__doc__
