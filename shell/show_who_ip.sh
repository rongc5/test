#!/bin/sh
IP=$(ifconfig eth0 | awk ' /inet addr:/  { print $2 } ' | cut -c6- )
if test -z "$IP"
then
        IP=$(hostname | awk -F. ' { print $1 } ')
fi
export PS1="[\u@$IP \w\$]"

