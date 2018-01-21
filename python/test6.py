#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys


if '__main__' == __name__:
    url = 'https://www.google.com/search?ei=BRViWsbHK4SGjwOZk5SYDA&q=linux+c+http+url+%E8%A7%A3%E6%9E%90&oq=linux+c+http+url+%E8%A7%A3%E6%9E%90&gs_l=psy-ab.3...8828.13164.0.13484.10.10.0.0.0.0.349.1854.2-3j3.6.0....0...1c.1.64.psy-ab..4.3.964...0j0i13k1.0.sO6yNCKSApI'
    items = url.split("://")
    print items

    items = url.split("qqqqqq");
    print items

    str = "hthth"
    str1 = str.strip("h")
    print str1
