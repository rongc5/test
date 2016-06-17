#!/usr/bin/env python
#coding=utf-8

import os, sys
from myLog import *
from utils import *

class RunJs:
    def __init__(self, log):
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutFile = ''
        self.stdRes = ''
        self.answerId = ''
        self.srcFile = ''
        self.errFile = ''

    def rest(self):
        self.lang = ''
        self.srcCode = ''
        self.inPutfileContent = ''
        self.stdRes = ''
        self.answerId = ''
        self.errContent = ''

        self.srcFile = ''
        self.inPutFile = ''
        self.errFile = ''

    def clear(self):
        os.chdir('../')
        #cmd = 'rm -rf %s' % (self.answerId)
        #os.system(cmd)

    def cmpResult(self):
        return cmp2str(self.stdRes, self.errContent)

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.answerId)
        os.system(cmd)
        path = '%s/%s' % (os.getcwd(), self.answerId)
        os.chdir(path)

        suffix = ''
        if 'node' in self.lang:
            suffix = '.js'
        else:
            self.log.warning("lang Error %s answerId %s" % (self.lang, self.answerId))

        self.srcFile = '%s%s' % (self.answerId, suffix)
        fp = open(self.srcFile, 'w+')
        fp.write(self.srcCode)
        fp.close()

        if self.inPutfileContent.strip():
            self.inPutFile = '%s.txt' % (self.answerId)
            fp = open(self.inPutFile, 'w+')
            fp.write(self.inPutfileContent)
            fp.close()

        self.errFile = '%s.err' % (self.answerId)



    def build(self):
        pass

    def run(self):
        cmd = ''
        if self.inPutfileContent.strip():
            cmd = '%s %s %s 2>&1 > %s' % (self.lang, self.srcFile, self.inPutFile, self.errFile)
        else:
            cmd = '%s %s 2>&1 > %s' % (self.lang, self.srcFile, self.errFile)

        os.system(cmd)


    def getErrInfo(self):
        fp = open(self.errFile, 'r')
        self.errContent = str(fp.read())
        fp.close()

    def buildAndrun(self, answerId, lang, srcCode, stdRes, makefile_path, inPutfileContent=''):
        self.rest()
        self.lang = lang
        self.srcCode = srcCode
        self.stdRes = stdRes
        self.inPutfileContent = inPutfileContent
        self.answerId = answerId

        self.genBuildFile()


        signal.signal(signal.SIGALRM, threadFun)
        signal.alarm(3)
        pid = os.fork()
        if not pid:
            self.run()
            os._exit(0)
        else:
            setSubPid(pid)
            try :
                os.wait()
                signal.alarm(0)
            except OSError:
                pass

        self.getErrInfo()
        self.clear()
        return self.errContent



if __name__ == '__main__':
    pass
