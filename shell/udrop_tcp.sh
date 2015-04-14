#!/bin/sh  
iptables=/opt/iptables.log  
iptables-save > $iptables  
drop_ip1=`cat $iptables |awk /DROP/'{print $4}'`  
for iptables_ip1 in $drop_ip1  
do  
/sbin/iptables -t nat -D PREROUTING -s $iptables_ip1 -j DROP  
done   

#定期执行此脚本  crontab -e 
