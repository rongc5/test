#!/usr/bin/env python
#encoding=utf-8

import os
import sys
import time
import subprocess
import datetime

def log_write(filename, str):
    file = open(filename, 'a')
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

def get_price(value, vol):
    if not vol or not value:
        return 0

    return round(value * 1.0/vol, 2)


def count_id_detail(id, filename):
    middle_in = 0
    middle_out = 0

    price_middle_in = 0
    price_middle_out = 0

    large_in = 0
    large_out = 0

    price_large_in = 0
    price_large_out = 0

    king_in = 0
    king_out = 0

    price_king_in = 0
    price_king_out = 0

    file = open(id)
    b = datetime.datetime.strptime('9:30', '%H:%M')
    e = datetime.datetime.strptime('14:57', '%H:%M')

    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')
        if len(items) <4:
            continue

        if not int(items[2]):
            continue
        d = datetime.datetime.strptime(items[0], '%H:%M:%S')
        if d.time() < b.time():
            #print items[0]
            continue
        if id.startswith('sz') and d.time() > e.time():
            #print items[0]
            continue
        price = float(items[1]) * int(items[2])
        if price >= 3000:
            if 'B' in items[3]:
                middle_in += int(items[2])
                price_middle_in += price
            elif 'S' in items[3]:
                middle_out += int(items[2])
                price_middle_out += price
            else:
                continue

        if price >= 10000:
            if 'B' in items[3]:
                large_in += int(items[2])
                price_large_in += price
            elif 'S' in items[3]:
                large_out += int(items[2])
                price_large_out += price
            else:
                continue

        if price >= 20000:
            if 'B' in items[3]:
                king_in += int(items[2])
                price_king_in += price
            elif 'S' in items[3]:
                king_out += int(items[2])
                price_king_out += price
            else:
                continue

    file.close()

    price_middle_in = get_price(price_middle_in, middle_in)
    price_middle_out = get_price(price_middle_out, middle_out)

    price_large_in = get_price(price_large_in, large_in)
    price_large_out = get_price(price_large_out, large_out)

    price_king_in = get_price(price_king_in, king_in)
    price_king_out = get_price(price_king_out, king_out)


    retstr = '%s\t%d\t%d\t%d\t' % (os.path.basename(id), middle_in - middle_out, large_in - large_out,
    king_in - king_out)
    retstr += '%d\t%.2f\t' % (middle_in, price_middle_in)
    retstr += '%d\t%.2f\t' % (middle_out, price_middle_out)
    retstr += '%d\t%.2f\t' % (large_in, price_large_in)
    retstr += '%d\t%.2f\t' % (large_out, price_large_out)
    retstr += '%d\t%.2f\t' % (king_in, price_king_in)
    retstr += '%d\t%.2f\t' % (king_out, price_king_out)

    log_write(filename, retstr)

DATAPATH = './data/singles'
if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(0)

    path = '%s' % (DATAPATH)
    cmd = ['mkdir', '-p', path]
    subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    today = time.strftime('%Y%m%d',time.localtime(time.time()))
    if os.path.isfile(sys.argv[1]):
        file_name = '%s/%s%s' % (DATAPATH, 'singles_', today)
        count_id_detail(sys.argv[1], file_name)
    elif os.path.isdir(sys.argv[1]):
        files= os.listdir(sys.argv[1])
        for file in files:
            #print os.path.abspath(file)
            if os.path.isfile(sys.argv[1] + '/' + file):
                file_name = '%s/%s%s' % (DATAPATH, 'singles_', os.path.basename(sys.argv[1]))
                #print file_name
                count_id_detail(sys.argv[1] + '/' + file, file_name)

