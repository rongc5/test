#!/usr/bin/env python2
#encoding=gbk

import urllib2

import utils
#from html.parser import HTMLParser
import HTMLParser
import getopt, sys, time, os

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
                        if 'http://q.stock.sohu.com/cn/bk_' in value:
                            self.href = value
                            #self.bk_links.append(value)
                            self.flag = True

    def handle_data(self,data):
        if self.flag:
            self.bk_links[data] = self.href
            self.flag = False
            self.href = ''


def get_bk_list(filename):
    #res = urllib2.urlopen('http://q.stock.sohu.com/cn/bk.shtml').read()
    res = urllib2.urlopen('http://q.stock.sohu.com/sitemap.shtml').read()
    #print res
    bk_links = {}
    hp = MyHTMLParser(bk_links)
    hp.feed(res)
    hp.close()

    file = open(filename, "w+")

    for key in bk_links:
        if sfalg:
            print key, bk_links[key]
        sb = '%s\t%s\n' % (key, bk_links[key])
        file.write(sb)

    file.close()




def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'p:w:s')
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)

    flag = False
    saveFileName = ''
    path = ''
    for o,a in opts:
        print o, a
        if o in '-w':
            flag = True
            saveFileName = a
        elif o in '-s':
            sfalg = True
        elif o in '-p':
            path = a

    if not flag:
        date = time.strftime('%Y-%m-%d',time.localtime(time.time()))
        saveFileName = '%s_bk_urls' % (date)

    if path.strip():
        os.chdir(path)
    #sprint saveFileName
    get_bk_list(saveFileName)



if __name__ == '__main__':
    print os.getcwd()
    main(sys.argv)
