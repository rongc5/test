#!/usr/bin/env python
#coding=utf-8

import json
import re
import execjs
import requests
import requests.utils
import  urllib
import sys
import io
import wencai as wc

## Github地址：https://github.com/zsrl/pywencai
## PyPI地址：https://pypi.org/project/pywencai/
##
if __name__ == '__main__':
    wc.set_variable(cn_col=True)
    wc.search(sys.argv[1])
