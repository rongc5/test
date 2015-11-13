#!/bin/sh

machines='120.197.138.90 120.197.138.95'
for machine in $machines
do
        #echo ${machine} 
        nohup ./load_confs.sh asp ${machine} >/dev/null 2>&1 &
done

