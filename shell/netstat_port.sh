#!/bin/sh
echo "已建立连结队列里套接字的数量"
while [ "1" = "1" ]
do  
    netstat -nat | grep ESTABLISHED | grep 8888 | wc -l
    sleep 2
done
