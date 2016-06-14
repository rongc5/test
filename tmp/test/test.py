#!/usr/bin/env python
#coding=utf-8

import sys, os, random, base64

sys.path.append("..")
from qf import *


class genTestData:

    def __init__(self, db, log):
        self.db = db
        self.log = log


    def createTables(self):
        sql = '''CREATE TABLE IF NOT EXISTS `fileInfo` (
            `fileId` INT(11) NOT NULL AUTO_INCREMENT,
            `fileName` char(50) NOT NULL,
            `fileContent` char(50) NOT NULL,
            PRIMARY KEY (`fileId`)
            ) ; '''

        self.db.exesql(sql)
        #self.log.info("%s", sql)

        sql = '''CREATE TABLE IF NOT EXISTS `answerInfo` (
            `answerId` INT(11) NOT NULL AUTO_INCREMENT,
            `programId` INT(11) NOT NULL,
            `srcCode` varchar(4096) NOT NULL,
            `lang` char(50) NOT NULL,
            `inPutFileId` INT(11) NOT NULL DEFAULT 0,
            `questionId` INT(11) NOT NULL,
            `resultInfo` varchar(2048) DEFAULT NULL,
            `isRun` tinyint NOT NULL DEFAULT 0,
            PRIMARY KEY (`answerId`)
            ) ;'''

        self.db.exesql(sql)

        sql = '''CREATE TABLE IF NOT EXISTS `questionInfo` (
            `questionId` INT(11) NOT NULL AUTO_INCREMENT,
            `questionName` varchar(50) NOT NULL,
            `questionContent` varchar(1024) DEFAULT NULL,
            `stdAnswer` varchar(120) NOT NULL,
            `inPutFileId` INT(11) NOT NULL,
            PRIMARY KEY (`questionId`)
            ) ;'''

        self.db.exesql(sql)


    def insertAnswer(self, srcFile, inputFile):
        fp = open(srcFile, "r")
        strContent = str(fp.read())
        #strContent = repr(strContent)[1:-1]
        fp.close()

        self.log.info("%s", srcFile)
        lang = ''
        resFile = 'res.txt'
        stdAnswer = ""
        inputFileContent = ""
        cmd = ''

        suffix = srcFile[srcFile.find('.'):]
        if '.c' == suffix:
            lang = 'gcc'
            cmd = '%s -o app %s' %(lang, srcFile)
            os.system(cmd)
            cmd = './%s > %s' % ('app', resFile)
            os.system(cmd)
        elif '.cpp' == suffix:
            lang = 'g++'
            cmd = '%s -o app %s' %(lang, srcFile)
            os.system(cmd)
            cmd = './%s > %s' % ('app', resFile)
            os.system(cmd)
        elif '.java' == suffix:
            lang = 'javac'
            cmd = '%s -g %s' % (lang, srcFile)
            os.system(cmd)
            cmd = 'java  %s > %s' % (srcFile[0:srcFile.find('.')], resFile)
            os.system(cmd)
        elif '.php' == suffix:
            lang = 'php'
            cmd = '%s -f %s > %s' % (lang, srcFile, resFile)
            os.system(cmd)
        elif '.js' == suffix:
            lang = 'node'
            cmd = '%s %s > %s' % (lang, srcFile, resFile)
            os.system(cmd)

        fp = open(resFile, "r")
        stdAnswer = str(fp.read())
        fp.close()

        inPutFileId = '0'
        if inputFile.strip():
            fp = open(inputFile, "r")
            inputFileContent = str(fp.read())
            fp.close()

            sql = '''insert into fileInfo(fileName, fileContent) values('%s', '%s')''' % (inputFile, base64.encodestring(inputFileContent))
            self.db.exesql(sql)

            print sql
            sql ='''SELECT LAST_INSERT_ID();'''
            inPutFileId = self.db.select(sql)[0]['LAST_INSERT_ID()']

        sql = '''insert into questionInfo(questionName, questionContent, stdAnswer, inPutFileId)
         values('%s', '%s', '%s','%s')''' % ('', base64.encodestring(strContent), base64.encodestring(stdAnswer), inPutFileId)
        self.db.exesql(sql)
        self.log.info("sql: %s", sql)
        sql ='''SELECT LAST_INSERT_ID();'''
        questionId = self.db.select(sql)[0]['LAST_INSERT_ID()']

        programId = random.randint(100, 100000)

        #print(strContent)
        sql = '''insert into answerInfo(programId, srcCode, lang, inPutFileId, questionId, resultInfo) values('%s', '%s',
        '%s', '%s', '%s', '%s')''' % (programId, base64.encodestring(strContent), lang, inPutFileId, questionId, '')
        self.log.info("sql: %s", sql)
        self.db.exesql(sql)




if __name__ == '__main__':

    logger= getLoger('test')

    db = MySQL(logger,'localhost','root','123456')
    db.selectDb('test')

    test = genTestData(db,logger)
    test.createTables()


    if len(sys.argv) == 2:
        test.insertAnswer(sys.argv[1], '')
    elif len(sys.argv) == 3:
        test.insertAnswer(sys.argv[1], sys.argv[2])


