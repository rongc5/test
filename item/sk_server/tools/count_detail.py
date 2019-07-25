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
    flag = False
    cmd = ['grep', os.path.basename(id), filename]
    tmp_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    if not os.path.isfile(filename) or not tmp_str.strip():
        flag = True

    if not flag:
        return

    middle_in = 0
    middle_out = 0

    price_middle = 0

    large_in = 0
    large_out = 0

    price_large = 0

    king_in = 0
    king_out = 0

    price_king = 0

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
        #if id.startswith('sz') and d.time() > e.time():
            #print items[0]
            #continue
        price = float(items[1]) * int(items[2])
        if price >= 3000:
            if 'B' in items[3]:
                middle_in += int(items[2])
                price_middle += price
            elif 'S' in items[3]:
                middle_out += int(items[2])
                price_middle += price
            else:
                continue

        if price >= 10000:
            if 'B' in items[3]:
                large_in += int(items[2])
                price_large += price
            elif 'S' in items[3]:
                large_out += int(items[2])
                price_large += price
            else:
                continue

        if price >= 20000:
            if 'B' in items[3]:
                king_in += int(items[2])
                price_king += price
            elif 'S' in items[3]:
                king_out += int(items[2])
                price_king += price
            else:
                continue

    file.close()

    price_middle = get_price(price_middle, middle_in + middle_out)

    price_large = get_price(price_large, large_in + large_out)

    price_king = get_price(price_king, king_in + king_out)

    retstr = '%s\t' % (os.path.basename(id))

    retstr += '%d\t' %(middle_in)
    retstr += '%d\t' %(middle_out)
    retstr += '%.2f\t' %(price_middle)

    retstr += '%d\t' %(large_in)
    retstr += '%d\t' %(large_out)
    retstr += '%.2f\t' %(price_large)

    retstr += '%d\t' %(king_in)
    retstr += '%d\t' %(king_out)
    retstr += '%.2f' %(price_king)

    log_write(filename, retstr)

CURRENT_FILE_PATH = os.path.abspath(__file__)
CURRENT_DIR_PATH = os.path.abspath(os.path.dirname(CURRENT_FILE_PATH) + os.path.sep + ".")
BASE_PATH=os.path.abspath(os.path.dirname(CURRENT_DIR_PATH))
BASE_DATA_PATH=BASE_PATH + '/data/'
DATAPATH = BASE_DATA_PATH + 'singles'
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
        file_name = '%s/%s%s' % (DATAPATH, 'singles_', os.path.basename(sys.argv[1]))
        if os.path.isfile(file_name):
            os.remove(file_name)
        files= os.listdir(sys.argv[1])
        for file in files:
            #print os.path.abspath(file)
            if os.path.isfile(sys.argv[1] + '/' + file):
                #print file_name
                count_id_detail(sys.argv[1] + '/' + file, file_name)

