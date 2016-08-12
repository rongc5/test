#!/usr/bin/env python
#coding=utf-8


from myLog import  *
from utils import *
import os, time
import ConfigParser
import redis



class Task:

    def __init__(self, log, cfg):
        self.cfg = cfg
        self.log = log
        self.conf = ConfigParser.ConfigParser()
        self.conf.read(cfg)
        #redis 配置读取
        self.redis_host = self.conf.get("redis", "redis_host")
        self.redis_port = self.conf.getint("redis", "redis_port")
        self.redis_passwd = self.conf.get("redis", "redis_passwd")

        #充值信息读取
        self.whether_to_recharge=self.conf.get("recharge", "whether_to_charge")
        self.whether_to_reset=self.conf.get("recharge", "whether_to_reset")
        self.userId_to_recharge=self.conf.get("recharge", "userId")
        self.money_to_recharge=self.conf.getint("recharge", "money")

        #账户信息读取
        self.whether_to_query = self.conf.get("account_query", "whether_to_query")
        self.userId_to_query = self.conf.get("account_query", "userId")
        self.date_to_query = self.conf.get("account_query", "date")

        self.log.info("cfg:%s", self.conf.sections())

    def reset(self, r):
        key = '%s_total' % (self.userId_to_recharge)
        total = r.get(key)
        if not total:
            total = '0'

        key = '%s_cost' % (self.userId_to_recharge)
        cost = r.get(key)
        if not cost:
            cost = '0'

        self.log.info("Before reset ==> userid:%s total:%s cost:%s recharge:%s", self.userId_to_recharge, total, cost, self.money_to_recharge)
        key = '%s_total' % (self.userId_to_recharge)
        r.set(key, '0')

        key = '%s_cost' % (self.userId_to_recharge)
        r.set(key, '0')

        self.conf.set('recharge', 'whether_to_reset', 'NO')
        self.conf.write(open(self.cfg, 'w'))


    def recharge(self, r):
        key = '%s_total' % (self.userId_to_recharge)
        total = r.incrby(key, self.money_to_recharge)
        total = r.get(key)
        if not total:
            total = '0'

        key = '%s_cost' % (self.userId_to_recharge)
        cost = r.get(key)
        if not cost:
            cost = '0'

        self.conf.set('recharge', 'whether_to_charge', 'NO')
        self.conf.write(open(self.cfg, 'w'))

        self.log.info("After recharge ==> userid:%s total:%s cost:%s recharge:%s", self.userId_to_recharge, total, cost, self.money_to_recharge)


    def query(self, r):
        key = '%s_%s_cost' % (self.userId_to_query, self.date_to_query)
        date_cost = r.get(key)
        if not date_cost:
            date_cost = '0'

        key = '%s_cost' % (self.userId_to_query)
        cost = r.get(key)
        if not cost:
            cost = '0'

        key = '%s_total' % (self.userId_to_query)
        total = r.get(key)
        if not total:
            total = '0'

        key = '%s_balance' % (self.userId_to_query)
        balance = r.get(key)
        if not balance:
            balance = '0'

        self.log.info("account_query ==> userid:%s total:%s balance:%s cost:%s date_cost:%s", self.userId_to_recharge, total, balance, cost, date_cost)

    def do(self):
        r = redis.Redis(self.redis_host,self.redis_port,0,self.redis_passwd)
        #r = redis.Redis(self.redis_host,self.redis_port)
        if not r.ping():
            return

        if 'yes' in self.whether_to_reset.lower():
            self.reset(r)

        #充值处理
        if 'yes' in self.whether_to_recharge.lower():
            #总金额
            self.recharge(r)


        if 'yes' in self.whether_to_query.lower():
            self.query(r)


if __name__ == '__main__':

    #daemon = Daemon()
    #daemon.daemonize()

    logger = getLoger('recharge')

    job = Task(logger, "conf/dsp.ini")
    job.do()

