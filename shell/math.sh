#!/bin/bash

echo "4 * 0.56" | bc  
   
no=54;  
result=`echo "$no * 1.5" | bc`  
echo $result  

result=`expr 3 + 4`  
echo $result
result=$(expr $no1 + 5) 
echo $result
