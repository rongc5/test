#!/usr/bin/env python
#encoding=utf-8


import urllib2
import json, os
import subprocess
import sys
import time
import hashlib

__author__ = 'mingz'

BASE_URL = 'http://10.26.24.86:6011/recomm.json?recommType'
REDIS_CLI = 'redis-3.0.6/redis-cli'
REDIS1_NS = '10.26.25.15:6645'
REDIS1_AUTH = 'Easou_RDS2017'
REDIS2_NS='10.26.24.66:7000'
RECOMM_APPID_EASOU2  =  20001
RECOMM_APPID_EASOU = 10001

RECOMM_ROOT_INTERVAL_TIME = 86400 * 3

RECOMM_UCF_MASK_FLAG = 127
RECOMM_UCF_ITEMINFO_NUM = 36

test_list = [{'uid':'48808231', 'reqGid':''}]

#获取用户类型
#1 免费
#2 准付费
#3 付费
#4 VIP付费
def get_user_type(search_key_dic):
    user_type = get_data_redis2(search_key_dic['cid1_key'], 'user_type')

    return user_type

def get_data_redis1(key, field):
    ip = REDIS1_NS.split(':')[0]
    port = REDIS1_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-a', REDIS1_AUTH, '-c', 'hget', key, field]
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    res_str = res_str.split('\n')[0]

    return res_str

def get_data_redis2(key, field):
    ip = REDIS2_NS.split(':')[0]
    port = REDIS2_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', key, field]
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    res_str = res_str.split('\n')[0]
    return res_str


def get_iteminfo(key):
    ip = REDIS1_NS.split(':')[0]
    port = REDIS1_NS.split(':')[1]
    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-a', REDIS1_AUTH, '-c', 'hgetall', key]
    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    res_str = res_str.split('\n')[0]

    items = res_str.split('\t')
    length = len(items)
    i = 0
    res_dic = {}
    while True:
        if i + 1>= length:
            break

        res_dic[items[i]] = items[i + 1]
        i = i + 2

    return res_dic

#1 包月前期
#2 包月中期
#3 包月末期
#4 包月结束
#5 流失包月用户
def get_baoyue(search_key_dic):
    baoyue = get_data_redis2(search_key_dic['cid1_key'], 'baoyue')

    return baoyue


#历史书架
#uid1_key u_48808231
#cid1_key cu_54159287
def get_history_user_cart(search_key_dic):
    res_str = get_data_redis2(search_key_dic['uid1_key'], 'cart_item')

    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')
    if not len(res_list):
        res_str = get_data_redis2(search_key_dic['cid1_key'], 'cart_item')
        if res_str.strip():
            res_list = res_str.split('\t')

    return res_list

#实时书架
#tid1_key t_54159287
def get_current_user_cart(search_key_dic):
    res_str = get_data_redis2(search_key_dic['tid1_key'], 'cart_item')

    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    i = 0;
    length = len(res_list)
    valid_time = int(time.time()) - RECOMM_ROOT_INTERVAL_TIME
    user_cart = []
    while True:
        if i + 1 >= length:
            break

        if res_list[i+1] < valid_time:
            continue

        user_cart.append(res_list[i])
        i = i + 2

    return user_cart

#tid3_key t_0EFF2921445097DC49E46156D5F6A18C
#付费协同  field  ucf_knn3
def get_current_ucf(search_key_dic, field='ucf_knn3'):
    res_str = get_data_redis2(search_key_dic['tid1_key'], field)
    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    if not len(res_list):
        res_str = get_data_redis2(search_key_dic['tid3_key'], field)
        if res_str.strip():
            res_list = res_str.split('\t')

    return res_list

def recomm_ucf_select_category(items_dic):

    key_array = {}
    for key in items_dic:
        if not items_dic[key].has_key('category1_sign') or not items_dic[key]['category1_sign'].strip():
            continue

        if items_dic[key]['category1_sign'] in key_array:
            key_array['wei_u41'] += 1
        else:
            key_array['category1_sign'] = items_dic[key]['category1_sign']
            key_array['wei_u41'] = 1

    category_sign = ''
    rid = 0
    for key in key_array:
        if key['wei_u41'] > rid:
            rid = key['wei_u41']
            category_sign = key['category1_sign']

    return  category_sign


def recomm_ucf_norm_filter(items_dic):

    ks_hmap = {}
    for key in items_dic:
        if not items_dic[key].has_key('norm_name') or not items_dic[key]['norm_name'].strip():
            continue

        if not items_dic[key].has_key('norm_author') or not items_dic[key]['norm_author'].strip():
            continue

        key_buf = '%s_%s' % (items_dic[key]['norm_name'], items_dic[key]['norm_author'])
        if key_buf in ks_hmap:
            items_dic[key]['mask_level'] = RECOMM_UCF_MASK_FLAG
            continue

        ks_hmap[key_buf] = ''


