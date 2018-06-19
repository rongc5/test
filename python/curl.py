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
                c.setopt(pycurl.POSTFIELDS, body)

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


def do_select():

    url='http://apitest.99shiji.com/api/v2/ad/ads?p=E02CD479E16061C96F604F15485BB63FD815D772046D83879C98EF8B6C66DE909E8F32C3E1479C30D55EDD4D7C7925A9C267FF1AA558D366FDBFBE38DCC4E90043F8593BBB738484C0F0DE26CFB03F747587BF06762782694B9BD32D4EB40C088AD4817EC4DB3E49E7BE5F8755DF5E5E3E7264764BAC479DB4D0D015F0E419DC23CC1AEC82F964560860376A9F9015F6E1230C5E69E42DA8D1B43D4A36942FEC2B2F9359C19C0A27D7635995E5866F0E82F19F638F4F0A438CB5DF0486E260A8FE6E35BD2E7054467FF1B031BC4225A31DE5C05BE2711C2B71A70AE682411B25'

    header = {}
    index = random.randint(0, len(user_agent_list) -1)
    #header['User-Agent'] = user_agent_list[index]
    header['User-Agent'] = 'Hisense__Hisense F20T__5.1.1'
    header["appid"] = "APP17101967332747";
    header["version"] = "2.0";

    try:
        curl =  CurlHTTPFetcher()
        curl.ALLOWED_TIME = 2
        res = curl.fetch(url, None, header)
    except BaseException, e:
        print e.message

    res_str = res['body'].strip()
    if res_str:
        print res_str


if __name__ == '__main__':
    do_select()
