#!/usr/bin/env python2
#encoding=gbk

import urllib2

import utils
#from html.parser import HTMLParser
import HTMLParser
import getopt, sys, time, os

sfalg =False
class MyHTMLParser(HTMLParser.HTMLParser, bk_links):

    def handle_starttag(self, tag, attrs):
        #print "Encountered the beginning of a %s tag" % tag
        if tag == "a":
            if len(attrs) == 0:
                pass
            else:
                for (variable, value) in attrs:
                    if variable == "href":
                        if 'http://q.stock.sohu.com/cn/bk_' in value:
                            bk_links.append(value)



def get_bk_list(filename):
    #res = urllib2.urlopen('http://q.stock.sohu.com/cn/bk.shtml').read()
    res = urllib2.urlopen('http://q.stock.sohu.com/sitemap.shtml').read()
    #print res
    bk_links = []
    hp = MyHTMLParser()
    hp.feed(res)
    hp.close()

    file = open(filename, "w+")

    for item in bk_links:
        if sfalg:
            print item
            file.write(item)

    file.close()




def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'w:s')
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)

    flag = False
    saveFileName = ''
    for o,a in opts:
        print o, a
        if o in '-w':
            flag = True
            saveFileName = a
        elif o in '-s':
            sfalg = True

    if not flag:
        date = time.strftime('%Y-%m-%d',time.localtime(time.time()))
        saveFileName = '%s_bk_urls' % (date)


    print saveFileName
    get_bk_list(saveFileName)



if __name__ == '__main__':
    print os.getcwd()
    main(sys.argv)