def recomm_ucf_cart_format(items_dic):

    remove_key = []
    for key in items_dic:
        if items_dic[key].has_key('mask_level') and items_dic[key]['mask_level'] == RECOMM_UCF_MASK_FLAG:
            remove_key.append(key)
            continue

        tv_now = int(time.time()) + (3600 << 3)
        if items_dic[key]['wei_u81'] <= tv_now:
            tv_now = tv_now - items_dic[key]['wei_u81']
        tv_now = tv_now / 86400
        if tv_now > 1095:
            tv_now = 1095

        items_dic[key]['base_weight'] = (2 << 10) / (1 + tv_now)

    for key in remove_key:
        items_dic.pop(key)

def recomm_dao_idata_bget(items_dic, field):

    for key in items_dic:
        res_str = get_data_redis2('gid', field)
        res_list = []
        if res_str.strip():
            res_list = res_str.split('\t')
            items_dic[key]['idata'] = res_list


def recomm_ucf_accumulation_weight(items_dic):

    ks_hmap = {}
    for key in items_dic:
        if not items_dic[key].has_key('idata'):
            continue
        length = len(items_dic[key]['idata'])
        i = 0
        while True:
            if i + 1 >= length:
                break

            if items_dic[key]['idata'][i] in ks_hmap:
                ks_hmap[items_dic[key]['idata'][i]] += items_dic[key]['idata'][i + 1] * items_dic[key]['base_weight']
            else:
                ks_hmap[items_dic[key]['idata'][i]] = int(items_dic[key]['idata'][i + 1]) * items_dic[key]['base_weight']
            i += 2

    return ks_hmap


def recomm_ucf_sort_weight(ks_hmap):
    d_view = [ (v,k) for k,v in ks_hmap.iteritems()]
    d_view.sort(reverse=True)
    res_list = []
    for v,k in d_view:
        res_list.append(k)

    return res_list

def recomm_ucf_select_topn(items_dic, res_list, cut_count):

    ks_hmap = {}
    #过滤书架
    for key in items_dic:
        if not items_dic[key].has_key('top_name') or not items_dic[key]['top_name'].strip():
            continue

        if items_dic[key]['top_name'] not in ks_hmap:
            ks_hmap[items_dic[key]['top_name']] =''


    cut_id = 0
    r_index = 0
    while  cut_id < cut_count:
        item_id = 0;
        while item_id < RECOMM_UCF_ITEMINFO_NUM:
            items = get_iteminfo(res_list[r_index])
            items['gid'] = res_list[r_index]
            ++item_id
            ++r_index

        item_count = item_id
        item_id = 0
        while item_id < item_count:
            



#uid1_key  u_54159287
#uid3_key  d_0EFF2921445097DC49E46156D5F6A18C
def get_history_ucf(search_key_dic, field='ucf_knn3'):
    #uk_version = 0
    #uk_version_str = get_data_redis1('i_version', 'uk_version')
    #if uk_version_str.strip():
    #    uk_version = int(uk_version_str)




    if 'ucf_knn4' in field:
        field = 'icf_knn4'
    elif 'ucf_knn3' in field:
        field = 'icf_knn3'
    elif 'ucf_knn2' in field:
        field = 'icf_knn2'
    elif 'ucf_knn' in field:
        field = 'icf_knn'

    cart_arr = get_history_user_cart(search_key_dic)
    cart_items_dic = {}
    i = 0
    length = len(cart_arr)
    while True:
        if i + 1 >= length:
            break

        item = get_iteminfo(cart_arr[i])
        item['gid'] = cart_arr[i]
        item['wei_u81'] = cart_arr[i+1]
        cart_items_dic[cart_arr[i]] = item
        i = i + 2

    cart_topn = 100
    category_sign = ''
    if field == 'icf_knn4':
        category_sign = recomm_ucf_select_category(cart_items_dic)



    #1、在线计算， 获取用户历史书架
    #redis-3.0.6/redis-cli --raw -a Easou_RDS2017 -h 10.26.25.15 -p 6645 -c hgetall i_100056338
    #2、获取书籍的信息
    #3、若为限免协同， 取一级类别比重最高的类别category_sign
    #4、recomm_ucf_cart_topn re.conf 中配置为100
    #5、根据  item_info->nname_sign, item_info->nauthor_sign 过滤

    #res_list = []
    #if res_str.strip():
    #    res_list = res_str.split('\t')
    #
    #if not len(res_list):
    #    cmd = [REDIS_CLI, '--raw', '-h', ip, '-p', port, '-c', 'hget', 'd_%s' % (udid), field]
    #    res_str = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    #    res_str = res_str.strip('\n')
    #    if res_str.strip():
    #        res_list = res_str.split('\t')

    #return res_list



