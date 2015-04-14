#!/bin/bash
start=$(date +%s)
sleep 10
echo "ls -l"
end=$(date +%s)
difference=$((end - start))
echo time taken to excute ls -l is $difference seconds
