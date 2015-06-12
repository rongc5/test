#!/bin/bash 
str1="NOT empty"
str2=""

if [[ -n $str1 ]] && [[ -z $str2 ]];
then
    echo str1 is non-empty and str2 is empty string.
fi