#免费流行度
def get_free_pop(search_key_dic):
    res_str = get_data_redis2(search_key_dic['uid4_key'], 'pop_topn')
    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    return res_list

#付费流行度
def get_pay_pop(search_key_dic):
    res_str = get_data_redis2(search_key_dic['uid2_key'], 'pop_topn')
    res_list = []
    if res_str.strip():
        res_list = res_str.split('\t')

    return res_list

#ik_version
#1501745183
#uk_version
#1501745183
#ik4_version
#1508112000
#uk4_version
#1508112000
#tf1
#4
#tf2
#4

#uid4_key   v_0001
def get_search_key(search_dic):
    id_dic = {}
    id_dic['uid2_key'] = ''
    id_dic['uid4_key'] = ''

    if search_dic.has_key('uid') and int(search_dic['uid']) > 0:
        id_dic['uid1_key'] = 'u_%s' % (search_dic['uid'])
        id_dic['cid1_key'] = 'cu_%s' % (search_dic['uid'])
        id_dic['tid1_key'] = 't_%s' % (search_dic['uid'])

        if search_dic.has_key('udid') and search_dic['udid'].strip():
            id_dic['uid3_key'] = 'd_%s' % (search_dic['udid'])
            id_dic['cid3_key'] = 'cd_%s' % (search_dic['udid'])
            id_dic['tid3_key'] = 't_%s' % (search_dic['udid'])

    elif search_dic.has_key('udid') and search_dic['udid'].strip():
        id_dic['uid1_key'] = 'd_%s' % (search_dic['udid'])
        id_dic['cid1_key'] = 'cd_%s' % (search_dic['udid'])
        id_dic['tid1_key'] = 't_%s' % (search_dic['udid'])


    if search_dic.has_key('reqGid') and search_dic['reqGid'].strip():
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['iid1_key'] = 'ns2_i_%s' % (search_dic['reqGid'])
        else:
            id_dic['iid1_key'] = 'i_%' (search_dic['reqGid'])

        id_dic['iid10_key'] = 'i_%' (search_dic['reqGid'])

    if search_dic['recomm_type'] == 203:
        if search_dic['req_feature'] == 1:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0102'
            else:
                id_dic['uid2_key'] = 'v_0102'
        elif search_dic['req_feature'] == 2:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0103'
            else:
                id_dic['uid2_key'] = 'v_0103'
        elif search_dic['req_feature'] == 3:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0107'
            else:
                id_dic['uid2_key'] = 'v_0107'
        elif search_dic['req_feature'] == 11:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0111'
            else:
                id_dic['uid2_key'] = 'v_0111'
        elif search_dic['req_feature'] == 12:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0112'
            else:
                id_dic['uid2_key'] = 'v_0112'
        elif search_dic['req_feature'] == 13:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0113'
            else:
                id_dic['uid2_key'] = 'v_0113'
        elif search_dic['req_feature'] == 14:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0114'
            else:
                id_dic['uid2_key'] = 'v_0114'
        elif search_dic['req_feature'] == 21:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0121'
            else:
                id_dic['uid2_key'] = 'v_0121'
        elif search_dic['req_feature'] == 22:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0122'
            else:
                id_dic['uid2_key'] = 'v_0122'
        elif search_dic['req_feature'] == 23:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0123'
            else:
                id_dic['uid2_key'] = 'v_0123'
        elif search_dic['req_feature'] == 24:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0124'
            else:
                id_dic['uid2_key'] = 'v_0124'
    elif search_dic['recomm_type'] == 205:
        if search_dic['req_feature'] == 0:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0201'
            else:
                id_dic['uid2_key'] = 'v_0201'
        elif search_dic['req_feature'] == 1:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0202'
            else:
                id_dic['uid2_key'] = 'v_0202'
        elif search_dic['req_feature'] == 2:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0203'
            else:
                id_dic['uid2_key'] = 'v_0203'
    elif search_dic['recomm_type'] == 206:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['uid2_key'] = 'ns2_v_0301'
        else:
            id_dic['uid2_key'] = 'v_0301'
    elif search_dic['recomm_type'] == 207:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['uid2_key'] = 'ns2_v_0001'
            id_dic['uid4_key'] = 'ns2_v_0301'
        else:
            id_dic['uid2_key'] = 'v_0001'
            id_dic['uid4_key'] = 'v_0301'
    elif search_dic['recomm_type'] == 208:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['uid2_key'] = 'ns2_v_0101'
        else:
            id_dic['uid2_key'] = 'v_0101'
    elif search_dic['recomm_type'] == 209:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            if search_dic['req_charge'] == 1:
                id_dic['uid2_key'] = 'ns2_v_0101'
            else:
                id_dic['uid2_key'] = 'ns2_v_0001'
        else:
            if search_dic['req_charge'] == 1:
                id_dic['uid2_key'] = 'v_0101'
            else:
                id_dic['uid2_key'] = 'v_0001'
    elif search_dic['recomm_type'] == 101:
        if search_dic['req_feature'] != 0:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_%04d' % (search_dic['req_feature'])
            else:
                id_dic['uid2_key'] = 'v_%04d' % (search_dic['req_feature'])
        else:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0101'
            else:
                id_dic['uid2_key'] = 'v_0101'
    elif search_dic['recomm_type'] == 301 or search_dic['recomm_type'] == 302 or search_dic['recomm_type'] == 3011 or search_dic['recomm_type'] == 3021:
        if search_dic['req_charge'] == 1:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0101'
            else:
                id_dic['uid2_key'] = 'v_0101'
        else:
            if search_dic['appid'] == RECOMM_APPID_EASOU2:
                id_dic['uid2_key'] = 'ns2_v_0001'
            else:
                id_dic['uid2_key'] = 'v_0001'
    elif search_dic['recomm_type'] == 201:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['uid2_key'] = 'ns2_v_0101'
            id_dic['uid4_key'] = 'ns2_v_0301'
        else:
            id_dic['uid2_key'] = 'v_0101'
            id_dic['uid4_key'] = 'v_0301'

    else:
        if search_dic['appid'] == RECOMM_APPID_EASOU2:
            id_dic['uid2_key'] = 'ns2_v_0101'
        else:
            id_dic['uid2_key'] = 'v_0101'

    return id_dic

