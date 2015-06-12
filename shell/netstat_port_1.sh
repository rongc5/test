#!/bin/sh
echo "未建立连结队列里套接字的数量"
while [ "1" = "1" ]
do
    netstat -nat | grep SYN_RECV | grep 4333 | wc -l
    sleep 2
done
