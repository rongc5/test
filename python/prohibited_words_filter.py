#/usr/bin/env python
#encoding=gbk

#=============================================================================
#     FileName: count_pvr.py
#         Desc:
#       Author: zm
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2015-09-02 11:00:09
#      History:
#=============================================================================

import os, sys,MySQLdb, urllib
import threading, time

reload(sys)
sys.setdefaultencoding("gbk")

class Task:
    def __init__(self, file):
        #sys.setdefaultencoding("gbk")
        self.prohibit = prohibit(file)
        self.data_dict = {}
        self.ideaid_dict = {}
        self.ideaid_url_dict = {}
        self.unitid_dict = {}
        self.ideaid_name_dict = {}
        #self.data_src = ['ideaid','userid','planid','unitid','title', 'desc', 'adtype', 'show_type', 'spread_type']

    #def loadImage(self):
    #    for key in self.ideaid_url_dict:
    #        url = self.ideaid_url_dict[key]
    #        if not url.strip():
    #            continue
    #
    #        name  = self.ideaid_name_dict[key]
    #        image = LoadImage(url, name)
    #        image.getImage()

    def loadImageUrl(self):
        db = Database()
        count = 1
        for key in self.ideaid_dict:
            items = key.split('_')
            ideaid = items[0]
            row = db.queryUrlFromTdeaid(ideaid)
            if row is None:
                continue

            url = str(row[4])
            #self.ideaid_url_dict[key] = url

            name = '%s_%s_%s_%s' % (row[0], row[1], row[2], row[3])
            #self.ideaid_name_dict[key] = name
            image = LoadImage(url, name)
            t = threading.Thread(target=image.getImage())
            t.start()
            #image.getImage()
            print '''第 %d 条记录''' % (count,)
            count += 1
        self.ideaid_dict = {}

    def checkPictureIsProhibited(self):
        self.loadImageUrl()
        #self.loadImage()

    def getProhibitedTextInfo(self):
        db = Database()
        for key in self.unitid_dict:
            row = db.queryNameFromUnitid(key)
            if row is None:
                continue
            print row[0], '\t',row[1],'\t',row[2],'\t',row[3],'\t',row[4],'\t',row[5]
        self.unitid_dict = {}

    #打印是违禁内容的
    def checkTextIsProhibited(self):
        if self.prohibit.isContainingprohibitedwords(self.data_dict['desc']) or self.prohibit.isContainingprohibitedwords(self.data_dict['title']):
                #print  self.data_dict['title'], self.data_dict['desc']
                #print self.data_dict['unitid'], self.data_dict['planid'], self.data_dict['userid']
                #value = '%s/t%s/t%s/t%s/t%s/t%s/t' % (self.data_dict['userid'], self.data_dict['planid'], self.data_dict['unitid'],
                #self.data_dict['adtype'], self.data_dict['show_type'], self.data_dict['spread_type'])
                self.unitid_dict[self.data_dict['unitid']] = ''

    def do_one_line(self):
        self.data_dict = {}
        for line in sys.stdin:
            line = line.strip()
            if not line:
                continue

            items = line.split('\t')
            #for item in items:
            #   print 'hello ==>', item
            #print line
            self.data_dict['ideaid'] = items[3]
            self.data_dict['userid'] = items[4]
            self.data_dict['planid'] = items[5]
            self.data_dict['unitid'] = items[6]
            #self.data_dict['title'] = items[7]
            #self.data_dict['desc'] = items[8]
            self.data_dict['adtype'] = items[12]
            self.data_dict['show_type'] = items[13]
            #self.data_dict['spread_type'] = items[15]
            #if self.prohibit.isContainingprohibitedwords(data_dict['title']) or self.prohibit.isContainingprohibitedwords(data_dict['desc']):
            #    print  data_dict['title'], data_dict['desc']

            #self.checkTextIsProhibited()
            if int(self.data_dict['adtype']) == 11 or int(self.data_dict['show_type']) == 2 or int(self.data_dict['show_type']) == 1:
                key_str = '%s_%s_%s_%s' % (self.data_dict['ideaid'], self.data_dict['userid'], self.data_dict['planid'], self.data_dict['unitid'])
                self.ideaid_dict[key_str] = ""
            #self.ideaid_title_dict[key_str] = self.data_dict['title']


