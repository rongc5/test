#!/bin/bash

#author zhangming

if [ -z "$2" ]
then
	exit
fi


if [ -z "$1" ]
then
	exit
fi


APP=$1
HOST=$2

Machine=`echo "$2" | awk -F '.' '{printf("%s\n", $4)}'`

mkdir -p ${Machine}

scp -r ${HOST}:/app/ecom/${APP}/conf ./${Machine}
scp -r ${HOST}:/app/ecom/${APP}/data ./${Machine}
