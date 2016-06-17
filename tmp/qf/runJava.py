#!/usr/bin/env python
#coding=utf-8

import os, sys
from myLog import *
from utils import *

class RunJava:
    def __init__(self, log):
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutFile = ''
        self.stdRes = ''
        self.answerId = ''
        self.srcFile = ''
        self.errFile = ''
        self.mk = ''
        self.makefile_path = ''
        self.classNmae = ''

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
        self.mk = ''
        self.makefile_path = ''
        self.classNmae = ''

    def clear(self):
        os.chdir("../")
        cmd = 'rm -rf %s' % (self.answerId)
        os.system(cmd)

    def cmpResult(self):
        return cmp2str(self.stdRes, self.errContent)

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.answerId)
        os.system(cmd)
        os.chdir(self.answerId)

        suffix = ''
        if 'java' in self.lang:
            suffix = '.java'
            self.mk = 'java.mk'
        else:
            self.log.warning("lang Error %s answerId %s" % (self.lang, self.answerId))

        cmd = 'cp %s/%s .' % (self.makefile_path, self.mk)
        os.system(cmd)

        self.errFile = '%s.err' % (self.answerId)

        self.srcFile = '%s%s' % (self.answerId, suffix)
        fp = open(self.srcFile, 'w+')
        fp.write(self.srcCode)
        fp.close()

        self.build()
        cmd = '''cat %s | grep "public" | grep "class" > %s''' % (self.errFile, "tmp.txt")
        os.system(cmd)
        fp = open("tmp.txt", 'r')
        self.classNmae = str(fp.read()).strip("\n").split(" ")[2]
        fp.close()
        cmd = 'mv %s %s%s' % (self.srcFile, self.classNmae, suffix)
        print cmd
        os.system(cmd)
        self.srcFile = '%s%s' % (self.classNmae, suffix)
        print self.srcFile


        if self.inPutfileContent.strip():
            self.inPutFile = '%s.txt' % (self.answerId)
            fp = open(self.inPutFile, 'w+')
            fp.write(self.inPutfileContent)
            fp.close()




    def build(self):
        cmd = 'make -f %s 2> %s' % (self.mk, self.errFile)
        os.system(cmd)

    def run(self):
        cmd = ''
        if self.inPutfileContent.strip():
            cmd = 'java %s %s 2>&1 > %s' % (self.classNmae, self.inPutFile, self.errFile)
        else:
            cmd = 'java %s 2>&1 > %s' % (self.classNmae, self.errFile)

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
