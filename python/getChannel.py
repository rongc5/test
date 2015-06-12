#!/usr/bin/env python
#_*_ utf-8 _*_
#writed by wuxt on 2012.12.26

import os, sys, time, re, MySQLdb, string, logging, datetime

cidDict = {}

logHandle = logging.getLogger("getChannel")

class logFilter:
    "' read log by line and filter it by regular expression, then will deal it by callback'"

    def __init__(self):
        self.count = 0

    def Process(self, log):
        fd = open(log)

        try:
            dbcon = MySQLdb.connect(host='123.103.62.131',user='permitdbuser',passwd='permitdbpwd',db='statistics')
        except:
            print "open db failure"
            logHandle.log(logging.CRITICAL,"open db failure")
            exit(0)

        cursor = dbcon.cursor()

        while True:
            line = fd.readline()

            print line
            if line:
                m = re.search(r'\<(\d*-\d*-\d*\s+\d*:\d*:\d*)\s+.*\bparserXml\b.*\bcontent\b.*\bSPREQ\b.*\<MAC\>(.*)\</MAC\>.*\<PF\>(.*)\</PF\>.*\<VALUE\>(.*)\</VALUE\>',line)
                print m
                if m is not None:
                    mac = m.groups()[1]
                    pf = m.groups()[2]
                    cid = m.groups()[3]
                    start = m.groups()[0]
                    print mac, pf, cid, start

                    vList = [pf,cid,start,start]

                    old = cidDict.get(mac)
                    if old is None:
                        print "don't has key %s " %mac
                        cidDict[mac]=vList
                    else:
                        print "has key %s:%s" %(mac,str(old))
                        old[3] = start
                        cidDict[mac] = old

                        if (old[1] != cid):
                            if old[2] > old[3]:
                                print old[2], old[3]
                            sql = "INSERT INTO t_devChannelInfo(cid,mac,pf,start,end) values(%d,'%s','%s','%s','%s')" %(int(old[1]),mac,old[0],old[2],old[3])
                            try:
                                cursor.execute(sql)
                                print 'ok', sql
                            except Exception, e:
                                print e
                                print 'failue', sql
                            cidDict[mac] = vList

                    self.count+=1
            else:
                print"readline failure"
                break
        dbcon.commit()
        cursor.close()
        dbcon.close()
        fd.close()

    def total(self):
        return self.count

if __name__ == "__main__":

    if (len(sys.argv)) < 2:
        print "Invalid argument"
        exit(0)
    else:
        logF = sys.argv[1]


    logging.basicConfig(
        filename = "getcid.log",
        format = "%(levelname)-8s %(asctime)s %(filename)-10s %(lineno)-5s :  %(message)s",
        level = logging.DEBUG
    )

    today = time.strftime("%Y-%m-%d", time.localtime(time.time()))

    if logF[-1] is '/':
        dir = logF+'whilst'+today
    else:
        dir = logF+'/'+'whilst'+today

    #print (dir)
    logHandle.log(logging.DEBUG,"Will anailysis %s", dir)

    #if (os.path.isdir(dir)) is True:
        #print "have  dir why?"
        ##delete all files in the dir
        #cmd = "rm -rf %s" %dir
        #print cmd
        #os.system(cmd)
        #logHandle.log(logging.DEBUG, "Remove %s OK", dir)

    #dir='/home/haier/unitServer/log/CAEWhilst/whilst2014-01-02'
    #dir = '/mnt/hgfs/D/code/linux-virtual/myproj/python/whilst2014-01-02'
    print dir
    print logF
    os.mkdir(dir)
    os.chdir(logF)
    cmd = "mv caewhilst.log* %s" %dir
    os.system(cmd)
    os.chdir(dir)

    channel = logFilter()

    files = sorted(os.listdir(dir), reverse=True)

    for logFile in files:
        print logFile
        start = time.clock()
        channel.Process(logFile)
        end = time.clock()
        num = channel.total()
        logHandle.log(logging.DEBUG,"file:%s total: %d time:%fms ", logFile,
                num, (end-start))