class LoadImage:
    def __init__(self, imageUrl, name):
        self.imageUrl = imageUrl
        self.name = name

    def getImageName(self):
        suffix = self.imageUrl.rfind('.')
        suffix = self.imageUrl[suffix:]
        self.name = './images/%s%s' % (self.name, suffix)
        #print self.name
    def getImage(self):
        self.getImageName()
        #conn = urllib.request.urlopen(self.imageUrl)
        #f = open(self.name,'wb')
        #f.write(conn.read())
        #f.close()
        try:
            urllib.urlretrieve(self.imageUrl, self.name)
        except:
            pass
class Database:
    def __init__(self):
        self.conn = MySQLdb.Connection(host="10.25.13.21", port=3721, user="readonly", passwd="read@easou", charset="gbk")
        self.conn.select_db("jingjia")
        self.cursor = self.conn.cursor()

        S1="use jingjia;"
        self.cursor.execute(S1)


    def queryUrlFromTdeaid(self, ideaid):
        #sql = 'select link_href1 from t_ad_ext WHERE ad_id = %s' % (ideaid,)

        sql = '''
SELECT t_base_user.`uname`, t_plan.`planname`,t_unit.`unitname`,t_ad.`title`, t_ad_ext.`link_href1`
FROM t_ad
JOIN t_ad_ext ON t_ad_ext.`ad_id` = t_ad.`id`
JOIN t_unit ON  t_unit.`id` =t_ad.`unitid`
JOIN t_plan ON t_plan.id = t_ad.`planid`
JOIN t_base_user ON t_base_user.id = t_ad.`userid`
JOIN t_user ON t_user.`userid` = t_ad.`userid`
WHERE t_ad.id = %s''' % (ideaid, )


        self.cursor.execute(sql)

        row = self.cursor.fetchone()

        return row

        #rows = self.cursor.fetchall()
        #for row in rows:
        #    ip_cnt = ""
        #for r in row:
        #        ip_cnt += str(r)
        #        ip_cnt += "\t"
        #print "%s" % ip_cnt


    def queryNameFromUnitid(self, unitid):

        sql = '''SELECT
t_base_user.`uname`,t_plan.`planname`, t_unit.`unitname`,t_user.`userstat`,t_plan.`planstat`, t_unit.`unitstat`
FROM t_unit
JOIN t_plan
ON t_plan.id = t_unit.`planid`
JOIN t_base_user
ON t_base_user.id = t_unit.`userid`
JOIN t_user
ON t_user.`userid` = t_unit.`userid`
WHERE  t_unit.id = %s
''' % (unitid, )

        self.cursor.execute(sql)

        row = self.cursor.fetchone()

        return row


    def __del__(self):
        self.cursor.close()
        self.conn.close()



class prohibit:
    def __init__(self, file):
        self.prohibit = set()
        self.init_from_file(file)

    def isContainingprohibitedwords(self, words):
        for w in self.prohibit:
            if w in words:
		#print w, words
                return True
        return False


    def init_from_file(self, file):
        fp_src = open(file, "r")
        for line in fp_src:
            line = line.strip()
            if not line or line in self.prohibit:
                continue

            self.prohibit.add(line)


def changeDir():
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    dirs = os.listdir(path)

if __name__ == '__main__':
    changeDir()
    os.system('rm -rf images')
    os.system('mkdir -p images')
    task = Task('forbid.txt')
    task.do_one_line()
    #task.getProhibitedTextInfo()
    task.checkPictureIsProhibited()
    while True:
        time.sleep(10)
        print "over !!!!!!!!!"
