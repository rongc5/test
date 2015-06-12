#!/bin/sh  
netstat-nat -n > /opt/netstat-net.log  
file=/opt/drop_ip.log  
#判断连接数大于20的IP  
/bin/awk -F: '/tcp/{a[$(NF-2)]++}END{for(i in a)if(a[i]>20)print i}' /opt/netstat-net.log > $file  
drop_ip=`cat $file |awk '{print $2}'`  
for iptables_ip in $drop_ip  
do  
#如果iptables的PREROUTING链中，没有出现过这个IP，则直接用iptables丢弃所有来自这个IP地址发送的请求  
if [ $iptables_ip != $0 ] && [ -z "` iptables -nvL -t nat|grep $iptables_ip`" ];then  
/sbin/iptables -t nat -I PREROUTING -s $iptables_ip -j DROP  
fi  
done 
