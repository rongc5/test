#coding=gbk
#!/usr/bin/python

from __future__ import with_statement
import ConfigParser

import smtplib, mimetypes, os, sys, xdrlib, xlrd, xlwt, string
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

class AutoPay():
    def __init__(self):
        config=ConfigParser.ConfigParser()
        with open('pay.ini', 'r+') as cfgfile:
            config.readfp(cfgfile)
            self.mail_host=config.get('mail', 'mail_host')
            self.mail_user=config.get('mail', 'mail_user')
            self.mail_alis=config.get('mail', 'mail_alis')
            self.mail_postfix=config.get('mail', 'mail_postfix')
            self.mail_pass=config.get('mail', 'mail_pass')

            self.sub = config.get('text', 'sub')
            self.body = config.get('text', 'body')

            workbook = config.get('file', 'workbook')
            formater = config.get('file', 'foramt')

            self.workbook = '%s.%s' % (workbook, formater)

        self.me = self.mail_alis+"<"+self.mail_user+"@"+self.mail_postfix+">"

        self.bookinfo = {}
        self.sheetname = []

    def send_mail(self, to_mail, to_user, attachment):
        msg = MIMEMultipart()

        msg['Subject'] = self.sub
        msg['From'] = self.me

        user_list = '''%s <%s>''' % (to_user, to_mail)
        msg['To'] = user_list

        str = '''Dear %s,\n\n%s''' % (to_user, self.body)
        print str

        txt = MIMEText(str)
        msg.attach(txt)

        if attachment:
            str = '''filename="%s"''' % (attachment, )
            att1 = MIMEText(open(attachment, 'rb').read(), 'base64', 'gb2312')
            att1["Content-Type"] = 'application/octet-stream'
            att1["Content-Disposition"] = str
            msg.attach(att1)

        try:
            server = smtplib.SMTP()
            server.connect(self.mail_host)
            server.login(self.mail_user,self.mail_pass)
            server.sendmail(self.me, user_list, msg.as_string())
            server.quit()

            mystr = 'send to usr:%s mail:%s succeed' % (to_user, to_mail)
            print mystr
            return True
        except Exception, e:
            mystr = 'send to usr:%s mail:%s failed:' % (to_user, to_mail, str(e))
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
         ncols = table.ncols

         mydirt = {}
         for i in range(0, ncols):
             mydirt[i] = table.col_values(i)

         return mydirt

    def get_book_info(self):
        for key in self.bookinfo:
            self.bookinfo[key] = self.get_table_byname(by_name=key)


    def create_pay(self):
        std_table = self.sheetname[0]
        std_dirt = self.bookinfo[std_table]
        std_list = std_dirt[0]
        name_list = std_dirt[1]
        mail_list = std_dirt[2]

        style = xlwt.XFStyle()
        font = xlwt.Font()
        #font.name = 'SimSun'
        font.name = 'Times New Roman'
        #font.name = 'Arial'
        font.bold = True
        #pattern = xlwt.Pattern()
        #pattern.pattern = xlwt.Pattern.SOLID_PATTERN
        #pattern.pattern_fore_colour = 17
        #style.pattern = pattern
        style.font = font

        gstyle = xlwt.XFStyle()
        font.bold = False
        gstyle.font = font

        count = 0
        for eid in std_list:
            if count == 0:
                count += 1
                continue
            wd = xlwt.Workbook()
            for row in self.sheetname:
                sd = wd.add_sheet(row, cell_overwrite_ok=True)
                mytable = self.bookinfo[row]
                list = mytable[0]

                for n in range(0, len(mytable)):
                    tmp_list = mytable[n]
                    print  tmp_list[0], n
                    sd.write(0, n, tmp_list[0], style)
                    sd.col(n).set_width(0x0d00 + len(tmp_list[0]))

                j = 0
                i = 1
                for k in list:
                    if k == eid:
                         for n in range(0, len(mytable)):
                            tmp_list = mytable[n]
                            mystr = '%s' % (tmp_list[j])
                            width = len(mystr) + 10
                            if width >  sd.col(n).get_width():
                                sd.col(n).set_width(width)
                            sd.write(i, n, tmp_list[j], gstyle)

                         i += 1
                    j += 1

            eid_str = 'EID_%d.%s' % (int(eid), 'xls')
            wd.save(eid_str)
            to_user = name_list[count]
            to_mail = mail_list[count]
            count +=1

            if to_mail:
                print to_user, to_mail
                self.send_mail(to_mail, to_user, eid_str)
            else:
                mystr = '''user: %s's mail doesn't exist''' % (to_user, )
                print mystr




def main():
    pay = AutoPay()
    pay.getsheetname()
    pay.get_book_info()
    pay.create_pay()

if __name__ == '__main__':
    main()
    os.system('pause')


