import xlrd, os
import xlwt
from xlutils.copy import copy

#rb = xlrd.open_workbook('Quarterly Commission Workbook1.xls','rb',formatting_info=True)
#wb = copy(rb)
#wb.save('simple2.xls')

#open('simple5.xls', "wb").write(open('Quarterly Commission Workbook1.xls', "rb").read())

src = "Quarterly Commission Workbook2.xls"
mystr = '''cp %s %s''' % (src,'simple6.xls')
#os.system (mystr)
#os.remove('simple6.xlsx')
open('simple6.xlsx', "wb").write(open('Quarterly Commission Workbook2.xlsx', "rb").read())