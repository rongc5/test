#!/usr/bin/env python
#encoding=utf-8

import json
import time
import pycurl
import cStringIO
import gzip
import unicodedata
import os
import sys
import random
import subprocess
import ast
import json
import StringIO

from time import strftime, localtime
from datetime import timedelta, date
import calendar

import ConfigParser

import optparse

__author__ = 'rong'

MAX_RESPONSE_KB = 10*1024

user_agent_list = [
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 "
        "(KHTML, like Gecko) Chrome/19.0.1061.0 Safari/536.3",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 "
        "(KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24"
]

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

class HTTPError(Exception):
    """Exception that is wrapped around all exceptions that are raised
    by the underlying fetcher when using the ExceptionWrappingFetcher

    @ivar why: The exception that caused this exception
    """
    def __init__(self, why=None):
        Exception.__init__(self, why)
        self.why = why

class CurlHTTPFetcher(object):
    """
    An C{L{HTTPFetcher}} that uses pycurl for fetching.
    See U{http://pycurl.sourceforge.net/}.
    """
    ALLOWED_TIME = 30 # seconds

    def _parseHeaders(self, header_file):
        header_file.seek(0)

        # Remove the status line from the beginning of the input
        unused_http_status_line = header_file.readline().lower ()
        if unused_http_status_line.startswith('http/1.1 100 '):
            unused_http_status_line = header_file.readline()
            unused_http_status_line = header_file.readline()

        lines = [line.strip() for line in header_file]

        # and the blank line from the end
        empty_line = lines.pop()
        if empty_line:
            raise HTTPError("No blank line at end of headers: %r" % (line,))

        headers = {}
        for line in lines:
            try:
                name, value = line.split(':', 1)
            except ValueError:
                raise HTTPError(
                    "Malformed HTTP header line in response: %r" % (line,))

            value = value.strip()

            # HTTP headers are case-insensitive
            name = name.lower()
            headers[name] = value

        return headers

    def fetch(self, url, body=None, headers=None):
        stop = int(time.time()) + self.ALLOWED_TIME
        off = self.ALLOWED_TIME

        if headers is None:
            headers = {}

        if  'User-Agent:' in headers or  'user-agent:' in headers:
            print 'no User-Agent', headers, url, sys._getframe().f_lineno
            return

        header_list = []
        if headers is not None:
            for header_name, header_value in headers.iteritems():
                header_list.append('%s: %s' % (header_name, header_value))

        c = pycurl.Curl()
        try:
            c.setopt(pycurl.NOSIGNAL, 1)

            if header_list:
                c.setopt(pycurl.HTTPHEADER, header_list)

            # Presence of a body indicates that we should do a POST
            if body is not None:
                c.setopt(pycurl.POST, 1)
                c.setopt(pycurl.POSTFIELDS, urllib.urlencode(body))

            while off > 0:
                if not url.startswith('http://') and not url.startswith('https://'):
                    raise HTTPError("Fetching URL not allowed: %r" % (url,))

                data = cStringIO.StringIO()
                def write_data(chunk):
                    if data.tell() > 1024*MAX_RESPONSE_KB:
                        return 0
                    else:
                        #print 'hello', data.getvalue().decode('gbk')
                        return data.write(chunk)

                response_header_data = cStringIO.StringIO()
                c.setopt(pycurl.WRITEFUNCTION, write_data)
                c.setopt(pycurl.HEADERFUNCTION, response_header_data.write)
                c.setopt(pycurl.TIMEOUT, off)
                c.setopt(pycurl.URL, url)

                c.perform()

                response_headers = self._parseHeaders(response_header_data)
                code = c.getinfo(pycurl.RESPONSE_CODE)
                if code in [301, 302, 303, 307]:
                    url = response_headers.get('location')
                    if url is None:
                        raise HTTPError(
                            'Redirect (%s) returned without a location' % code)

                    # Redirects are always GETs
                    c.setopt(pycurl.POST, 0)

                    # There is no way to reset POSTFIELDS to empty and
                    # reuse the connection, but we only use it once.
                else:
                    res = {}
                    res['head'] = response_headers
                    res['status'] = code
                    res['final_url'] = url
                    res['body'] = '%s' % (data.getvalue())
                    return res

                off = stop - int(time.time())

            raise HTTPError("Timed out fetching: %r" % (url,))
        finally:
            response_header_data.close()
            data.close()
            c.close()

