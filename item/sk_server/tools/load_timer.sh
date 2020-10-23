#!/bin/bash
. /etc/profile
. ~/.bash_profile


##返回这个脚本文件放置的目录

stime=$(date "+%Y-%m-%d %H:%M:%S")

cd `dirname $0`
path=`pwd`
echo "start", $path, $stime >> timer.log


python load_ids.py
python load_finance.py
python load_brief.py
python load_plate.py

cp ../data/addr* ~/sk_ser/data/
cp ../data/plate_* ~/sk_ser/data/
cp ../data/finance_dict ~/sk_ser/data/
cp ../data/code_all ~/sk_ser/data/


etime=$(date "+%Y-%m-%d %H:%M:%S")
echo "end", $path, $etime >> timer.log
