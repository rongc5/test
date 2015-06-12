#!/bin/sh

i=0
NTP_LST=( "ntp.ubuntu.com" "ntp.fudan.edu.cn" \
	 "ntp.api.bz" "time.twc.weather.com" \
	"swisstime.ethz.ch")
ntpdate()
{
    prefix="SERVER NTPDATE $1"
    /usr/sbin/ntpdate  $1 >/dev/null 
    ret=$?
    if [ $ret -eq 0 ]
    then printf "$prefix\t OK\n"
        return 1
    else printf "$prefix\t ERROR\n"
        return 0
    fi
}

while [ "$i" -lt "${#NTP_LST[@]}" ]
do
	var="${NTP_LST[ $i ]}"
	ntpdate "$var"
	ret=$?
	if [ $ret -eq 1 ]
    then  /sbin/hwclock -w >/dev/null 
	exit 0 
	else i=$((i+1))
	#i=$((i+1))
	fi
done