def test_201(search_dic):
    #print search_dic
    if not len(search_dic):
        return False

    url = create_url(search_dic)
    print url
    res_dic = {}
    try:
        f = urllib2.urlopen(url)
        res_dic = json.loads(f.read())
    except Exception, e:
        print e
        return False

    user_type = 0
    if search_dic.has_key('uid'):
        user_type = get_user_type(search_dic['uid'])

    print user_type

    history_cat = []
    history_cat = get_history_user_cart(search_dic['uid'])
    #current_user_cart = get_current_user_cart(search_dic['uid'])

    t_ucf_knn4 = get_current_ucf(search_dic['uid'], search_dic['udid'], 'ucf_knn4')
    t_ucf_knn3 = get_current_ucf(search_dic['uid'], search_dic['udid'], 'ucf_knn3')

    ucf_knn4 = get_history_ucf(search_dic['uid'], search_dic['udid'], 'ucf_knn4')
    ucf_knn3 = get_history_ucf(search_dic['uid'], search_dic['udid'], 'ucf_knn3')

    for key in res_dic:
        if key in history_cat:
            return False


def create_url(search_dic):
    url = ''

    if not len(search_dic):
        return url

    url = '%s=%s' % (BASE_URL, search_dic['recommType'])

    if search_dic.has_key('uid'):
        url += '&uid=%s' % (search_dic['uid'])

    if search_dic.has_key('udid'):
        url += '&udid=%s' % (search_dic['udid'])

    if search_dic.has_key('start') and search_dic['start'] > 0:
        url += '&start=%d' % (search_dic['start'])

    if search_dic.has_key('cid'):
        url += '&cid=%s' % (search_dic['cid'])

    if search_dic.has_key('count') and search_dic['count'] > 0:
        url += '&count=%d' % (search_dic['count'])

    if search_dic.has_key('random') and search_dic['random'] > 0:
        url += '&random=%d' % (search_dic['random'])

    if search_dic.has_key('feature') and search_dic['feature'] > 0:
        url += '&feature=%d' % (search_dic['feature'])

    if search_dic.has_key('charge') and search_dic['charge'] > 0:
        url += '&charge=%d' % (search_dic['charge'])

    if search_dic.has_key('appid'):
        url += '&appid=%s' % (search_dic['appid'])

    print url
    return url


def lod_request_list(path):
    res_list = []
    if not os.path.isfile(path):
        return res_list

    file = open(path)
    while 1:
        line = file.readline().strip()
        if not line:
            break

        res_dic = json.loads(line)
        #print res_dic
        if res_dic.has_key('guessResGids'):
            res_dic.pop('guessResGids')

        if res_dic.has_key('categoryRecGids'):
            res_dic.pop('categoryRecGids')
        res_list.append(res_dic)

    file.close()
    return res_list




if __name__ == '__main__':
    pass
    #for line in sys.stdin:
    #    line = line.strip()
    #    if not line:
    #        continue
    #    item = line.split('_')
    #    if len(item) != 2:
    #        continue
    #    if len(get_current_user_cart(item[1])) and not len(get_history_user_cart(item[1])):
    #        print item[1]


    #res_list = []
    #res_list = lod_request_list('response.log')
    #for key in res_list:
    #    if key.has_key('recommType')and key['recommType'] == 201:
    #        test_201(key)