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
        self.outFile = ''
        self.errFile = ''
        self.exec_input_file = ''
        self.exec_input = ''
        self.param = ''


    def clear(self):
        os.chdir('../')
        cmd = 'rm -rf %s' % (self.answerId)
        os.system(cmd)

    def cmpResult(self):
        res = cmp2str(self.stdRes, self.errContent)
        self.log.warning("id %s cmpResult %s  %s %d" % (self.answerId, self.stdRes, self.errContent, res))

        return res

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.answerId)
        os.system(cmd)
        path = '%s/%s' % (os.getcwd(), self.answerId)
        os.chdir(path)

        suffix = ''
        if 'gcc' in self.lang:
            suffix = '.c'
        elif 'g++' in self.lang:
            suffix = '.cpp'
        else:
            self.log.warning("lang Error %s answerId %s" % (self.lang, self.answerId))

        self.srcFile = '%s%s' % (self.answerId, suffix)
        fp = open(self.srcFile, 'w+')
        fp.write(self.srcCode)
        fp.close()

        self.inPutFile = '%s.txt' % (self.answerId)
        fp = open(self.inPutFile, 'w+')
        fp.write(self.inPutfileContent)
        fp.close()

        self.exec_input_file = '%s.exec_input' % (self.answerId)
        fp = open(self.exec_input_file, 'w+')
        fp.write(self.exec_input)
        fp.close()


        self.exeFile = '%s.exe' % (self.answerId)
        self.errFile = '%s.err' % (self.answerId)
        self.outFile = '%s.outFile' % (self.answerId)


    def build(self):
        cmd = '%s -o %s %s 2> %s' % (self.lang, self.exeFile, self.srcFile, self.errFile)
        os.system(cmd)

    def run(self):
        cmd = ''
        cmd = './%s %s %s 2> %s < %s | head -c 1024 > %s' % (self.exeFile, self.param, self.inPutFile, self.errFile, self.exec_input_file, self.outFile)

        os.system(cmd)


    def getResInfo(self, file):
        try:
            fp = open(file, 'r')
            self.errContent = str(fp.read())
            fp.close()
        except IOError:
            pass

    def buildAndrun(self, item):
        self.lang = item['lang']
        self.srcCode = item['srcCode']
        self.stdRes = item['standard_output']
        self.inPutfileContent = item['inPutfileContent']
        self.answerId = item['answerId']
        self.exec_input = item['exec_input']
        self.param = item['param']

        self.genBuildFile()
        self.build()

        resInfo = {}
        resInfo['id'] = self.answerId
        if not os.path.exists(self.exeFile):
            self.getResInfo(self.errFile)
            resInfo['status'] = 'compile_error'
            resInfo['exec_output'] = self.errContent
            self.clear()
            return resInfo

        pid = os.fork()
        if not pid:
            self.run()
            os._exit(0)

        setSubPid(pid)
        signal.signal(signal.SIGALRM, threadFun)
        signal.alarm(3)
        try :
            os.wait()
            signal.alarm(0)
        except OSError:
            pass

        if  os.path.exists(self.outFile):
            self.getResInfo(self.outFile)
            if self.cmpResult():
                resInfo['status'] = 'ok'
            else:
                resInfo['status'] = 'run_error'
            resInfo['exec_output'] = self.errContent
        else:
            self.getResInfo(self.errFile)
            resInfo['status'] = 'run_error'
            resInfo['exec_output'] = self.errContent

        self.clear()
        return resInfo


if __name__ == '__main__':
    pass
