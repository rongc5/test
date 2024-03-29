#coding=gbk
#!/usr/bin/python.26

from __future__ import with_statement
import ConfigParser

import smtplib, os, string
import base64
from time import sleep
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

class MyMail():
    def __init__(self):
        config=ConfigParser.ConfigParser()
        with open('eml.ini', 'r+') as cfgfile:
            config.readfp(cfgfile)
            self.mail_host=config.get('mail', 'mail_host')
            mail_port=config.get('mail', 'mail_port')
            self.mail_port = string.atoi(mail_port)
            self.mail_user=config.get('mail', 'mail_user')
            self.mail_alis=config.get('mail', 'mail_alis')
            self.mail_postfix=config.get('mail', 'mail_postfix')
            self.mail_pass=config.get('mail', 'mail_pass')
            self.to_user=config.get('mail', 'to_user')
            self.to_mail=config.get('mail', 'to_mail')

            self.sub = config.get('text', 'sub')
            self.body = config.get('text', 'body')
            self.myname=config.get('text','myname')

        self.mail_host = '%s' % (self.mail_host)
        self.mail_user = '%s' % (self.mail_user)
        self.mail_pass = '%s' % (self.mail_pass)
        self.me = self.mail_alis+"<"+self.mail_user+"@"+self.mail_postfix+">"

    def mail_login(self):
        try:
            if not self.mail_port:
                try:
                    self.server = smtplib.SMTP(self.mail_host, timeout=120)
                except:
                    self.server = smtplib.SMTP(timeout=120)
                    self.server.connect(self.mail_host)
            else:
                self.server = smtplib.SMTP(self.mail_host,port=self.mail_port,timeout=120)

            self.server.ehlo()
            if self.server.has_extn('STARTTLS'):
                self.server.starttls()
                self.server.ehlo()
            try:
                self.server.login(self.mail_user,self.mail_pass)
            except:
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

    def send_mail(self, filename):
        msg = MIMEMultipart()

        msg['Subject'] = self.sub
        msg['From'] = self.me

        user_list = '''%s <%s>''' % (self.to_user, self.to_mail)
        msg['To'] = self.to_mail

        str = '''Dear %s,\n\n%s\n\nThanks,\n\n%s''' % (self.to_user, self.body, self.myname)
        print str

        txt = MIMEText(str)
        msg.attach(txt)

        if filename:
            str = '''filename="%s"''' % (filename, )
            #att1 = MIMEText(open(filename, 'rb').read(), 'base64', 'gb2312')
            att1 = MIMEText(open(filename, 'rb').read(), 'base64', 'utf8')
            att1["Content-Type"] = 'application/octet-stream'
            att1["Content-Disposition"] = str
            msg.attach(att1)

        try:
            self.server.sendmail(self.me, user_list, msg.as_string())

            mystr = 'send to usr:%s mail:%s succeed' % (self.to_user, self.to_mail)
            print mystr
            return True
        except Exception, e:
            mystr = 'send to usr:%s mail:%s failed:%s' % (self.to_user, self.to_mail, e)
            self.server.quit()
            print  mystr
            return False

def main(filename):
    print filename
    ml = MyMail()
    ml.mail_login()
    ml.send_mail(filename)


if __name__ == '__main__':
    print 'Begin Process:'
    main("hello.pdf")
    print 'End Process'
    while True:
        sleep(60)
    #os.system('pause')


