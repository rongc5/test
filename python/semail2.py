#coding=gbk
#!/usr/bin/python

import smtplib
from email.mime.text import MIMEText
mailto_list=["zhangming025251@gmail.com"]
mail_host="smtp.gmail.com"  #设置服务器
mail_user="zhangming025251"    #用户名
mail_pass="scat952pat469"   #口令
mail_postfix="gmail.com"  #发a件箱的后缀

def send_mail(to_list,sub,content):  #to_list：收件人；sub：主题；content：邮件内容
    me="zhangming025251"+"<"+mail_user+"@"+mail_postfix+">"   #这里的hello可以任意设置，收到信后，将按照设置显示
    msg = MIMEText(content,_subtype='html',_charset='gb2312')    #创建一个实例，这里设置为html格式邮件
    msg['Subject'] = 'hello world'    #设置主题
    msg['From'] = me
    msg['To'] = ";".join(to_list)
    try:
        s = smtplib.SMTP(mail_host, port=25, timeout=60)
        #s.connect(mail_host, 25)  #连接smtp服务器
        #s.ehlo()
        print 11111
        s.starttls()
        print 333333
        #s.ehlo()
        s.login("zhangming025251","scat952pat469")  #登陆服务器
        print 222
        s.sendmail(me, to_list, msg.as_string())  #发送邮件
        s.close()
        return True
    except Exception, e:
        print str(e)
        return False
if __name__ == '__main__':
    if send_mail(mailto_list,mail_user,"<a href='http://www.cnblogs.com/xiaowuyi'>北京海淀区</a>"):
        print "发送成功"
    else:
        print "发送失败"