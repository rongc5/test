#!/bin/bash

tvsniffer_path=`pwd`
#test script
sudo echo "start tvsniffer script ...">>$tvsniffer_path/tvsniffer_test.log
cd $tvsniffer_path;cd ../bin;
sudo echo "in path [`pwd`]  ...">>$tvsniffer_path/tvsniffer_test.log
sudo ./tvsniffer-alpha
sudo echo "started programing  ...">>$tvsniffer_path/tvsniffer_test.log

