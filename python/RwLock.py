#coding=utf-8
#!/usr/bin/python

import threading
import time
import random
import time
import sys
import os

class RwLock():
    def __init__(self):
        self.rcount = 0
        self.wcount = 0
        self.rtotal = 0
        #self.mutex = threading.Lock()
        self.condition = threading.Condition()
        
    def rlock(self):
        self.condition.acquire()
        self.rtotal += 1
        if self.rtotal >=  10000:
            str = "%d %d" %  (self.rtotal, long(time.time()))
            os.system("echo %s >> stress_test.log" % str)
            self.rtotal = 0
        while self.wcount != 0:
            self.condition.wait()
        self.rcount += self.rcount
        self.condition.release()
        
    def wlock(self):
        self.condition.acquire()
        while self.rcount + self.wcount > 0:
            self.condition.wait()
        self.wcount += 1
        self.condition.release()
        
    def unrlock(self):
        self.condition.acquire()
        self.rcount -= self.rcount
        if self.rcount == 0:
            self.condition.notify() 
        self.condition.release()
        pass
    def unwlock(self):
        self.condition.acquire()
        self.wcount -= 1
        self.condition.notify()
        self.condition.release()

class LockTest():
    def __init__(self, rwlock):
        self.rwlock = rwlock
        self.value = 0
        
    def read(self, name):
        while True:
            self.rwlock.rlock()
            print "read: ", name
            #time.sleep(random.randint(1, 4))
            print "read: ", name, self.value
            self.rwlock.unrlock()
        
    def write(self, name):
        while True:
            self.rwlock.wlock()
            print "write: ", name
            #time.sleep(100)
            before_value = self.value
            self.value += 1
            print "write: ", name, before_value, self.value
            self.rwlock.unwlock()
      
if __name__ == "__main__":
    lock = RwLock()
    test_obj = LockTest(lock)
    read_thread_one = threading.Thread(target=test_obj.read, args=("read_thread_one",))
    read_thread_one.start()
    read_thread_two = threading.Thread(target=test_obj.read, args=("read_thread_two",))
    read_thread_two.start()
    read_thread_three = threading.Thread(target=test_obj.read, args=("read_thread_three",))
    read_thread_three.start()
    write_thread_one = threading.Thread(target=test_obj.write, args=("write_thread_one",))
    write_thread_one.start()
    while True:
        time.sleep(100)
