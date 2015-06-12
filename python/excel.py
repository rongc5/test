# -*- coding: utf-8 -*-
#!/usr/bin/env python
import  xdrlib ,sys
import xlrd
def open_excel(file= 'Quarterly Commission Workbook.xlsx'):
    try:
        data = xlrd.open_workbook(file)
        #print data.sheet_names()
        return data
    except Exception,e:
        print str(e)
 #根据索引获取Excel表格中的数据   参数:file：Excel文件路径     colnameindex：表头列名所在行的所以  ，by_index：表的索引
def excel_table_byindex(file= 'Quarterly Commission Workbook.xlsx',colnameindex=0,by_index=0):
    data = open_excel(file)
    table = data.sheets()[by_index]
    #print table.col_values(0)
    nrows = table.nrows #行数
    ncols = table.ncols #列数
    colnames =  table.row_values(colnameindex) #某一行数据
    #print nrows, ncols, colnames
    list =[]
    for rownum in range(1,nrows):
        row = table.row_values(rownum)
        print len(row), row
        #if row:
            #app = {}
            #for i in range(len(colnames)):
                #app[colnames[i]] = row[i]
            #list.append(app)
    return list

 #根据名称获取Excel表格中的数据   参数:file：Excel文件路径     colnameindex：表头列名所在行的所以  ，by_name：Sheet1名称
def excel_table_byname(file= 'file.xls',colnameindex=0,by_name=u'Sheet1'):
     data = open_excel(file)
     table = data.sheet_by_name(by_name)
     nrows = table.nrows #行数
     colnames =  table.row_values(colnameindex) #某一行数据
     list =[]
     for rownum in range(1,nrows):
         row = table.row_values(rownum)
         print len(row)
         if row:
              app = {}
              for i in range(len(colnames)):
                  app[colnames[i]] = row[i]
              list.append(app)
     return list

def main():
    open_excel()


    tables = excel_table_byindex()
    for row in tables:
        print row

    return
    tables = excel_table_byname()
    for row in tables:
        print row_values

if __name__=="__main__":
    main()
