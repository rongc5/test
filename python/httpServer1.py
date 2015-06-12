#/usr/bin/env python
#encoding=gbk

from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from SocketServer import ThreadingMixIn
import threading



pic = '''<?xml version="1.0" encoding="gbk"?>
<cpro>
    <adnum>1</adnum>
    <noad>0</noad>
    <ads>
        <ad>
            <material>
                <![CDATA[http://ubmcmm.baidustatic.com/media/v1/0f000rguXKkIAekXoyuAJf.jpg]]>
</material>
            <surl>
                <![CDATA[eyeis.cn]]>
</surl>
            <curl>
                <![CDATA[http://www.baidu.com/cpro.php?Ks0000jVHrWXJ8EcqWf-NDKvsQFYrqQJhlfCAf96h-suNiyWOl-XormS5Zw8SAk5k2Sgkq4evE7cFl85_Y-VIv0X1QRUOf0mQZehDfN-iZ4R
j8k_iNoKjgtRjLQm.7Y_iFYFxZzfTPyKh26Sj9CTEw6BCEuB6eL_3_ZKGyFWW3vUSh1GLtEZ-9k1QjPakkenh1cC.IgF_5y9YIZ0lQzqBUvqoQhN9TvqdQhPEUiqYmiq1pAqLQhYOugPGu0KYUHYkPjfvrjT10Zwd5H00Ivsqn6KWUMw8
5HnLnjbvnjm4g1PxmLKz0A7bmvk9TLnqn0K8mgPYTjYs0ZF15H00mvmqn0KWIAYqmh7GuZNmHRkamgP-0A49IZRqrfKGIA-b5H00pgwGUhuEpyfqn0KMTA-b5H00ThNhTA-b5H00mLKzUv-b5fKWUvdsUARqPfK9IA-b5fK9IZw45fKLT
LFW5H00Tvk1Thnqn0K1UAPo5H00Tvkopyfqn0KBIywMugwxug9spyfqn0K9pg0qnYNxugKsHLT0Iy-s5HuMyR6dT7RVNvVu0APC5fKYmgcqn6KbIZ0qn0K1uyk_ugFxpyfqnfKGTdq9TZ0qn0K9TZKxpyfqn0KWIgPY5fKGTdqLpgF-UA
N1T1Ys0ZI9T7qYXgK-5HD0TAsqn0KVm1YYuWcYrjT3n6KVIWY10A4vTjYsQW0snj0snj0s0AT45H00uh-zTLwxThNMpyq85HDk0ZP-mvq8u7qzuyIGUv3qnHTv0ZP1TjYk0ZP1Tjcqn6KYIZ0qnfKbpgPYTjYs0A7B5H00myw35H00TAu
YTh7buHYY0ZK1IZF9uARqPj030ZKGujYs0ZIspyfqn0K9uZw4TARqn0K1pgF-Uhw-TWYs0ZPLpyfqn0K1IZ-suHYs0APzm1YzPHDkn6K]]>
</curl>
            <width>468</width>
            <height>60</height>
        </ad>
    </ads>
</cpro>'''

