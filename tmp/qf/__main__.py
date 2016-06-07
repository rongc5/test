#!/usr/bin/env python
#coding=utf-8


from myLog import  *
from mysql import *
from runCpp import *

import os, time, base64

from  runJava import *

#工作目录
work_path = '/tmp/qf'


#makefile 目录
makefile_path = ''


def getAnswerInfo(db, log):

    #sql = '''select a.answerId as answerId,  a.programId as programId, a.srcCode
    #as srcCode, a.lang as lang, a.inPutFileId as inPutFileId, a.questionId as questionId, b.fileContent as inPutFileContent
    #from answerInfo a left join fileInfo b on a.inPutFileId = b.fileId where a.isRun = 0 limit 1000;'''


    sql = '''select a.answerId as answerId,  a.programId as programId, a.srcCode
    as srcCode, a.lang as lang, a.inPutFileId as inPutFileId, a.questionId as questionId, b.fileContent as inPutFileContent,
    c.stdAnswer as stdAnswer from answerInfo a , questionInfo c left join fileInfo b on inPutFileId = b.fileId where
    a.isRun = 0 and a.questionId = c.questionId limit 1000;'''




    log.debug("SQL:%s" %(sql))

    db.selectDb('test')

    res = db.select(sql)

    return res


def updateAnswerInfo(db, log, answerId, resultInfo):

    sql = '''UPDATE  answerInfo set isRun = '1', resultInfo = '%s' where answerId = '%s';''' % (resultInfo, answerId)

    log.debug("SQL:%s" % (sql))

    db.selectDb('test')
    db.exesql(sql)


def doJobs(db, log):
    rCpp = RunCpp(log)

    while 1:
        res = getAnswerInfo(db,log)
        print res
        if len(res) <= 0:
            time.sleep(10)
            continue

        for item in res:

            inPutFileContent = item['inPutFileContent'].strip()
            if inPutFileContent:
                inPutFileContent = base64.decodestring(inPutFileContent)

            if 'gcc' in item['lang'] or 'g++' in item['lang']:

                resInfo = rCpp.buildAndrun(item['answerId'], item['lang'], base64.decodestring(item['srcCode']),
                                           base64.decodestring(item['stdAnswer']), makefile_path, inPutFileContent)
                updateAnswerInfo(db, log, item['answerId'], base64.encodestring(resInfo))

            elif 'javac' in item['lang']:
                pass


if __name__ == '__main__':

    makefile_path = '%s/%s' % (os.getcwd(), 'script')

    cmd = 'mkdir -p %s' % (work_path)
    os.system(cmd)
    os.chdir(work_path)
    print("hello world")
    logger = getLoger('qf')
    db = MySQL(logger, 'localhost','root','123456')
    doJobs(db, logger)

