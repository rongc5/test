#!/bin/bash

tmp=hello$(date +%F%r)
echo "$tmp"
mv job.sh  "$tmp"
