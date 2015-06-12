#!/bin/bash
func()
{
	if [ $# -lt 2 ]
	then
		echo "using .... basePath  baseLogName"
		exit
	fi
	

	basePath=$1
	baseLog=$2
	MAXLOGNUM=10
	

	if [ -d $basePath ]
	then
		cd $basePath
	else
		echo "$basePath is not dir"
		exit
	fi
	
	
	allLogfile=`ls -ld $basePath/$baseLog*  |wc -l`
	
	echo $allLogfile 
	date=`date +%Y%m%d%k%M`
	
	
	if [ $allLogfile -gt $MAXLOGNUM ]
	then
		cd $basePath
		mkdir logback
		tar -zcvf logback/$baseLog-$date.tar.gz $baseLog*
		rm -rf $baseLog*
	fi
}

caewhilepath=/home/haier/unitServer/log/CAEWhilst
func $caewhilepath whilst
