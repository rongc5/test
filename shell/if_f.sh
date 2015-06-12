#!/bin/bash 

if [ -e tmp ]
then echo "/bin/bash is a file"
else echo "/bin/bash is not a file"
fi 
if :; then echo "always true"; fi

