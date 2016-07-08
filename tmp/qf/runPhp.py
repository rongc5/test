#!/usr/bin/env python
#coding=utf-8

import os, sys
from myLog import *
from utils import *

class RunPhp:
    def __init__(self, log):
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutFile = ''
        self.stdRes = ''
        self.answerId = ''
        self.srcFile = ''
        self.errFile = ''
        self.exec_input = ''
        self.param = ''
        self.exec_input_file = ''
        self.outFile = ''
        self.path = ''
        self.statFile = ''

    def cmpResult(self):
        return cmp2str(self.stdRes, self.errContent)

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.path)
        os.system(cmd)
        os.chdir(self.path)

        suffix = ''
        if 'php' in self.lang:
            suffix = '.php'
        else:
            self.log.warning("lang Error %s answerId %s" % (self.lang, self.answerId))

        self.srcFile = '%s%s' % (self.answerId, suffix)
        writeStrToFile(self.srcFile, self.srcCode)


        if self.inPutfileContent.strip():
            self.inPutFile = '%s.txt' % (self.answerId)
            writeStrToFile(self.inPutFile, self.inPutfileContent)


        self.exec_input_file = '%s.exec_input' % (self.answerId)
        writeStrToFile(self.exec_input_file, self.exec_input)

        self.errFile = getErrFileName(self.answerId)
        self.outFile = getOutPutFileName(self.answerId)
        self.statFile = getStatusFileName(self.answerId)


    def build(self):
        pass

    def run(self):
        cmd = ''

        cmd = '%s -f %s %s %s 2>%s < %s | head -c 1024 > %s' % (self.lang, self.srcFile, self.param, self.inPutFile, self.errFile, self.exec_input_file, self.outFile)

        os.system(cmd)


    def buildAndrun(self, item):
        self.lang = item['lang']
        self.srcCode = item['srcCode']
        self.stdRes = item['standard_output']
        self.inPutfileContent = item['inPutfileContent']
        self.answerId = item['answerId']
        self.exec_input = item['exec_input']
        self.param = item['param']
        self.path = item['path']

        self.genBuildFile()

        self.run()

        if  os.path.exists(self.outFile):
                exec_output = getFileInfo(self.outFile)
                if cmp2str(exec_output, item['standard_output']):
                    writeStrToFile(self.statFile, 'ok')
                else:
                    writeStrToFile(self.statFile, 'run_error')
        else:
                writeStrToFile(self.statFile, 'run_error')


        return


if __name__ == '__main__':
    pass
