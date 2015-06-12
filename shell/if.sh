#!/bin/bash 
echo "请输入文件名:"
read var
if [ -e $var ]; then
    echo File exists
else
    echo Does not exists
fi
