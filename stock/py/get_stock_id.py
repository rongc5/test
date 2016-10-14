#!/usr/bin/env python2
#encoding=gbk

import urllib2

import utils
#from html.parser import HTMLParser
import HTMLParser
import getopt, sys, time, os, re

sfalg =False
class MyHTMLParser(HTMLParser.HTMLParser):

    def __init__(self, bk_links):
        HTMLParser.HTMLParser.__init__(self)
        self.bk_links  = bk_links
        self.flag = False
        self.href = ''

    def handle_starttag(self, tag, attrs):
        #print "Encountered the beginning of a %s tag" % tag
        if tag == "a":
            if len(attrs) == 0:
                pass
            else:
                for (variable, value) in attrs:
                    if variable == "href":
                        res = re.search(r'[0-9]{6,}', value)
                        if res:
                            self.href = value
                            self.flag = True

    def handle_data(self,data):
        if self.flag:
            self.bk_links[data] = self.href
            self.flag = False
            self.href = ''


def get_stock_url(filename, url):
    res = urllib2.urlopen(url).read()
    #print res
    bk_links = {}
    hp = MyHTMLParser(bk_links)
    hp.feed(res)
    hp.close()

    file = open(filename, "w+")

    for key in bk_links:
        if sfalg:
            print key, bk_links[key]

        res = re.search(r'[0-9]{6,}', bk_links[key])
        if res:
            stockid = res.group(0)
            #sb = '%s\t%s\t%s%s\n' % (stockid, key, host, bk_links[key])
            sb = '%s\t%s\n' % (stockid, key)
            file.write(sb)

    file.close()




def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'u:p:w:s')
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)

    flag = False
    saveFileName = ''
    path = ''
    url = ''
    for o,a in opts:
        if o in '-w':
            flag = True
            saveFileName = a
        elif o in '-s':
            sfalg = True
        elif o in '-p':
            path = a
        elif o in '-u':
            url = a

    if not flag:
        date = time.strftime('%Y-%m-%d',time.localtime(time.time()))
        saveFileName = '%s_stockIds' % (date)

    if path.strip():
        os.chdir(path)
    #sprint saveFileName
    get_stock_url(saveFileName, url)

host = 'http://q.stock.sohu.com/'

if __name__ == '__main__':
    #print os.getcwd()
    main(sys.argv)
