#!/usr/bin/python
# -*- coding: utf-8 -*-
# wangyang 2011-04-05


import sys, os, time, atexit, commands
from signal import SIGTERM

class Daemon:
	"""
	A generic daemon class.
	Usage: subclass the Daemon class and override the _run() method
	"""
	def __init__(self, pidfile, exefile , args=[] ,args2=[], stdin='/dev/null', stdout='/dev/null', stderr='/dev/null'):
		
		self.stdin = stdin
		self.stdout = stdout
		self.stderr = stderr
		stat, output = commands.getstatusoutput("pwd")

		self.pidfile = output + "/pid"
		self.exefile = exefile
		self.filename = os.path.basename(self.exefile)
		self.args = args[:]
		self.args2 = args2[:]
		self.argstr = self.args
		self.argstr2 = ' '.join(self.args2)
			

	def _daemonize(self):
		"""
		do the UNIX double-fork magic, see Stevens' "Advanced 
		Programming in the UNIX Environment" for details (ISBN 0201563177)
		http://www.erlenstar.demon.co.uk/unix/faq_2.html#SEC16
		"""
		if os.path.exists(self.exefile) != True:
			sys.stderr.write("exe file %s not exist \n" % self.exefile)
			sys.exit(1)

		try:
			pid = os.fork()
			if pid > 0:
				sys.exit(0)
		except OSError, e:
			sys.stderr.write("fork #1 failed: %d (%s)\n" % (e.errno, e.strerror))
			sys.exit(1)

		os.setsid()
		os.chdir("/")
		os.umask(0)

		try:
			pid = os.fork()
			if pid > 0:
				sys.exit(0)
		except OSError, e:
			sys.stderr.write("fork #2 failed: %d (%s)\n" % (e.errno, e.strerror))
			sys.exit(1)

		sys.stdout.flush()
		sys.stderr.flush()
		si = file(self.stdin, 'r')
		so = file(self.stdout, 'a+')
		se = file(self.stderr, 'a+', 0)

		os.dup2(si.fileno(), sys.stdin.fileno())
		os.dup2(so.fileno(), sys.stdout.fileno())
		os.dup2(se.fileno(), sys.stderr.fileno())

		atexit.register(self.delpid)
		pid = str(os.getpid())
		file(self.pidfile,'w+').write("%s\n" % pid)

	def delpid(self):
		os.remove(self.pidfile)

	def start(self):
		"""
		Start the daemon
		"""
		# Check for a pidfile to see if the daemon already runs
		try:
			pf = file(self.pidfile,'r')
			pid = int(pf.read().strip())
			pf.close()
		except IOError:
			pid = None

		if pid:
			message = "pidfile %s already exist. Daemon already running?\n"
			sys.stderr.write(message % self.pidfile)
			sys.exit(1)

		# Start the daemon
		self._daemonize()
		self._run()

	def stop(self):
		"""
		Stop the daemon
		"""
		# Get the pid from the pidfile
		try:
			pf = file(self.pidfile,'r')
			pid = int(pf.read().strip())
			pf.close()
		except IOError:
			pid = None

		if not pid:
			message = "pidfile %s does not exist. Daemon not running?\n"
			sys.stderr.write(message % self.pidfile)
			return # not an error in a restart
		# Try killing the daemon process    
		try:
			while 1:
				os.kill(pid, SIGTERM)
				time.sleep(0.1)
		except OSError, err:
			err = str(err)
			if err.find("No such process") > 0:
				if os.path.exists(self.pidfile):
					os.remove(self.pidfile)
			else:
				print str(err)
				sys.exit(1)

	def restart(self):
		"""
		Restart the daemon
		"""
		self.stop()
		self.start()

	def _run(self):
		"""
		Real exe file
		"""
		exefile = self.exefile + ""
		filename = self.filename + ""
		argstr = self.argstr + ""
		argstr2 = self.argstr2 + ""
		os.execl(exefile, filename, argstr,argstr2)





if __name__ == "__main__":
	if len(sys.argv) >= 2:
		if 'start' == sys.argv[1]:
			if len(sys.argv) == 3:
				daemon = Daemon('./pid', sys.argv[2])
			else:
				daemon = Daemon('./pid', sys.argv[2], sys.argv[3],sys.argv[4:])
				daemon.start()
		elif 'stop' == sys.argv[1]:
			daemon = Daemon('','')
			daemon.stop()
		elif 'restart' == sys.argv[1]:
			if len(sys.argv) == 3:
				daemon = Daemon('./pid', sys.argv[2])
			else:
				daemon = Daemon('./pid', sys.argv[2], sys.argv[3],sys.argv[4:])
			daemon.restart()
		else:
			print "Unknown command".sys.argv[1]
			sys.exit(2)
	else:
		print "usage: %s [start exefile exearg]|[stop]|[restart exefile exearg]" % sys.argv[0]
		sys.exit(2)

