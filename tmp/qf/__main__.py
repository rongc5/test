#!/usr/bin/env python
#coding=utf-8


from myLog import  *
from mysql import *
from runCpp import *
from runJava import *
from runPhp import *
from runJs import *
from runSwift import *
import os, time, base64


#工作目录
work_path = '/tmp/qf'


#makefile 目录
makefile_path = ''


def getAnswerInfo(db, log):

    sql = '''select a.id as answerId, a.source_code as srcCode, a.language as language, a.exec_input, a.param, a.standard_output,
    b.input_file as inPutfileContent from student_program a,
    program_info b where a.program_id = b.id and a.status = "submit_ok" limit 1000;'''

    log.debug("SQL:%s" %(sql))

    db.selectDb('qf_admin_online')

    res = db.select(sql)

    return res


def updateAnswerInfo(db, log, resultInfo):

    sql = '''UPDATE  student_program set status = '%s', exec_output = '%s' where id = '%s';''' % (db.escape_string(resultInfo['status']), db.escape_string(resultInfo['exec_output']), resultInfo['id'])

    log.debug("SQL:%s" % (sql))

    db.selectDb('qf_admin_online')
    db.exesql(sql)


def doJobs(db, log):

    while 1:
        res = getAnswerInfo(db,log)
        #print("doJobs ===> len = %u", len(res))
        if len(res) <= 0:
            time.sleep(10)
            continue

        for item in res:

            resInfo = {}
            item['makefile_path'] = makefile_path
            if  'cpp' in item['language']:
                item['lang'] = 'gcc'
                rCpp = RunCpp(log)
                resInfo = rCpp.buildAndrun(item)

            elif 'java' == item['language']:

                item['lang'] = 'javac'
                rJava = RunJava(log)
                resInfo = rJava.buildAndrun(item)
                print "hello world", resInfo
            #elif 'php' in item['language']:
            #    rPhp = RunPhp(log)
            #    resInfo = rPhp.buildAndrun(item)
            #elif 'node' in item['language']:
            #    rJs = RunJs(log)
            #    resInfo = rJs.buildAndrun(item)
            #elif 'swift' in item['language']:
            #    rSwift = RunSWift(log)
            #    resInfo = rSwift.buildAndrun(item)


            if len(resInfo):
                updateAnswerInfo(db, log, resInfo)


if __name__ == '__main__':

    makefile_path = '%s/%s' % (os.getcwd(), 'script')

    cmd = 'mkdir -p %s' % (work_path)
    os.system(cmd)
    os.chdir(work_path)

    logger = getLoger('qf')
    db = MySQL(logger, '115.28.35.83','qf_admin','5dxZM1b!uS')

    PATH = os.getenv('PATH')
    PATH = '%s:%s' % (PATH, '/home/yang/jdk1.8/bin')
    os.environ['PATH'] = PATH

    doJobs(db, logger)

