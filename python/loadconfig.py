#coding=utf-8
#!/usr/bin/python

from __future__ import with_statement
import ConfigParser

config=ConfigParser.ConfigParser()
with open('config.ini', 'r+') as cfgfile:
    config.readfp(cfgfile)
    name=config.get('','name')
    age=config.get('','age')
    age=config.get('','sex')
    print name
    print age
#config.set('info','age', 33)
#config.set('info', 'name','zhang')
#name=config.get('info','name')
#age=config.get('info','age')
#age=config.get('info','sex')
#print name
#print age