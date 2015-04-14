#!/usr/bin/python2.6-config

import os, sys

def fun():
    """hello world"""
pass

print os.path.realpath(__file__)
print __name__
print fun.__doc__
print __package__


