#!/bin/bash
 #清空前一次的记录
 #echo "" > ~/send.txt
 #echo "Now total on line IP:" >> ~/send.txt
 #统计出有多少人线访问
 #/bin/netstat -ant | grep ESTABLISHED |wc -l >> ~/send.txt
 #统计内存使用了多少
 #echo "Total memory:" >> ~/send.txt
 #free -m|grep Mem|awk '{print $2}' >> ~/send.txt
 #echo "Used memory:" >> ~/send.txt
 #free -m|grep Mem|awk '{print $3}' >> ~/send.txt
 #统计出前一天总访问IP数：
 #echo "Yesterday total access IP:" >> ~/send.txt
 #find /home/webserver/httpd2.2.14/logs/backup_logs -type f -mtime 1 -exec awk -F "-" '{print $1}' {} \;|sort|uniq|wc -l >> ~/send.txt
truncate -s 0 send.txt
echo "hello world" >>send.txt
echo "this is a test" >> send.txt
 #cat ~/send.txt | mail -s "hello" 13466349692@139.com
 #cat send.txt | mail -s "hello" zhangming025251@gmail.com
 cat send.txt | mail -s "hello" zhangxl@haierubic.com
