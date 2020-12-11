#!/bin/bash
source /etc/profile
source ~/.bash_profile



stime=$(date "+%Y-%m-%d %H:%M:%S")
PROC_NAME=load_quarterly.sh

ProcNumber=`ps -ef |grep -w $PROC_NAME|grep -v grep|wc -l`
if [ $ProcNumber -le 0 ];then
    echo $LINENO, "$PROC_NAME is not run" >> timer.log
else
    echo $LINENO, "$PROC_NAME is  running..", $stime >> timer.log
    exit
fi


id_arr=load_*.py
for PROC_NAME in $id_arr
do
ProcNumber=`ps -ef |grep -w $PROC_NAME|grep -v grep|wc -l`
if [ $ProcNumber -le 0 ];then
    echo $LINENO, "$PROC_NAME is not run" >> timer.log
else
    echo $LINENO, "$PROC_NAME is  running..", $stime >> timer.log
    exit
fi
done


##返回这个脚本文件放置的目录
programName=${0##*/}


cd `dirname $0`
path=`pwd`
echo $LINENO, "start", $path, $programName, $stime >> timer.log


python $path/load_ids.py 
python $path/load_finance.py 
python $path/load_brief.py
python $path/load_plate.py 

cp ../data/addr* ~/sk_ser/data/
cp ../data/plate_* ~/sk_ser/data/
cp ../data/finance_dict ~/sk_ser/data/
cp ../data/code_all ~/sk_ser/data/


etime=$(date "+%Y-%m-%d %H:%M:%S")
echo $LINENO, "end", $path, $programName, $etime >> timer.log
