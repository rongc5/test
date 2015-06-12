#/usr/bin/env python
#encoding=gbk


#=============================================================================
#     FileName: count_cmatch.py
#         Desc:
#       Author: zm
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2015-05-21 11:08:15
#      History:
#=============================================================================

import threading, os, time

token="ds ret cmatch"

src_log_name = "asp.log.db"
dst_log_name = "log.db"
sleep_time = 60 * 10 * 3

thread_num = 16

class Grep:
    def __init__(self, src_name, dst_name):
        self.src_name = src_name
        self.dst_name = dst_name


    def do(self):
        self.fp_src = open(self.src_name, "r")
        self.fp_dest = open(self.dst_name, "w")
        for line in self.fp_src:
            line = line.strip()
            if not line:
                continue

            if token in line:
                try:
                    self.fp_dest.write(line+'\n')
                except:
                    pass

        self.fp_src.close()
        self.fp_dest.close()

        try:
            os.remove(self.src_name)
        except:
            pass




class myThread(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.t_name = name


    def run(self):
        src_name = self.t_name.strip()
        if not src_name:
            return

        tail = src_name[src_name.find(src_log_name) + len(src_log_name):]
        if not tail.strip():
            return
        dest_name = "%s%s" % (dst_log_name,tail)
        print '%s ===> %s' % (src_name, dst_log_name)
        mygrep = Grep(src_name, dest_name)
        mygrep.do()

        print 'tread %s is over!' % (self.t_name)



def doTask():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)
    threads = []
    i = 0
    for file in dirs:
        if i >= thread_num:
            for t in threads:
                t.join()
            threads = []

        if src_log_name in file:
            thread = myThread(file)
            thread.start()
            threads.append(thread)
            i += 1
            print 'start the %d thread' % (i,)



    print 'doTask() over'

if __name__ == '__main__':
    while True:
        doTask()
        time.sleep(sleep_time)