txt = '''<?xml version="1.0" encoding="gbk"?>
<cpro xmlns="http://www2.baidu.com" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www2.baidu.com http://cpro.baidu.com/cpro.xsd">
    <adnum>2</adnum>
    <noad>0</noad>
    <ads>
        <ad>
            <id>129680274</id>
            <bid>千足金 赛车游戏 乐高 耳鸣</bid>
            <title>北京专业的耳鼻喉医院 [首大]</title>
            <desc>国内知名耳鼻喉品牌医院 首都耳鼻喉专科医院,专治耳鼻喉.</desc>
            <surl>www.bjshouda.com</surl>
            <curl>http://www.baidu.com/cpro.php?af0000KMu9UzJG7NjyDnFY2wPWFWDInnIK8L0cya3ga6eMgnIZaGMxbD2x5cDm0DbS-x8so4BIuV6hyEiw7tnyb4GuUn8f-7qTUf8P_583fRUfh_hju7yu8XZbTj.7b_iwdrsgnSNSyrG va4-DZjf6HvlPDZOfOxqXjHMEz4SAlZ5Yp2Ax63Ur2knTxA1xYvIrO-o4mTTzs1f_U_he_oR.IgF_5y9YIZ0lQzqBQhN9TvqdQhPEUiqYmiq1pAqLQhYOugPGu0KYUHYkPjfvrjT10Zwd5H00IvsqP6KWUMw85HnLnjbvnjm4g17xmLKz 0A7bmvk9TLnqn0K8mgPYTjYs0ZF15H00mvmqn0KWIAYqmh7GuZNjR7F5yDdn0A49IZRqrfKGIA-b5H00pgwGUhuEpyfqn0KMTA-b5H00ThNhTA-b5H00mLKzUv-b5fKWUvdsUARqPfK9IA-b5fK9IZw45fKLTLFW5HDsn0K1UZPzm1Ys0 ZP_mv_qn0K1UAVGujYs0AFduAI-I7q-XZKGujYs0A7GTjdZuvNnpb9vp0Kdpg0quMI7TjKyQRd4fHn0mv6q0Zw9TWYvP0KbIZ0qn0K1uyk_ugFxpyfqnfKGTdq9TZ0qn0K9TZKxpyfqn0KWIgPY5fKGTdqLpgF-UAN1T1Ys0ZI9T7qYXg K-5HD0TAsqn0KVm1Y1rjf4nW99rfKVIWd90A4vTjYsQW0snj0snj0s0AT45H00uh-zTLwxThNMpyq85HDd0ZP-mvq8u7qzuyIGUv3qnWn40ZP1TjYk0ZP1Tjcqn6KYIZ0qnfKbpgPYTjYs0A7B5H00myw35H00TAuYTh7buHYY0ZK1IZF 9uARqPj030ZKGujYs0ZIspyfqn0K9uZw4TARqn0K1pgF-Uhw-TWYs0ZPLpyfqn0K1IZ-suHYs0APzm1Y1PjTsn6K</curl>
        </ad>
        <ad>
            <id>182262973</id>
            <bid>千足金 赛车游戏 乐高 耳鸣</bid>
            <title>[北京军颐]长期失眠怎么办?</title>
            <desc>北京军颐医院高效治失眠 治失眠权威保障 失眠症在线咨询失眠权威专家亲诊</desc>
            <surl>wq3g.120bjjy.com</surl>
            <curl>http://www.baidu.com/cpro.php?af0000aMYZ9W3qDyhpn3FZEIhrx9R-tPHeWurD9N45tFpqCaH9QWJfsH9ThZpUUxOgqAcr9jBcG50dUZOzAwueeFBfNScacImxCu6Sd9KZELyupb4KTcHOrqO0M4.7R_iwECFuk3O2qM7 6wKAS6hjERlr6s2t--SgQCwdWh9mJCRnXZWeTrHl3TMd35ukdmTMMgb_Ez4IAZF4s8Y3TMdstxoxIsppotrH4N4QQt8gw9zx25H9zxQ6H7sEs45-eTrH4XkqKMjbSyZFvI5i_HZZk_LqPMHE4pSyZZn_nYQAO__LJ0.IgF_5y9YIZ0lQz qBQhN9TvqdQhPEUiqYmiq1pAqLQhYOugPGu0KYUHYkPjfvrjT10Zwd5H00IvsqP6KWUMw85HnLnjbvnjm4g17xmLKz0A7bmvk9TLnqn0K8mgPYTjYs0ZF15H00mvmqn0KWIAYqmh7GuZNjR7F5yDdn0A49IZRqrfKGIA-b5H00pgwGUhu Epyfqn0KMTA-b5H00ThNhTA-b5H00mLKzUv-b5fKWUvdsUARqPfK9IA-b5fK9IZw45fKLTLFW5HDsn0K1UZPzm1Ys0ZP_mv_qn0K1UAVGujYs0AFduAI-I7q-XZKGujYs0A7GTjdZuvNnpb9vp0Kdpg0quMI7TjKyQRd4fHn0mv6q0Zw9 TWYvP0KbIZ0qn0K1uyk_ugFxpyfqnfKGTdq9TZ0qn0K9TZKxpyfqn0KWIgPY5fKGTdqLpgF-UAN1T1Ys0ZI9T7qYXgK-5HD0TAsqn0KVm1YYnARLnHDzP0KVIWYk0A4vTjYsQW0snj0snj0s0AT45HD0uh-zTLwxThNMpyq85HDd0ZP-m vq8u7qzuyIGUv3qnWn40ZP1TjYk0ZP1Tjcqn6KYIZ0qnfKbpgPYTjYs0A7B5H00myw35H00TAuYTh7buHYY0ZK1IZF9uARqPj030ZKGujYs0ZIspyfqn0K9uZw4TARqn0K1pgF-Uhw-TWYs0ZPLpyfqn0K1IZ-suHYs0APzm1Yzn1R4rf K</curl>
        </ad>
    </ads>
</cpro>'''



class Handler(BaseHTTPRequestHandler):

    def log_request(self, code='-', size='-'):
        pass

    def do_GET(self):
        response = self.path
        self.send_response(200)
        self.send_header("Content-type", 'text/plain')
        self.end_headers()
        if not 'width' in self.path:
            self.wfile.write(txt)
        else:
            self.wfile.write(pic)


    def do_POST(self):

        self.send_response(200)
        self.send_header("Content-type", 'text/plain')
        self.end_headers()
        response = self.parse(self.path)
        if not 'width' in self.path:
            self.wfile.write(txt)
        else:
            self.wfile.write(pic)


class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""


def do():
    server = ThreadedHTTPServer((I, 8080), Handler)
    #server = HTTPServer(('localhost', 8080), Handler)
    print 'Starting server, use <Ctrl-C> to stop'
    server.serve_forever()

PORT = 8080
I="0.0.0.0"
if __name__ == '__main__':
    do()