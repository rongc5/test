#!/usr/bin/env python3
#coding=utf-8

import urllib2

# http://isub.snssdk.com/2/data/v2/post_message/?iid=5405265729&os_version=10.0.2&os_api=18&app_name=joke_essay&channel=App%20Store&device_platform=iphone&idfa=8E5048E4-1CF8-48BF-B95B-FF9841387C79&live_sdk_version=130&vid=AF866ED9-493A-429E-887B-C7663976F348&openudid=a39030210aeba8c216881304865bf0e9065f4e42&device_type=iPhone%206%20Plus&version_code=5.5.5&ac=WIFI&screen_width=1242&device_id=9514110558&aid=7
def do():
    url = 'http://isub.snssdk.com'
    ua = 'Joke/5.5.5 (iPhone; iOS 10.0.2; Scale/3.00)'
    values = {'name' : 'who','password':'123456'}
    header = {'''Host: isub.snssdk.com
Content-Type: application/x-www-form-urlencoded
Cookie: install_id=5405265729; ttreq=1$12cc375b1ef925f058ba204d54bf11bb30ef75a8; qh[360]=1; login_flag=ab711bc63e9df8c22921971513379c65; sessionid=b7785ddf3aaf10a839a35f5749c892c6; sid_guard="b7785ddf3aaf10a839a35f5749c892c6|1475450798|2592000|Tue\054 01-Nov-2016 23:26:38 GMT"; sid_tt=b7785ddf3aaf10a839a35f5749c892c6; _ga=GA1.2.509035857.1468497336; uuid="w:7a70e6822a8949068f0f5bacfa5c3cf3"
Connection: keep-alive
Connection: keep-alive
Accept: */*
User-Agent: Joke/5.5.5 (iPhone; iOS 10.0.2; Scale/3.00)
Accept-Language: zh-Hans-CN;q=1
Content-Length: 68
Accept-Encoding: gzip, deflate'''}





if __name__ == '__main__':
    pass