def print_res(res, parser):
    if res.has_key('recode'):
        print res['recode']

    if res.has_key('data'):
        for item in res['data']:
            if item.has_key('id'):
                tmp_str = 'id='
                tmp_str += item['id']
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('finance'):
                tmp_str = 'finance='
                myjson = json.dumps(item['finance'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('quotation'):
                tmp_str = 'quotation='
                myjson = json.dumps(item['quotation'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('plate'):
                tmp_str = 'plate='
                myjson = json.dumps(item['plate'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('block'):
                tmp_str = 'block='
                myjson = json.dumps(item['block'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('addr'):
                tmp_str = 'addr='
                myjson = json.dumps(item['addr'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('single'):
                tmp_str = 'single='
                myjson = json.dumps(item['single'], ensure_ascii=False)
                tmp_str += myjson
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)

            if item.has_key('last_single'):
                tmp_str = 'last_single='
                print tmp_str
                if parser.file:
                    log_write(parser.file, tmp_str)
                for kk in item['last_single']:
                    myjson = json.dumps(kk, ensure_ascii=False)
                    print myjson
                    if parser.file:
                        log_write(parser.file, myjson)

            print '\n'
            if parser.file:
                log_write(parser.file, '\n')



def do_query_id(parser):
    url = 'http://%s:%s/queryid?id=%s&last_day_num=%d' % (parser.ip, parser.port, parser.id, parser.last_day_num)

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    header['User-Agent'] = user_agent_list[index]

    res = {}
    try:
        curl =  CurlHTTPFetcher()
        curl.ALLOWED_TIME = 2
        res = curl.fetch(url, None, header)
    except BaseException, e:
        print e.message

    if not res.has_key('body'):
        print 'net not ok'
        return
    res_str = res['body'].strip()
    res_str = res_str.decode('gb18030')
    #res_str = json.loads(res_str, encoding="GB2312")
    res_str = json.loads(res_str)
    if res_str:
        print print_res(res_str, parser)


# http://www.iwencai.com/unifiedwap/unified-wap/v2/result/get-robot-data
# post
#http://www.iwencai.com/gateway/urp/v7/landing/getDataList

def POST():
    url = 'http://www.iwencai.com/unifiedwap/unified-wap/v2/result/get-robot-data'

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    #header['User-Agent'] = user_agent_list[index]
    header['User-Agent'] = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36'
    header["Referer"] = "http://www.iwencai.com/unifiedmobile/?q=%E4%BA%BA%E6%B0%94%E6%8E%92%E5%90%8D";
    header["Origin"] = "http://www.iwencai.com";
    header['Host'] = 'www.iwencai.com'
    header['Cookie'] = 'chat_bot_session_id=b4bb9a3e04caf390556039de461d23bd; other_uid=Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69; ta_random_userid=zd909m2ebs; cid=b5c74e62f7c2895b6e31ba69d049fb101669001849; THSSESSID=2b5db7769f0df582059c5d2cbc; v=Ax3j3JYwIseH9sbrAl0b7ROPKvISOlGMW261YN_iWXSjljMsp4phXOu-xTVs'
    header['DNT'] = '1'
    header['hexin-v']= 'Ax3j3JYwIseH9sbrAl0b7ROPKvISOlGMW261YN_iWXSjljMsp4phXOu-xTVs'
    bodys='question=2022%E5%B9%B411%E6%9C%8813%E6%97%A5%E4%BA%BA%E6%B0%94%E6%8E%92%E5%90%8D&source=ths_mobile_iwencai&user_id=0&user_name=0&version=2.0&secondary_intent=&add_info=%7B%22urp%22%3A%7B%22scene%22%3A3%2C%22company%22%3A1%2C%22business%22%3A1%7D%2C%22contentType%22%3A%22json%22%7D&log_info=%7B%22input_type%22%3A%22click%22%7D&rsh=Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69'

    header_list = []
    for header_name, header_value in header.iteritems():
        header_list.append('%s: %s' % (header_name, header_value))


    crl = pycurl.Curl()
    crl.setopt(pycurl.VERBOSE,1)
    crl.setopt(pycurl.FOLLOWLOCATION, 1)
    crl.setopt(pycurl.MAXREDIRS, 5)
#crl.setopt(pycurl.AUTOREFERER,1)
    crl.setopt(pycurl.CONNECTTIMEOUT, 60)
    crl.setopt(pycurl.TIMEOUT, 300)

    if header_list:
        crl.setopt(pycurl.HTTPHEADER, header_list)

#crl.setopt(pycurl.PROXY,proxy)
    crl.setopt(pycurl.HTTPPROXYTUNNEL,1)
#crl.setopt(pycurl.NOSIGNAL, 1)
    crl.fp = StringIO.StringIO()
# Option -d/--data <data>  HTTP POST data
    crl.setopt(crl.POSTFIELDS, bodys)
    crl.setopt(pycurl.URL, url)
    crl.setopt(crl.WRITEFUNCTION, crl.fp.write)
    crl.perform()
    print crl.fp.getvalue()

def do_select():

    #url='http://apitest.99shiji.com/api/v2/ad/ads?p=E02CD479E16061C96F604F15485BB63FFFAEC6DE050DFC1D3BEE816813C16F1E8B0E11A356C99161067922E41FCA85B4569D74DA40B9D0595B5AC833D3120A8E14898D8EAF8BAEC745ED177B7EAEC8B6ADD6777508DB8325AA280CEE0A8C7C4A8769137F40A85F7E06C978FF79CAB2BFDDECAE4D246216E7E6084CEE974D8DB507F782B8E3C3A691EA87BB7CB40DC048A860717B8A468D5204B0630643D888A67042DF83FB9FBE90AB9ACB031401DCE63A845B664DF362360A92416397C58F2992EFCCBD8E18BFAF495766853E8DFCFF5B4DE12C19C6E125D47D86301B437A01'
    #url = 'http://readfree.zhulang.com/v1/free/page?channel_id=2&tab_key=mfzq&offset=40&limit=10&version=1'
    url = 'http://www.iwencai.com/unifiedwap/unified-wap/v2/result/get-robot-data'

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    #header['User-Agent'] = user_agent_list[index]
    header['User-Agent'] = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36'
    header["Referer"] = "http://www.iwencai.com/unifiedmobile/?q=%E4%BA%BA%E6%B0%94%E6%8E%92%E5%90%8D";
    header["Origin"] = "http://www.iwencai.com";
    header['Host'] = 'www.iwencai.com'
    header['Cookie'] = 'chat_bot_session_id=b4bb9a3e04caf390556039de461d23bd; other_uid=Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69; ta_random_userid=zd909m2ebs; cid=b5c74e62f7c2895b6e31ba69d049fb101669001849; THSSESSID=2b5db7769f0df582059c5d2cbc; v=A6pU9W1RvR-_JzFzl65UpLge_RtJGy_hIJqiBjRzVomIe0TFHKt-hfAv8jcH'
    header['DNT'] = '1'
    header['hexin-v']= 'A6pU9W1RvR-_JzFzl65UpLge_RtJGy_hIJqiBjRzVomIe0TFHKt-hfAv8jcH'

#question: 2022年11月13日人气排名
#source: ths_mobile_iwencai
#user_id: 0
#user_name: 0
#version: 2.0
#secondary_intent:
    #add_info: {"urp":{"scene":3,"company":1,"business":1},"contentType":"json"}
    #log_info: {"input_type":"typewrite"}
    #rsh: Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69
    bodys = ''
    bodys='question=2022%E5%B9%B411%E6%9C%8813%E6%97%A5%E4%BA%BA%E6%B0%94%E6%8E%92%E5%90%8D&source=ths_mobile_iwencai&user_id=0&user_name=0&version=2.0&secondary_intent=&add_info=%7B%22urp%22%3A%7B%22scene%22%3A3%2C%22company%22%3A1%2C%22business%22%3A1%7D%2C%22contentType%22%3A%22json%22%7D&log_info=%7B%22input_type%22%3A%22typewrite%22%7D&rsh=Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69'

    print bodys
    res = {}
    try:
        curl =  CurlHTTPFetcher()
        #curl.ALLOWED_TIME = 15
        res = curl.fetch(url, bodys, header)
    except BaseException, e:
        print e.message

    print res
    #res_str = res['body'].strip()
    #if res_str:
        #print res_str


if __name__ == '__main__':
    #do_select()
    POST()
