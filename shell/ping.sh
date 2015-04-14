#!/bin/sh

COUNTER=1
NTP_LST=( "ntp.ubuntu.com" "ntp.fudan.edu.cn" \
	 "ntp.api.bz" "time.twc.weather.com" \
	"swisstime.ethz.ch " )
pingcmd()
{
    prefix="SERVER PING $1"
    ping -w 1 -c 1 $1>/dev/null
    ret=$?
    if [ $ret -eq 0 ]
    then printf "$prefix\t OK\n"
	/usr/sbin/ntpdate -u "$var"
        return 1
    else printf "$prefix\t ERROR\n"
        return 0
    fi
}



var=ntp.ubuntu.com
pingcmd "$var"




