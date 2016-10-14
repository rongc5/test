#!/bin/bash

awk -F' ' '{print $2}' | xargs -n1 python get_stock_urls.py -u
