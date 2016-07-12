#!/usr/bin/env python
#coding=utf-8

import sys, os, random, time

sys.path.append("..")
from qf import *


class genTestData:

    def __init__(self, db, log):
        self.db = db
        self.log = log


    def createTables(self):

        while True:
            tid = '257'
            sql = '''SELECT b.id, a.input, a.output from program_test_case a, student_program b where
            a.program_id = b.program_id and b.status = "ok" and b.id >= '%s';''' % (tid)

            #self.log.info("%s", sql)
            res = db.select(sql)

            for item in res:
                sql = '''update student_program set status = "submit_ok", exec_input='%s', standard_output = '%s' where id = '%s';''' % (item['input'], item['output'], item['id'])
                db.exesql(sql)
                time.sleep(5)

                if item['id'] > tid:
                    tid = item['id']

                sql = '''select status, standard_output from student_program where id = '%s';''' % (item['id'])
                tmp = db.select(sql)
                if len(tmp) <= 0:
                    continue;
                tmp = tmp[0]
                #print tmp
                if tmp['status'] != 'ok':
                    self.log.warning('ERr ==> status:%s id:%s input:%s output:%s standard_output:%s', tmp['status'], item['id'], item['input'], item['output'], tmp['standard_output'])
                else:
                    self.log.warning('OK ==> status:%s id:%s input:%s output:%s standard_output:%s', tmp['status'], item['id'], item['input'], item['output'], tmp['standard_output'])

            time.sleep(60)

if __name__ == '__main__':

    logger= getLoger('test')

    db = MySQL(logger,'115.28.35.83','qf_admin','5dxZM1b!uS')
    db.selectDb('qf_admin_online')

    test = genTestData(db,logger)
    test.createTables()



