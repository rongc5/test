#!/bin/bash
fname()
{
    echo $1, $2;
    echo "$@";
    echo "$*"
    return 0
}

read arg1 arg2;
fname $arg1 $arg2

