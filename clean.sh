#!/bin/sh 

array="algorithms  asm  c  c++  design  java  lecture lua  python  regex  zhangming linux gtest"
#for var in ${var[@]}
for var in $array
do
    #echo "dir ==> "$var
    #find $var -type f -exec perl -e 'print (-B $_ ? "$_\n" : "" ) for @ARGV' {} + | grep -v "*.c" | grep -v "*.cpp" | xargs rm -f
    file=`find $var -type f -exec perl -e 'print (-B $_ ? "$_\n" : "" ) for @ARGV' {} + `
    if [ -n "$file" ]
    then
        echo "$file"
        rm -f "$file"
    fi

done
