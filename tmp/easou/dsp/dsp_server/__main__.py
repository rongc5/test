#/usr/bin/env python
#coding=utf-8

from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from SocketServer import ThreadingMixIn
import threading
import easou_adx_pb2
import urlparse, sys,base64
import myLog
from Crypto.Cipher import AES
from urllib import quote, unquote



class Handler(BaseHTTPRequestHandler):

    def log_request(self, code='-', size='-'):
        pass

    def deal_req_ads(self, bid_req_str):
        print bid_req_str

        bid_req = easou_adx_pb2.BidRequest()
        bid_req.ParseFromString(bid_req_str)

        bid_res = easou_adx_pb2.BidResponse()
        bid_res.bid = bid_req.bid




        for adzinfo in bid_req.adzinfo:
            ads = bid_res.ads.add()
            ads.adzinfo_id = adzinfo.pid
            ads.view_type = adzinfo.view_type

            for i in range(adzinfo.ad_bid_count):
                adps = ads.adps.add()
                adps.creative_id  = 9295120
                adps.title = '加盟福祺道烤鱼，赚翻了'
                adps.desc = '创业成功不是梦，从加入我们开始'
                adps.click_url = 'http%3A%2F%2Fqd.shangjijm.com%2Ffuqidao%2Fqd5.html'
                adps.show_url = 'http%3A%2F%2Fqd.shangjijm.com%2F'
                adps.img_url = 'http://a.hiphotos.baidu.com/zhidao/pic/item/f9dcd100baa1cd11aa2ca018bf12c8fcc3ce2d74.jpg'

                adps.show_mon_url = 'http://%s:%s/showmonUrl.html?id=%s&creative_id=%s' % (IP, PORT, bid_res.bid, adps.creative_id)
                adps.open_type = 1
                adps.support_os = 3
                adps.ad_user = 'zhangming'
                adps.max_cpm_price = 3900
                adps.winnotice_url = 'http://%s:%s/winnoticeAds.html?price=PRICE&id=ID&creative_id=CREATIVE_ID' % (IP, PORT)



        bid_res_str = bid_res.SerializeToString()
        self.wfile.write(bid_res_str)

    def deal_win_notice(self, query):
        price = str(query['price'][0])
        aes = AES.new('123456789',AES.MODE_CBC, '000000000')
        price_str = aes.decrypt(base64.decodestring(price))
        sid = query['id'][0]
        creative_id = query['creative_id'][0]

        logger.debug('recv winnotice_url: sid: %s creative_id: %s price:%s', sid, creative_id, price_str)


    def deal_showmon_url(self, query):
        sid = query['id'][0]
        creative_id = query['creative_id'][0]
        logger.debug('recv show mon url: sid: %s creative_id: %s', sid, creative_id)

    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", 'text/plain')
        self.end_headers()

        parsed_path = urlparse.urlparse(self.path)
        print parsed_path
        if '/requestAds.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)

            self.deal_req_ads(unquote(res['req'][0]))

        elif '/winnoticeAds.html' in parsed_path.path:
            self.deal_win_notice(parsed_path.query)

        elif '/showmonUrl.html' in parsed_path.path:
            self.deal_showmon_url(parsed_path.query)





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

PORT = 8980
IP="0.0.0.0"
if __name__ == '__main__':
    #daemon = Daemon()
    #daemon.daemonize()
    do()
