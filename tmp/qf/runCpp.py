#!/usr/bin/env python
#coding=utf-8

import os, sys
from myLog import *
from utils import *

class RunCpp:
    def __init__(self, log):
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutFile = ''
        self.stdRes = ''
        self.answerId = ''
        self.srcFile = ''
        self.exeFile = ''
        self.errFile = ''
        self.mk = ''
        self.makefile_path = ''

    def rest(self):
        self.lang = ''
        self.srcCode = ''
        self.inPutfileContent = ''
        self.stdRes = ''
        self.answerId = ''
        self.errContent = ''

        self.srcFile = ''
        self.inPutFile = ''
        self.exeFile = ''
        self.errFile = ''
        self.mk = ''
        self.makefile_path = ''

    def clear(self):
        os.chdir('../')
        cmd = 'rm -rf %s' % (self.answerId)
        os.system(cmd)

    def cmpResult(self):
        return cmp2str(self.stdRes, self.errContent)

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.answerId)
        os.system(cmd)
        path = '%s/%s' % (os.getcwd(), self.answerId)
        os.chdir(path)

        suffix = ''
        if 'gcc' in self.lang:
            suffix = '.c'
            self.mk = 'c.mk'
        elif 'g++' in self.lang:
            suffix = '.cpp'
            self.mk = 'cpp.mk'
        else:
            self.log.warning("lang Error %s answerId %s" % (self.lang, self.answerId))

        cmd = 'cp %s/%s .' % (self.makefile_path, self.mk)
        os.system(cmd)

        self.srcFile = '%s%s' % (self.answerId, suffix)
        fp = open(self.srcFile, 'w+')
        fp.write(self.srcCode)
        fp.close()

        if self.inPutfileContent.strip():
            self.inPutFile = '%s.txt' % (self.answerId)
            fp = open(self.inPutFile, 'w+')
            fp.write(self.inPutfileContent)
            fp.close()


        self.exeFile = '%s.exe' % (self.answerId)
        self.errFile = '%s.err' % (self.answerId)



    def build(self):
        cmd = 'make -f %s TARGET=%s 2> %s' % (self.mk, self.exeFile, self.errFile)
        os.system(cmd)

    def run(self):
        cmd = ''
        if self.inPutfileContent.strip():
            cmd = './%s %s 2>&1 > %s' % (self.exeFile, self.inPutFile, self.errFile)
        else:
            cmd = './%s 2>&1 > %s' % (self.exeFile, self.errFile)

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
        self.makefile_path = makefile_path

        self.genBuildFile()
        self.build()

        self.getErrInfo()
        if self.errContent.strip():
            self.clear()
            return self.errContent
        self.run()

        self.getErrInfo()
        self.clear()
        return self.errContent



if __name__ == '__main__':
    pass
