#! /usr/bin/python
'''
Created on 2014-1-13
@author: jiang

map:
    asp: sid,ideaid,0,uid,...
    cdp: sid,ideaid,1

reduce:
    based on 0/1 to handle asp or cdp
'''
import sys
import urllib


class AspParser:
	#asp log parser
	word_map = {}
	def __init__(self):
		self.keyval = {}
		self.maps = {}
		self.logkeys = ["sid", "time","esid","uid","pd","pt","sty","cid","ua","wvar","op_type", "pid","city", "np", "muid", "mmid","ip","sim", \
		"mid","dl"]
		#             userid,plainid, unitid, ideaid, mt, cmatch, showtype
		self.adkeys = [2,        3,    4,      7,     16,   19,      24]
	def load_word_map(self, bidword_file):
		for line in file(bidword_file):
			line = line.strip()
			items = line.split("\t")
			if len(items) < 2:
				continue
			self.word_map.setdefault(items[0], items[1])

	def get_log_items(self):
		vals = []
		for key in self.logkeys:
			val = self.maps.get(key, "")
			if val == "":
				val = "null"
			vals.append(val)
		return "\t".join(vals)

	def get_ad_items(self, adinfo):
		items = adinfo.split("\t")
		if len(items) < 25:
			return (None, None)
		vals = []
		for key in self.adkeys:
			val = items[key]
			if val == "":
				val = "null"
			vals.append(val)
		return (items[7], "\t".join(vals))

	#print asp log: sid,ideaid,uid,...
	def parse(self, line):
		if line.strip() == "":
			return
		istart = line.find('show_ver=')
		if -1 == istart:
			return
		ikdad = line.find(' kdad=', istart)
		iad = line.find(' ad=', istart)
		if ikdad == -1:
			return
		kdadinfo = ""
		adinfo = ""
		stlog = ""
		if ikdad != -1:
			stlog = line[istart:ikdad]
			if iad == -1:
				kdadinfo = line[ikdad+6:]
			else:
				kdadinfo = line[ikdad+6:iad]
		else:
			if iad == -1:
				stlog = line[istart:]
			else:
				stlog = line[istart:iad]
		if iad != -1:
			adinfo = line[iad+4:]

		self.maps.clear()
		for item in stlog.split(" "):
			key = item.split("=",1)[0]
			val = item[len(key)+1:]
			self.maps[key] = val
		if self.maps["uid"] == "":
			self.maps["uid"] = self.maps["esid"]
		vers = int(self.maps["show_ver"])
		if vers < 104:
			return
		#self.maps['ua'] = urllib.unquote_plus(self.maps['ua'])
		#self.maps['qry'] = urllib.unquote_plus(self.maps['qry'])
		time = int(line.split(" ", 3)[2].split(":")[0])
		self.maps["time"] = str(time)
		output_log = self.get_log_items()

		#kdadinfo = urllib.unquote_plus(kdadinfo)
		for ad in kdadinfo.split("|"):
			ideadid, output_ad = self.get_ad_items(ad)
			if ideadid:
				print "\t".join([self.maps["sid"], ideadid, "0", output_log, output_ad])

		#adinfo = urllib.unquote_plus(adinfo)
#		for ad in adinfo.split("|"):
#			ideadid, output_ad = self.get_ad_items(ad)
#			if ideadid:
#				print "\t".join([self.maps["sid"], ideadid, "0", output_log, output_ad])


class CifParser:
	#cdp log parser
	#print cdp log: sid,ideaid,1
	def parse(self, line):
		items = line.split("\t", 30)
		if len(items) < 30:
			return
		be=int(items[20])
		end=int(items[21])
		tim=int(end/1000-be)
		ips = items[14] + "_" + items[28]
		charge = items[11]
		print "\t".join([items[1], items[9], "1", str(tim), ips, charge])


def mapper():
	aspParser = AspParser()
	#aspParser.load_word_map(sys.argv[2])
	cifParser = CifParser()
	for line in sys.stdin:
		line = line.strip()
		if line=="":
			continue
		if line.startswith("NOTICE"):
			aspParser.parse(line)
		else:
			try:
				cifParser.parse(line)
			except:
				continue
#tim_ips add charge 

def reducer():
	last_sid = None
	last_ideaid = None
	last_asplog = None
	click = 0
	#cdp default data: clicktime, ips
	tim_ips = "0\t0\t0"
	for line in sys.stdin:
		line = line.strip()
		if line == "":
			continue
		fields = line.split("\t", 3)
		sid = fields[0]
		ideaid = fields[1]
		if "0" == fields[2]:
			#asp log
			if last_sid and (last_sid != sid or last_ideaid != ideaid):
				print "%s\t%s\t%d"%(last_asplog,tim_ips,click)
				click = 0
				tim_ips = "0\t0\t0"
			last_sid = sid
			last_ideaid = ideaid
			last_asplog = fields[3]
		elif "1" == fields[2]:
			#cdp log
			if last_sid == sid and last_ideaid == ideaid:
				click += 1
				tim_ips = fields[3]
	print "%s\t%s\t%d"%(last_asplog,tim_ips,click)

if __name__=='__main__':
	#if len(sys.argv) < 2:
	#	print "error param, usage: %s [-m] (or [-r])" % sys.argv[0]
	#	sys.exit()
	#if sys.argv[1] == "-m":
		mapper()
	#elif sys.argv[1] == "-r":
	#	reducer()
	#else:
	#	print "error param, usage: %s [-m] (or [-r])" % sys.argv[0]
	#	sys.exit()
