#coding=utf-8
#!/usr/bin/python

import os, sys, xdrlib, xlrd, xlwt, string

class myExcel:
    def __init__(self, bookname, sheetname):
        self.bookname = bookname
        self.sheetname = sheetname
        self.wd = xlwt.Workbook()
        self.sd = self.wd.add_sheet(self.sheetname, cell_overwrite_ok=True)
        self.style = style = xlwt.XFStyle()
        font = xlwt.Font()
        font.name = 'SimSun'
        font.bold = True
        self.style.font = font


        self.i = 0
        self.j = 0
    def write(self, direction, value, count=1):
        if direction == 0:
            self.i += count
        else:
            self.j += count

        self.sd.write(self.i, self.j, value, self.style)
        """direction == 0  means Vertical direction
           direction == 1 means horizontal direction
        """
    def setStartLocation(self, i, j):
        self.i = i
        self.j = j

    def over(self):
        self.wd.save(self.bookname)
        """save the xls with your init name"""


if __name__ == '__main__':
    test = myExcel('TopX.xls', '2013-10-01')
    for m in range(1, 90):
        test.write(0, m)
        test.write(0, '中国人'.decode('UTF-8'))
        #test.write(0, '中国人')
    test.setStartLocation(0,0)
    for word in string.lowercase:
        print word

    test.over()


