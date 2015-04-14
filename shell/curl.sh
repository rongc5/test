#!/bin/sh  
#http://web20.hujiaozhuanyi.com:80/loginBiLinUser.htmlusername13466349692password22cdd21ee2a4e4fe3071a13a9916fa87areaCode86authcodenullgraphsidnullsetupMark338E52312F9A7C281EB0A92AD5976AAA

#curl --data "username=13466349692&password=22cdd21ee2a4e4fe3071a13a9916fa87&areaCode=86&authcode=null&graphsid=null&setupMark=338E52312F9A7C281EB0A92AD5976AAA" http://web20.hujiaozhuanyi.com:80/loginBiLinUser.html

#login='''curl --data "username=13466349692&password=22cdd21ee2a4e4fe3071a13a9916fa87&areaCode=86&authcode=null&graphsid=null&setupMark=338E52312F9A7C281EB0A92AD5976AAA" http://web20.hujiaozhuanyi.com:80/loginBiLinUser.html'''

login='''curl --data "username=13466349692&password=22cdd21ee2a4e4fe3071a13a9916fa87&areaCode=86&authcode=null&graphsid=null&setupMark=338E52312F9A7C281EB0A92AD5976AAA" http://web20.hujiaozhuanyi.com:80/loginBiLinUser.html'''


logfile="logfile"

rm -f $logfile
touch $logfile
chmod 777 $logfile
i=10
while [ i -gt 0 ]
do
    start=$(date "+%T")
	echo $i, $login >> $logfile
    end=$(date "+%T")	
    echo "$i ==> $start ===> $end \n\n" >> $logfile
	i=$((i-1))
done