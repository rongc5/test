#/usr/bin/env python
#coding=utf-8


import urllib2, sys
import myLog



def do(path):
    req = urllib2.Request(path)
    html = urllib2.urlopen(req).read()
    logger.info("recv: %s", html)
    print html, path

logger = myLog.getLoger('log')
if __name__ == '__main__':
    if len(sys.argv) >= 2:
        #path = 'http://223.99.226.48/dsp.jsp?url=http://10.26.28.193:8888/click_url?title=123&creative_id=%s' % (sys.argv[1])
        pass
    #path = 'http://223.99.226.48/dsp.jsp?url=http://10.26.28.193:8888/click_url?title=title_488667&creative_id=488667'
    path = 'http://223.99.226.48/dsp.jsp?dsp_click_url=http://10.26.28.193:8888/click_url?title=title_386126&creative_id=386126'
    #path = 'http://10.26.28.193:8888/click_url?title=title_386126&creative_id=386126'
    #path = 'http://223.99.226.48/dsp.jsp?url=http%3A%2F%2F10.26.28.193%3A8888%2Fclick_url%3Ftitle%3Dtitle_746659%26creative_id%3D746659&chgt=0&usrid=0&ideaid=746659&t=http%3A%2F%2F10.26.28.193%3A8888%2Fclick_url%3Ftitle%3Dtitle_746659%26creative_id%3D746659&e=1&ot=0'
    do(path)
