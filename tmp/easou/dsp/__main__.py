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
        conf = ConfigParser.ConfigParser()
        conf.read(cfg)
        #redis 配置读取
        self.redis_host = conf.get("redis", "redis_host")
        self.redis_port = conf.getint("redis", "redis_port")
        self.redis_passwd = conf.get("redis", "redis_passwd")

        #充值信息读取
        self.whether_to_recharge=conf.get("recharge", "whether_to_charge")
        self.userId_to_recharge=conf.get("recharge", "userId")
        self.money_to_recharge=conf.getint("recharge", "money")

        #账户信息读取
        self.whether_to_query = conf.get("account_query", "whether_to_query")
        self.userId_to_query = conf.get("account_query", "userId")
        self.date_to_query = conf.get("account_query", "date")

        self.log.info("cfg:%s", conf.sections())

    def do(self):
        #r = redis.Redis(self.redis_host,self.redis_port,0,self.redis_passwd)
        r = redis.Redis(self.redis_host,self.redis_port)
        if not r.ping():
            return

        #充值处理
        if 'yes' in self.whether_to_recharge.lower():
            #总金额

            key = '%s_total' % (self.userId_to_recharge)
            total = r.get(key)
            self.log.info("Before recharge ==> userid:%s total:%s recharge:%s", self.userId_to_recharge, total, self.money_to_recharge)
            total = r.incrby(key, self.money_to_recharge)
            self.log.info("After recharge ==> userid:%s total:%s recharge:%s", self.userId_to_recharge, total, self.money_to_recharge)


        #查询处理
        if 'yes' in self.whether_to_query.lower():
            #日消费
            key = '%s_%s_cost' % (self.userId_to_query, self.date_to_query)
            date_cost = r.get(key)

            #总消费
            key = '%s_cost' % (self.userId_to_query)
            cost = r.get(key)

            #总金额
            key = '%s_total' % (self.userId_to_query)
            total = r.get(key)

            #总余额
            key = '%s_balance' % (self.userId_to_query)
            balance = r.get(key)

            self.log.info("account_query ==> userid:%s total:%s balance:%s cost:%s date_cost:%s", self.userId_to_recharge, total, balance, cost, date_cost)










if __name__ == '__main__':

    #daemon = Daemon()
    #daemon.daemonize()


    logger = getLoger('logs/dsp')

    job = Task(logger, "conf/dsp.ini")
    job.do()

