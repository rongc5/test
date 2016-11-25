#/usr/bin/env python
#coding=utf-8

from SocketServer import ThreadingMixIn
from Queue import Queue
import threading, socket

from SimpleHTTPServer import SimpleHTTPRequestHandler
from SocketServer import TCPServer
import urlparse

import os,sys, time

class Daemon:
        """
        A generic daemon class.

        Usage: subclass the Daemon class and override the run() method
        """
        def __init__(self, stdin='/dev/null', stdout='/dev/null', stderr='/dev/null'):
                self.stdin = stdin
                self.stdout = stdout
                self.stderr = stderr


        def daemonize(self):
                try:
                        pid = os.fork()
                        if pid > 0:
                                # exit first parent
                                sys.exit(0)
                except OSError, e:
                        sys.stderr.write("fork #1 failed: %d (%s)\n" % (e.errno, e.strerror))
                        sys.exit(1)

                #os.chdir("/")
                os.setsid()
                os.umask(0)

                # do second fork
                try:
                        pid = os.fork()
                        if pid > 0:
                                # exit from second parent
                                sys.exit(0)
                except OSError, e:
                        sys.stderr.write("fork #2 failed: %d (%s)\n" % (e.errno, e.strerror))
                        sys.exit(1)

                # redirect standard file descriptors
                sys.stdout.flush()
                sys.stderr.flush()
                si = file(self.stdin, 'r')
                so = file(self.stdout, 'a+')
                se = file(self.stderr, 'a+', 0)
                os.dup2(si.fileno(), sys.stdin.fileno())
                os.dup2(so.fileno(), sys.stdout.fileno())
                os.dup2(se.fileno(), sys.stderr.fileno())


class ThreadPoolMixIn(ThreadingMixIn):
    '''
    use a thread pool instead of a new thread on every request
    '''
    numThreads = 3
    allow_reuse_address = True  # seems to fix socket.error on server restart

    def serve_forever(self):
        '''
        Handle one request at a time until doomsday.
        '''
        # set up the threadpool
        self.requests = Queue(self.numThreads)

        for x in range(self.numThreads):
            t = threading.Thread(target = self.process_request_thread)
            t.setDaemon(1)
            t.start()

        # server main loop
        while True:
            self.handle_request()

        self.server_close()

    
    def process_request_thread(self):
        '''
        obtain request from queue instead of directly from server socket
        '''
        while True:
            ThreadingMixIn.process_request_thread(self, *self.requests.get())

    
    def handle_request(self):
        '''
        simply collect requests and put them on the queue for the workers.
        '''
        try:
            request, client_address = self.get_request()
        except socket.error:
            return
        if self.verify_request(request, client_address):
            self.requests.put((request, client_address))


thread_pv = {}

