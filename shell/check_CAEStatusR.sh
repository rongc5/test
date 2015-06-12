#!/bin/bash

programname=CAEStatusR

pidinfo=`ps  -eo pid,lstart,comm | grep $programname`
if [ ! -n "$pidinfo" ];
then
        echo "[$programname] stat: Not Running!-(port:7000) check time `date`"
        exit 1
fi

pid=`echo $pidinfo | awk '{print $1}'`
start=`echo $pidinfo | awk '{print "start time: "$2" "$3" "$4" "$5" "$6}'`
echo $start
tmp=`top -b -n 1 -p $pid | tail -2 | head -1 | awk '{print "USER: "$2, "VIRT: "$5, "RES: "$6, "%CPU: "$9"%", "%MEM: "$10"%"}'`
tcp_info="PID:$pid $start $tmp"
tcp_num=`netstat -an | grep 7000 | wc -l`
if [ $tcp_num -gt 5000 ];
then
        echo "[$programname] stat: OK-(port:7000) BUT [tcp:$tcp_num] stat: waining-(tcp connections -gt 5000) [$tcp_info]"
exit 1;
else
        echo "[$programname] stat: OK-(port:7000) AND [tcp:$tcp_num] stat: OK-(tcp connections -lt 5000)  [$tcp_info]"
fi
