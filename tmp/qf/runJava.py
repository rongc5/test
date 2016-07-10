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
        self.outFile = ''
        self.errFile = ''
        self.exec_input_file = ''
        self.makefile_path = ''
        self.classNmae = ''
        self.exec_input = ''
        self.param = ''
        self.path = ''
        self.statFile = ''


    def cmpResult(self):
        return cmp2str(self.stdRes, self.errContent)

    def genBuildFile(self):

        cmd = 'mkdir -p %s' % (self.path)
        os.system(cmd)
        os.chdir(self.path)

        suffix = '.java'

        self.srcFile = '%s%s' % (self.answerId, suffix)

        writeStrToFile("tmp.txt", self.srcCode)

        self.errFile = getErrFileName(self.answerId)

        cmd = '''cat %s | grep -v "package" > %s''' % ("tmp.txt", self.srcFile)
        os.system(cmd)

        self.build()
        cmd = '''cat %s | grep "public" | grep "class" > %s''' % (self.errFile, "tmp.txt")
        os.system(cmd)
        sys.stdout.flush()
        fp = open("tmp.txt", 'r')
        try:
            for line in fp:
                items = line.strip("\n").split(" ")
                if items[0] == "public" and items[1] == "class":
                    self.classNmae = items[2]
                    cmd = 'mv %s %s%s' % (self.srcFile, self.classNmae, suffix)
                    print cmd
                    os.system(cmd)
                    self.srcFile = '%s%s' % (self.classNmae, suffix)
        finally:
            fp.close()

        if self.inPutfileContent.strip():
            self.inPutFile = '%s.txt' % (self.answerId)
            writeStrToFile(self.inPutFile, self.inPutfileContent)


        self.exec_input_file = '%s.exec_input' % (self.answerId)
        writeStrToFile(self.exec_input_file, self.exec_input)

        self.outFile = getOutPutFileName(self.answerId)
        self.statFile = getStatusFileName(self.answerId)


    def build(self):
        cmd = '%s %s 2> %s' % (self.lang, self.srcFile, self.errFile)
        os.system(cmd)

    def run(self):
        cmd = ''
        cmd = 'java %s %s %s 2>%s < %s | head -c 1024 > %s' % (self.classNmae, self.param, self.inPutFile, self.errFile, self.exec_input_file, self.outFile)

        os.system(cmd)

    def buildAndrun(self,item):
        self.lang = item['lang']
        self.srcCode = item['srcCode']
        self.stdRes = item['standard_output']
        self.inPutfileContent = item['inPutfileContent']
        self.answerId = item['answerId']
        self.makefile_path = item['makefile_path']
        self.exec_input = item['exec_input']
        self.param = item['param']
        self.path = item['path']

        self.genBuildFile()
        self.build()

        cmd = 'ls *.class > %s' % ("tmp.txt")
        os.system(cmd)
        name = getFileInfo("tmp.txt")

        if not name.strip():
            writeStrToFile(self.statFile, 'compile_error')
            return
        else:
            writeStrToFile(self.statFile, 'compile_ok')

        if not self.classNmae.strip():
            self.classNmae = name[0:name.find('.')]


        self.run()

        if  os.path.exists(self.outFile):
                exec_output = getFileInfo(self.outFile)
                if cmp2str(exec_output, item['standard_output']):
                    writeStrToFile(self.statFile, 'ok')
                else:
                    writeStrToFile(self.statFile, 'run_error')
        else:
                writeStrToFile(self.statFile, 'run_error')



if __name__ == '__main__':
    pass