class Handler(SimpleHTTPRequestHandler):

    def deal_req_adview(self, query):
        #adstr = '''{"ast":"","adLogo":"http:\/\/rm.lomark.cn\/upload\/lomark_ad_logo.png","dai":"","dan":"","act":1,"ate":"","adIcon":"http:\/\/bid.adview.cn\/images\/adicon.png","das":0,"ati":"","api":["http:\/\/rm.lomark.cn\/Upload\/AdMaterials\/E9F8904500944558A189E27C3109C3B5_640X100.jpg"],"ec":["http:\/\/open.adview.cn\/agent\/openClick.do?st=0&uuidEncType=0&sv=0&src=79&sy=0&nt=&adi=20161125-181342_7432640100_165-1856-FXqC-1073_1&bi=com.esbook.reader&ai=xfX5mlgBAABeIFhFWyE7efwlRFdCh7aCGek6lxdbvllhRM4XI__jTq_nqOgX-mPnTMq7n4hEPw-_pFBbsp90Nxt_NDhAzjPbMDrHLgal4d0aTJ827xM1a_JrOWf0HozME3m1yW71BWucFiyyFBxnd4G8BsHuvIINRHawnGT34yZtC48sG9kfqOllVn-zyR4k9OrAhx7Ha-fkDe7pJH7an4KT04iy7iSv39icGNEUjWDrejjJIV0v62oEDiP_ysOWWQ&ud=868612023497673&andt=0&as=0x0&se=&cv=&rqt=1&ti=1480068827&tm=0&to=a27978777f1df8fefe2185972aeb15f4&aid=SDK201610261009459kupt7lxc97hj29&ro=1&ca=0","http:\/\/114.55.152.60\/clk?p=100011&r=0_0_0&id=20161125-181342_reqimp_165-6527-18te-867&aid=7432&area=%E5%B9%BF%E4%B8%9C%E7%9C%81_%E6%B7%B1%E5%9C%B3%E5%B8%82_%E6%9C%AA%E7%9F%A5&key=3B601AC4301941FDBA5F79A1588194E9&paymodeltype=2&netaddr=112_97_48_133&appid=0e5c2f23a2394e8851775eda92a475bf&appname=%E5%AE%9C%E6%90%9C%E5%B0%8F%E8%AF%B4%E5%AE%89%E5%8D%93&uuid=752b178ab5a1d4003151fd4e10ad8bb61820e69e&operator=4&net=3&client=1&osv=4.4.3&adspacetype=1&devicetype=1&device=&lat=0&long=0&quality=0&hd=100011_2_3_752b178ab5a1d4003151fd4e10ad8bb61820e69e&iptype=1&at=1&cate=3&t=1480069122&mediatype=1&sign=5c4acc0af26970b173e9e67366234c95"],"dpn":"","adi":"20161125-181342_7432640100_165-1856-FXqC-1073_1","abi":"","al":"http:\/\/hn-site.lomark.cn\/gz\/amyPro\/201611\/gfyh006\/index.html?lextp=NzQzMnwxMDAwMTE=","es":{"0":["http:\/\/open.adview.cn\/agent\/openDisplay.do?st=0&uuidEncType=0&sv=0&src=79&sy=0&nt=&adi=20161125-181342_7432640100_165-1856-FXqC-1073_1&bi=com.esbook.reader&ai=xfX5mlgBAABeIFhFWyE7efwlRFdCh7aCGek6lxdbvllhRM4XI__jTq_nqOgX-mPnTMq7n4hEPw-_pFBbsp90Nxt_NDhAzjPbMDrHLgal4d0aTJ827xM1a_JrOWf0HozME3m1yW71BWucFiyyFBxnd4G8BsHuvIINRHawnGT34yZtC48sG9kfqOllVn-zyR4k9OrAhx7Ha-fkDe7pJH7an4KT04iy7iSv39icGNEUjWDrejjJIV0v62oEDiP_ysOWWQ&ud=868612023497673&andt=0&as=0x0&se=&cv=&rqt=1&ti=1480068827&tm=0&to=a27978777f1df8fefe2185972aeb15f4&aid=SDK201610261009459kupt7lxc97hj29&ro=1&ca=0","http:\/\/114.55.152.60\/imp?p=100011&r=0_0_0&id=20161125-181342_reqimp_165-6527-18te-867&aid=7432&area=%E5%B9%BF%E4%B8%9C%E7%9C%81_%E6%B7%B1%E5%9C%B3%E5%B8%82_%E6%9C%AA%E7%9F%A5&key=3B601AC4301941FDBA5F79A1588194E9&paymodeltype=2&netaddr=112_97_48_133&appid=0e5c2f23a2394e8851775eda92a475bf&appname=%E5%AE%9C%E6%90%9C%E5%B0%8F%E8%AF%B4%E5%AE%89%E5%8D%93&uuid=752b178ab5a1d4003151fd4e10ad8bb61820e69e&operator=4&net=3&client=1&osv=4.4.3&adspacetype=1&devicetype=1&device=&lat=0&long=0&quality=0&hd=100011_2_3_752b178ab5a1d4003151fd4e10ad8bb61820e69e&iptype=1&at=1&cate=3&t=1480069122&mediatype=1&sign=5c4acc0af26970b173e9e67366234c95"]},"as":"320x50","at":0}],"mg":"","co":1}'''

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

        thread = threading.current_thread()
        print thread.getName()

        pv = 0
        if thread_pv.has_key(thread.getName()):
            pv = thread_pv[thread.getName()]



        if pv % 10 < 1:
            time.sleep(0.1)
            print pv, 'time.sleep(0.1)'
        elif pv % 10 >= 4:
            time.sleep(0.35)
            print pv, 'time.sleep(0.35)'
        else:
            time.sleep(0.2)
            print pv, 'time.sleep(0.2)'

        thread_pv[thread.getName()] = pv + 1
        parsed_path = urlparse.urlparse(self.path)
        print self.path


        if '/requestAdViewAds.html' in parsed_path.path:
            res = urlparse.parse_qs(parsed_path.query)
            self.deal_req_adview(res)
        elif '/adview_ec.jsp' in parsed_path.path or '/adview_es.jsp' in parsed_path.path:
	        self.wfile.write(self.path)



if __name__ == '__main__':

    daemon = Daemon()
    #daemon.daemonize()

    class ThreadedServer(ThreadPoolMixIn, TCPServer):
        pass

    def test(HandlerClass = Handler,
            ServerClass = ThreadedServer, 
            protocol="HTTP/1.0"):
        '''
        Test: Run an HTTP server on port 8002
        '''

        port = 8002
        server_address = ('', port)

        HandlerClass.protocol_version = protocol
        httpd = ServerClass(server_address, HandlerClass)

        sa = httpd.socket.getsockname()
        print "Serving HTTP on", sa[0], "port", sa[1], "..."
        httpd.serve_forever()

    test()
