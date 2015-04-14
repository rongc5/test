'''
#=============================================================================
#     FileName: autoPay.py
#         Desc:
#       Author: zhangming
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2013-11-27 20:07:28
#      History:
#=============================================================================
'''
#coding=gbk
#!/usr/bin/python

from __future__ import with_statement
import ConfigParser

import smtplib, os, xlrd, string
import base64
from time import sleep
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import win32com.client
from win32com.client import constants, Dispatch

class AutoPay():
    def __init__(self):
        config=ConfigParser.ConfigParser()
        with open('pay.ini', 'r+') as cfgfile:
            config.readfp(cfgfile)
            self.mail_host=config.get('mail', 'mail_host')
            mail_port=config.get('mail', 'mail_port')
            self.mail_port = string.atoi(mail_port)
            self.mail_user=config.get('mail', 'mail_user')
            self.mail_alis=config.get('mail', 'mail_alis')
            self.mail_postfix=config.get('mail', 'mail_postfix')
            self.mail_pass=config.get('mail', 'mail_pass')

            self.sub = config.get('text', 'sub')
            self.body = config.get('text', 'body')
            self.myname=config.get('text','myname')

            workbook = config.get('file', 'workbook')
            self.formater = config.get('file', 'format')

            self.workbook = '%s.%s' % (workbook, self.formater)

        self.mail_host = '%s' % (self.mail_host)
        self.mail_user = '%s' % (self.mail_user)
        self.mail_pass = '%s' % (self.mail_pass)

        self.me = self.mail_alis+"<"+self.mail_user+"@"+self.mail_postfix+">"

        self.bookinfo = {}
        self.sheetname = []

        self.std_eid = []
        self.std_user = []
        self.std_mail = []

    def mail_login(self):
        try:
            #myport = string.atoi(self.mail_port)
            #mail_host = '%s' % (self.mail_host)
            #mail_user = '%s' % (self.mail_user)
            #mail_pass = '%s' % (self.mail_pass)
            #print self.mail_host, self.mail_user, self.mail_pass, self.mail_port
            #self.server = smtplib.SMTP(self.mail_host,port=self.mail_port)

            if not self.mail_port:
                try:
                    self.server = smtplib.SMTP(self.mail_host, timeout=120)
                except:
                    self.server = smtplib.SMTP(timeout=120)
                    self.server.connect(self.mail_host)
            else:
                self.server = smtplib.SMTP(self.mail_host,port=self.mail_port,timeout=120)

            #self.server.set_debuglevel(1)
            #self.server = smtplib.SMTP_SSL(self.mail_host)

            #if self.mail_type.find('gm') != -1:
            self.server.ehlo()
            if self.server.has_extn('STARTTLS'):
                #self.server.ehlo()
                self.server.starttls()
                self.server.ehlo()
            #print  111111
            #print self.mail_user, self.mail_pass
            try:
                self.server.login(self.mail_user,self.mail_pass)
            except:
                #self.server.docmd( " EHLO server " )
                self.server.docmd( " AUTH LOGIN " )
                self.server.send(base64.encodestring(self.mail_user))
                self.server.getreply()
                self.server.send(base64.encodestring(self.mail_pass))
                self.server.getreply()
            print  'Email login success'
            return True
        except Exception, e:
            mystr = 'Email login  failed:%s' % (e,)
            print  mystr
            return False

    def send_mail(self, to_mail, to_user, attachment):
        msg = MIMEMultipart()

        msg['Subject'] = self.sub
        msg['From'] = self.me

        user_list = '''%s <%s>''' % (to_user, to_mail)
        msg['To'] = user_list

        str = '''Dear %s,\n\n%s\n\nThanks,\n\n%s''' % (to_user, self.body, self.myname)
        #print str

        txt = MIMEText(str)
        msg.attach(txt)

        if attachment:
            str = '''filename="%s"''' % (attachment, )
            att1 = MIMEText(open(attachment, 'rb').read(), 'base64', 'gb2312')
            att1["Content-Type"] = 'application/octet-stream'
            att1["Content-Disposition"] = str
            msg.attach(att1)

        try:
            self.server.sendmail(self.me, user_list, msg.as_string())

            mystr = 'send to usr:%s mail:%s succeed' % (to_user, to_mail)
            print mystr
            return True
        except Exception, e:
            mystr = 'send to usr:%s mail:%s failed:%s' % (to_user, to_mail, e)
            self.server.quit()
            print  mystr
            return False

    def getsheetname(self):
        try:
            data = xlrd.open_workbook(self.workbook)
            list = data.sheet_names()

            self.sheetname = list

            for row in list:
                self.bookinfo[row] = 0

        except Exception,e:
            print 'open excel err: ' + str(e)

    def get_table_byname(self, by_name=u'Sheet1'):

         data = xlrd.open_workbook(self.workbook)
         table = data.sheet_by_name(by_name)

         return table.col_values(0)

    def get_book_std(self):
         data = xlrd.open_workbook(self.workbook)
         name = self.sheetname[0]
         table = data.sheet_by_name(name)

         self.std_eid = table.col_values(0)
         self.std_user = table.col_values(1)
         self.std_mail = table.col_values(2)


    def get_book_info(self):
        for key in self.bookinfo:
            self.bookinfo[key] = self.get_table_byname(by_name=key)


    def create_pay(self):

        std_list = self.std_eid
        name_list = self.std_user
        mail_list = self.std_mail
        stdPath = os.path.abspath(self.workbook)

        for k in range(1, len(std_list)):
            eid = std_list[k]
            eid_str = 'EID_%d.%s' % (int(eid), self.formater)
            xlsPath = eid_str
            absPath = os.path.abspath(xlsPath);

            try:
                open(absPath, "wb").write(open(stdPath, "rb").read())
            except:
                print "%s existed!" % (absPath)
                os.remove(absPath)
                open(absPath, "wb").write(open(stdPath, "rb").read())
            #open(eid_str, "wb").write(open('Quarterly Commission Workbook2.xlsx', "rb").read())


            print eid_str
            exc = win32com.client.gencache.EnsureDispatch('Excel.Application')
            exc.Visible = 0
            exc.Workbooks.Open(Filename=absPath)

            wbk = exc.ActiveWorkbook

            for t in range(0, len(self.sheetname)):
                id = t + 1
                sheet = wbk.Sheets(id)
                table = self.sheetname[t]
                list = self.bookinfo[table]
                length = len(list)

                #print  table,  eid

                i = 2
                count = i
                while i <= length:
                    if count > length:
                        break
                    data = sheet.Cells(i,1).Value
                    if data != eid:
                        #print data, eid
                        sheet.Rows(i).Delete(Shift=constants.xlUp)
                    else:
                        i += 1
                    count += 1

            wbk.Save()
            #wbk.SaveAs(eid_str)
            wbk.Close()
            exc.Quit()
            if k < len(name_list):
                to_user = name_list[k]

            if k < len(mail_list):
                to_mail = mail_list[k]

            if to_mail:
                #print to_user, to_mail
                while not self.send_mail(to_mail, to_user, eid_str):
                    while not self.mail_login():
                        sleep(1)
                    sleep(1)
            else:
                mystr = '''user: %s's mail doesn't exist''' % (to_user, )
                print mystr
            to_mail = ''
            to_user = ''
    def close(self):
        self.server.quit()



def main():
    pay = AutoPay()
    pay.getsheetname()
    pay.get_book_std()
    pay.get_book_info()
    while not pay.mail_login():
        sleep(1)
    pay.create_pay()
    pay.close()

if __name__ == '__main__':
    print 'Begin Process:'
    main()
    print 'End Process'
    while True:
        sleep(60)
    #os.system('pause')


