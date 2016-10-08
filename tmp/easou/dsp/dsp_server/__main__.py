#/usr/bin/env python
#coding=utf-8

from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from SocketServer import ThreadingMixIn
import threading
import easou_adx_pb2
import urlparse, sys,base64
import myLog
from utils import *
from Crypto.Cipher import AES
from urllib import quote, unquote
import random



class Handler(BaseHTTPRequestHandler):

    def log_request(self, code='-', size='-'):
        pass

    def deal_req_ads(self, bid_req_str):

        bid_req = easou_adx_pb2.BidRequest()
        bid_req.ParseFromString(bid_req_str)

        bid_res = easou_adx_pb2.BidResponse()
        bid_res.bid = bid_req.bid

        count = 0
        for adzinfo in bid_req.adzinfo:
            ads = bid_res.ads.add()
            ads.adzinfo_id = int(adzinfo.pid)
            ads.view_type = int(adzinfo.view_type[count])
	    ads.creative_type = 1
	    count = count + 1

            for i in range(adzinfo.ad_bid_count):
                adps = ads.adps.add()
                id = random.randint(1000, 1000000)
                adps.creative_id  = str(id)
                adps.title = 'title_%d' % (id)
                adps.desc = 'desc_%d' % (id)
                adps.click_url = 'http://%s:%s/click_url?title=%s&creative_id=%s' % (IP, PORT, adps.title, adps.creative_id)
                adps.show_url = 'http%3A%2F%2Fqd.shangjijm.com%2F'
                adps.img_url = 'http://ubmcmm.baidustatic.com/media/v1/0f000rguXKkIAekXoyuAJf.jpg'

                adps.show_mon_url = 'http://%s:%s/showmonUrl.html?id=%s&creative_id=%s' % (IP, PORT, bid_res.bid, adps.creative_id)
                adps.open_type = 1
                adps.support_os = 3
                adps.ad_user = 'zhangming'
                ratio = random.randint(1, 10)
                adps.max_cpm_price = adzinfo.min_cpm_price * ratio
                logger.info('deal_req_ads: creative_id: %s max_cpm_price: %u', adps.creative_id, adps.max_cpm_price)
                #print('deal_req_ads: creative_id: %s max_cpm_price: %u', adps.creative_id, adps.max_cpm_price)
            ads.winnotice_url = 'http://%s:%s/winnoticeAds.html?price=DONE_PRICE&id=REQ_ID&creative_id=CREATIVE_ID' % (IP, PORT)



        bid_res_str = bid_res.SerializeToString()
        self.wfile.write(bid_res_str)

    def deal_win_notice(self, query):
        price = unquote(query['price'][0])
        key = '123456789' + 7 * '\x00'
        iv = 16 * '\x00'
        aes = AES.new(key,AES.MODE_CBC, iv)
        price_str = aes.decrypt(base64.decodestring(price))
        sid = query['id'][0]
        creative_id = query['creative_id'][0]

        logger.info('recv winnotice_url: sid: %s creative_id: %s price:%s', sid, creative_id, price_str)


    def deal_showmon_url(self, query):
        sid = query['id'][0]
        creative_id = query['creative_id'][0]
        logger.info('recv show mon url: sid: %s creative_id: %s', sid, creative_id)


    def deal_click_url(self, query):
        creative_id = query['creative_id'][0]
        title = query['title'][0]
        self.wfile.write("hello world")
        logger.info('recv click_url: creative_id: %s title: %s', creative_id, title)

    def deal_req_adview(self, query):
        adstr = '''{
            "res": 1,
            "co": 1,
            "mg": "",
            "ad": [
                    {
                        "abi": "",
                        "ast": "一款你不得不玩的游戏",
                        "adi": "20150528-135744_zceCCbKv_61-357-U9fM-1_1",
                        "aic": "",
                        "ate": "萌江湖创造手游新奇迹",
                        "api": [
                                "http://test2014.adview.cn:8088/agent/image/zceccbkv20150526164937501000_640_320.jpg"
                                ],
                                "apn": "",
                                "ati": "萌江湖重磅上市",
                                "dpn": "",
                                "at": 15,
                                "dai": "",
                                "al": "http://www.baidu.com",
                                "das": "",
                                "dan": "",
                                "act": 2,
                                "ec": ["url1","url2"],
                                "es": { "0": ["url3","url4"]}
                    }
                    ]
                    }'''

        self.wfile.write(adstr)


    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", 'text/plain')
        self.end_headers()

        parsed_path = urlparse.urlparse(self.path)
        print parsed_path
        if '/requestAds.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_req_ads(base64.decodestring(unquote(res['req'][0])))

        elif '/winnoticeAds.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_win_notice(res)

        elif '/showmonUrl.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_showmon_url(res)
        elif '/click_url' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_click_url(res)
        elif '/requestAdViewAds.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_req_adview(res)






    def do_POST(self):

        self.send_response(200)
        self.send_header("Content-type", 'text/plain')
        self.end_headers()



class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""

logger = myLog.getLoger('dsp_server')

def do():
    server = ThreadedHTTPServer((IP, PORT), Handler)
    #server = HTTPServer(('localhost', 8080), Handler)
    print 'Starting server, use <Ctrl-C> to stop'
    server.serve_forever()

PORT = 8888
IP="10.26.28.193"
if __name__ == '__main__':
    daemon = Daemon()
    daemon.daemonize()
    do()
