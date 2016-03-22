#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8


import sys, re


ipdict = {}
ipXdict = {}
siddict = {}


def get_hour(line):
    items = line.split(" ")
    p = r'(\d{4}:\d{2}:\d{2}:\d{2})'
    for subitems in items:
        tmp = re.findall(p, subitems)
        if len(tmp) == 1:
            print tmp
            break

    time_items = tmp.split(":")
    if len(time_items) < 4:
        return
    print time_items[2]

def get_sid(line):
    items = line.split("sid=")
    if "sid=" not in line:
        return
    subitems = items[1].split("&")

    print subitems[0]
    if siddict.has_key(subitems[0]):
        values = siddict[subitems[0]]
        values += 1
        siddict[subitems[0]] = values
    else:
        values = 1
        siddict[subitems[0]] = values


def print_sid():
    for k in siddict:
        print "%s\t%d" %(k, siddict[k])



def handle_asp_log(line):
    items = line.split(" ")
    p = r'(?<![\.\d])(?:\d{1,3}\.){3}\d{1,3}(?![\.\d])'
    tmp = re.findall(p,  items[len(items)-1].strip("\""))
    #print tmp
    key = ""
    keyx = ""
    subitems = items[0].split(".")
    keyx = "%s.%s.%s" % (subitems[0], subitems[1], subitems[2])
    if len(tmp) >= 1:
        key = "1_%s" % (items[0])
        keyx = "1_%s" % (keyx)
    else:
        key = "0_%s" % (items[0])
        keyx = "0_%s" % (keyx)

    key = key.strip("\'")
    keyx = keyx.strip("\'")
    if ipdict.has_key(key):
        values = ipdict[key]
        values += 1
        ipdict[key] = values
    else:
        values = 1
        ipdict[key] = values



    if ipXdict.has_key(keyx):
        values = ipXdict[keyx]
        values += 1
        ipXdict[keyx] = values
    else:
        values = 1
        ipXdict[keyx] = values
    #print items[0], items[len(items)-1].strip("\"")


if '__main__' == __name__:
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue

        if "bd.jsp" in line and "m.baidu.com/cpro.php" in line:
            #handle_asp_log(line)
            #get_sid(line)
            get_hour(line)

    #for k in ipdict:
    #    print "%s\t%d" %(k, ipdict[k])
    #
    #
    #for k in ipXdict:
    #    print "%s\t%d" %(k, ipXdict[k])


    print_sid()