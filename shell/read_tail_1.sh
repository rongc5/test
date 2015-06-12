#!/bin/bash


#tail -10 press.log | tac | while read line
val=10
list=accept
touch accept.log
echo -e "\n\n\n\n\n\n" >> accept.log
prefix="new note start at $(date)"
printf "$prefix\t OK\n" >> accept.log

while [ "1" = "1" ]
do 
    tail -$val press.log | tac | cat | while read line
do
    ret=$(echo $line | grep accept | wc -l)
    if [ $ret -eq 1 ]
    then  date >> accept.log
        echo $line >> accept.log
        break
    fi
done
sleep 60
done


