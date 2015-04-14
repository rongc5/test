import urllib
import urllib2

url = 'http://www.hao123.com/'

values = {'name' : 'WHY',
          'location' : 'SDU',
          'language' : 'Python' }

#data = urllib.urlencode(values)
req = urllib2.Request(url)
response = urllib2.urlopen(req)
the_page = response.read()
f = open("hao123",'w+')
f.write(the_page)
f.close()
