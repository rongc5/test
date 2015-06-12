#! /bin/bash
 

 
str1="howdy, world!"
 
str2="The world is changing everyday."
 
len=${#str1}
 

 
for((i=$len;i>0;i--))
 
do
 
        k=$((len-i))
 
        for((j=0;j<=$k;j++))
 
        do
 
                if echo "$str2" | grep -qF "${str1:$j:$i}"
 
                then
 
                       echo "max str is '${str1:$j:$i}'"
 
                        exit
 
                fi
        done
 
done
 

 
echo "no max str"

