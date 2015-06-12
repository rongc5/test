#!/bin/bash

check_path=`pwd`
names=whilst2014
m_file=Tarlog
check_name_num=`ls -l $check_path | grep $names | wc -l`
if [ `ls -l $check_path| grep $m_file | grep '^d'| wc -l` -eq 0 ];then
	mkdir $m_file
fi

for ((i=1;i<=$check_name_num;i++))
do
check_name=`ls -l $check_path | grep $names | awk '{print $NF}'|sed -n $i'p' `
	echo "$check_name"
tar czvf $check_name.tgz $check_name
mv $check_name.tgz $m_file
rm -rf $check_name	
done
