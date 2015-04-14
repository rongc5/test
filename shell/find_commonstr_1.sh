#!/bin/bash
 
str2="howdy, world!";
 
str1="The world is changing everyday.";
 
len=${#str1}
 
[ ${#str1} -ge ${#str2} ] && len=${#str2}
 
for((i=len;i>0;i--))
 
do
 
   str=$(echo -e "$str1\n$str2" | sed -r 'N;s/.*(.{'$i'}).*\n.*\1.*/\1/;/\n/d')
 
    [ -n "$str" ] && break
 
done
 
echo "$str"

