#!/bin/bash

a="111 222 333 444"
b="111 333"
null=""

for i in $b
do
  a="${a//$i/$null}"
done

c="$a"

echo $c
