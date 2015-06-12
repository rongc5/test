#coding=utf-8
#!/usr/bin/python

import smtplib, mimetypes
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.image import MIMEImage

msg = MIMEMultipart()
msg['From'] = "chenfeng025251@126.com"
msg['To'] = 'chenfeng025251@126.com'
msg['Subject'] = 'Quarterly Commission Workbook'

#添加邮件内容
str = '''<html>
  <head></head>
  <body>
    <p>
        <br>Dear zhangming,</br>
        <br>Attached is quarterly  commission</br>
        <br>Thanks,</br>
        <br>richard</br>
    </p>
  </body>
</html>
'''




att1 = MIMEText(open('pay.ini', 'rb').read(), 'base64', 'gb2312')
att1["Content-Type"] = 'application/octet-stream'
att1["Content-Disposition"] = 'filename="pay.ini"'
msg.attach(att1)

str = '''\nAttached is quarterly  commission\n\nThanks\n\n'''
txt = MIMEText(str)
msg.attach(txt)

#发送邮件
smtp = smtplib.SMTP()
smtp.connect('smtp.126.com:25')
smtp.login('chenfeng025251', 'scat952pat469')
smtp.sendmail('chenfeng025251@126.com', 'chenfeng025251@126.com', msg.as_string())
smtp.quit()
print '邮件发送成功'
