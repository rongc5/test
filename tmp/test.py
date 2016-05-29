#!/usr/bin/env python
#coding=utf-8

import sys, os


from qf import *


class genTestData:

    def __init__(self, db, log):
        self.db = db
        self.log = log


    def insertAnswer(self, file):
        fp = open(file, "r")
        strContent = str(fp.read())
        fp.close()

        lang = ''
        resFile = 'res.txt'
        cmd = ''
        suffix = file[file.find('.'):]
        if '.c' in suffix:
            lang = 'gcc'
            cmd = '%s -o app %s > %s' %(lang, file, resFile)
        elif '.cpp' in suffix:
            lang = 'g++'
            cmd = '%s -o app %s > %s' %(lang, file, resFile)

        os.system(cmd)
        fp = open(resFile, "r")
        stdAnswer = str(fp.read())
        fp.close()


        #print(strContent)
        sql = '''insert into answerInfo(programId, srcCode, lang, inPutFileId, questionId, resultInfo) values('%s', '%s',
        '%s', '%s', '%s', '%s')''' % ('10086', strContent, lang, '0', '1', '')

        self.db.exesql(sql)




if __name__ == '__main__':

    logger=myLog.getLoger('mysql')

    db = mysql.MySQL('localhost','root','123456',logger=logger)
    db.selectDb('test')

    test = genTestData(db,logger)
    test.insertAnswer(sys.argv[1], sys.argv[2])


