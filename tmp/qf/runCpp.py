#!/usr/bin/env python
#coding=utf-8

import mysql, myLog

class RunCpp:
    def __init__(self, db, log):
        self.db = db
        self.log = log
        self.lang = ''
        self.srcCode = ''
        self.inPutfile = ''

    def rest(self):
        self.lang = ''
        self.srcCode = ''
        self.inPutfile = ''

    def build(self, lang, srcCode, inPutfile=''):
        pass
