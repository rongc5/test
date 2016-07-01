#!/usr/bin/env python
#coding=utf-8

import threading, signal, os,sys

def trimStr(str):
    str = str.replace(" ", "")
    str = str.replace("\r", "")
    str = str.replace("\t", "")
    str = str.replace("\n", "")
    return str

def cmp2str(str1, str2):
    str1 = trimStr(str1)
    str2 = trimStr(str2)

    #print str1
    #print str2
    return str1 == str2


def threadFun(signum, frame):

    subPid = int(getSubPid())
    if subPid:
        try:
            os.kill(subPid, signal.SIGKILL)
            truncateSubPid()
        except OSError:
            pass


def getSubPid():
    pid = ""
    try:
        fp = open("subPid", 'r')
        pid = str(fp.read())
        fp.close()
    except:
        pass

    return pid

def setSubPid(subPid):
    fp = open("subPid", 'w+')
    fp.write(str(subPid))
    fp.close()

def truncateSubPid():
    fo = open("subPid", "w+")
    fo.truncate()
    fo.close()



class Daemon:
        """
        A generic daemon class.

        Usage: subclass the Daemon class and override the run() method
        """
        def __init__(self, stdin='/dev/null', stdout='/dev/null', stderr='/dev/null'):
                self.stdin = stdin
                self.stdout = stdout
                self.stderr = stderr


        def daemonize(self):
                try:
                        pid = os.fork()
                        if pid > 0:
                                # exit first parent
                                sys.exit(0)
                except OSError, e:
                        sys.stderr.write("fork #1 failed: %d (%s)\n" % (e.errno, e.strerror))
                        sys.exit(1)

                #os.chdir("/")
                os.setsid()
                os.umask(0)

                # do second fork
                try:
                        pid = os.fork()
                        if pid > 0:
                                # exit from second parent
                                sys.exit(0)
                except OSError, e:
                        sys.stderr.write("fork #2 failed: %d (%s)\n" % (e.errno, e.strerror))
                        sys.exit(1)

                # redirect standard file descriptors
                sys.stdout.flush()
                sys.stderr.flush()
                si = file(self.stdin, 'r')
                so = file(self.stdout, 'a+')
                se = file(self.stderr, 'a+', 0)
                os.dup2(si.fileno(), sys.stdin.fileno())
                os.dup2(so.fileno(), sys.stdout.fileno())
                os.dup2(se.fileno(), sys.stderr.fileno())


if __name__ == '__main__':
    #str = str.replace("\n", "\\n")
    str1 = "hello world\n"
    str2 = "hello       wor       ld \n"
    print cmp2str(str1, str2)


