#!/usr/bin/env python2
#encoding=gbk

import urllib2

import getopt, sys, time, os, re




def is_in_list(item, out_list):
    #tmp = unicode(item, 'gb2312')
    for key in out_list:
        #if key in item:
        #keystr = unicode(key, 'gb2312')
        #if tmp.find(keystr) != -1:
        if key in item:
            #print key
            return key

    return ''


def get_out_str(tmp_dict, out_list):
    sb = ''
    for key in out_list:
        if key in tmp_dict.keys():
            sb +='%s\t' % (tmp_dict[key])

    return sb

def job(line, fp, out_dict, out_list):

    items = line.split('\t')
    #print items
    count = 0
    tmp_dict = {}
    for item in items:
        out_item = is_in_list(item, out_list)
        if out_item.strip():
            out_dict[count] = out_item

        if count in out_dict.keys():
            tmp_dict[out_dict[count]] = item

        count += 1
    sb = get_out_str(tmp_dict, out_list)

    if sb.strip() and count >= len(out_list):
        sb += '\n'
        fp.write(sb)




def main():


    date = time.strftime('%Y%m%d',time.localtime(time.time()))
    saveFileName = '%s_stock_list' % (date)
    fp = open(saveFileName, 'w+')

    out_list = ['代码', '名称', '流通市值', '流通股本', '市盈', '行业', '现价']
    out_dict = {}
    for line in sys.stdin:
        line = line.strip()

        if not line:
            continue

        job(line, fp,out_dict, out_list)
    fp.close()
    #sprint saveFileName



if __name__ == '__main__':
    #print os.getcwd()
    main()

