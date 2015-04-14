#!/bin/bash

var=ping www.baidu.com
if [$var]; 
then 
    echo "yes"
else
    echo "no"
fi
