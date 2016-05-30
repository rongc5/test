#!/usr/bin/env python
#coding=utf-8

import os
from myLog import *
from utils import *

class RunCpp:
    def __init__(self, log):
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutFile = ''
        self.stdRes = ''
        self.answerId = 0
        self.srcFile = ''
        self.exeFile = ''
        self.errFile = ''

    def rest(self):
        self.lang = ''
        self.srcCode = ''
        self.inPutfileContent = ''
        self.stdRes = ''
        self.answerId = 0
        self.errContent = ''

        if os.path.exists(self.srcFile):
            os.remove(self.srcFile)

        if os.path.exists(self.inPutFile):
            os.remove(self.inPutFile)

        if os.path.exists(self.exeFile):
            os.remove(self.exeFile)

        if os.path.exists(self.errFile):
            os.remove(self.errFile)

        self.srcFile = ''
        self.inPutFile = ''
        self.exeFile = ''
        self.errFile = ''

    def genBuildFile(self):
        suffix = ''
        if 'gcc' in self.lang:
            suffix = '.c'
        elif 'g++' in self.lang:
            suffix = '.cpp'
        else:
            self.log.warning("lang Error %s answerId %u" % (self.lang, self.answerId))

        self.srcFile = '%u%s' % (self.answerId, suffix)
        fp = open(self.srcFile, 'w+')
        fp.write(self.srcCode)
        fp.close()

        if self.inPutfileContent:
            self.inPutFile = '%u.txt' % (self.answerId)
            fp = open(self.inPutfileContent, 'w+')
            fp.write(self.inPutfileContent)
            fp.close()


        self.exeFile = '%u.exe' % (self.answerId)
        self.errFile = '%u.err' % (self.answerId)


    def build(self):
        cmd = '%s -o %s %s 2> %s' % (self.lang, self.exeFile, self.srcFile, self.errFile)
        os.system(cmd)

    def run(self):
        cmd = ''
        if self.inPutFile:
            cmd = './%s %s 2>&1 > %s' % (self.exeFile, self.inPutFile, self.errFile)
        else:
            cmd = './%s 2>&1 > %s' % (self.exeFile, self.errFile)

        os.system(cmd)


    def getErrInfo(self):
        fp = open(self.errFile, 'r')
        self.errContent = str(fp.read())
        fp.close()

    def buildAndrun(self, answerId, lang, srcCode, stdRes, inPutfileContent=''):
        self.rest()
        self.lang = lang
        self.srcCode = srcCode
        self.stdRes = stdRes
        self.inPutfileContent = inPutfileContent
        self.answerId = answerId

        self.genBuildFile()
        self.build()

        self.getErrInfo()
        if self.errContent.strip():
            return self.errContent
        self.run()

        self.getErrInfo()
        return self.errContent



if __name__ == '__main__':
    pass
