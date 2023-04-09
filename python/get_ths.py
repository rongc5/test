#!/usr/bin/env python
#coding=utf-8
import json
import re
import execjs
import requests
import requests.utils
import  urllib
import sys
import io

#hexin_v
hexin_v = "Ax3j3JYwIseH9sbrAl0b7ROPKvISOlGMW261YN_iWXSjljMsp4phXOu-xTVs"
log_id= "1b93de30ae17862ce6620e64d8149ad7"
token_time_text="var TOKEN_SERVER_TIME=1669390000.384;"

#url="http://www.iwencai.com/customized/chart/get-robot-data"
url="http://www.iwencai.com/unifiedwap/unified-wap/v2/result/get-robot-data"
#url="http://www.iwencai.com/gateway/urp/v7/landing/getDataList"


#更新hexin-v
def update_time():
    global hexin_v
    token_time_url = requests.post(url=url,headers = headers,data=urllib.urlencode(data)).text
    #print token_time_url
    url_js = re.compile('<script src="(?P<jsurl>.*?)" type=',re.S)
    url_js_t = "http:"+url_js.search(token_time_url).group("jsurl")
    print url_js_t
    tt = requests.get(url_js_t).text
    #print tt
    token_time_text = tt[:tt.find(";")+1]
    print token_time_text
    hexin_v = get_hexin(token_time_text)
    print hexin_v

    print("已更新hexin-v与时间")

def load_from_file():
    with open("token_time.txt","r")as toke:
        token_time = toke.readline()
        hexin_v = toke.readline()
        log_id = toke.readline()
        print(hexin_v)
        print(token_time)
        print(log_id)

def load_to_file():
    with open("token_time.txt","w")as fa:
        fa.write(token_time_text+"\n"+hexin_v+"\n"+log_id+"\n")

data = {
        "question":"2022年11月13日人气排名",
        "source":"ths_mobile_iwencai",
        "user_id": "0",
        "user_name": "0",
        "secondary_intent":"",
        "log_info":{"input_type":"typewrite"},
        "source":"ths_mobile_iwencai",
        "version":"2.0",
        "add_info":{"urp":{"scene":3,"company":1,"business":1},"contentType":"json"},
        "log_info": {"input_type":"click"},
        "rsh": "Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69",
}

def get_datas(question):
    s1 = 'question='
    #s1 += urllib.quote("2022年11月13日人气排名".decode('gbk').encode('utf-8'))
    s1 += urllib.quote(question.decode('gbk').encode('utf-8'))
    s1 += '&source='
    s1 += 'ths_mobile_iwencai'
    s1 += '&user_id='
    s1 += '0'
    s1 += '&user_name='
    s1 += '0'
    s1 += '&secondary_intent='
    s1 += '&log_info='
    s1 += urllib.quote('''{"input_type":"typewrite"}''')
    s1 += '&source='
    s1 += 'ths_mobile_iwencai'
    s1 += '&version='
    s1 += '2.0'
    s1 += '&add_info='
    s1 += urllib.quote('''{"urp":{"scene":3,"company":1,"business":1},"contentType":"json"}''')
    s1 += '&log_info='
    s1 += urllib.quote('''{"input_type":"click"}''')
    s1 += '&rsh='
    s1 += 'Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69'

    print s1
    return s1


def get_headers(hexin_v, log_id, length):

    headers = {
            "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36",
            "Content-Type": "application/json",
            "Host": "www.iwencai.com",
            "Origin": "http://www.iwencai.com",
            "DNT": "1",
            "Content-Length":length,
            "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
            "content-type": "application/x-www-form-urlencoded",
            "Referer": "http://www.iwencai.com/unifiedwap/result?w=20221030%E6%B6%A8%E5%81%9C",
            "cookie": "chat_bot_session_id=" + log_id + "; other_uid=Ths_iwencai_Xuangu_wqmd2x6n9qa2wxanptvasnri7x656m69; ta_random_userid=zd909m2ebs;cid=b5c74e62f7c2895b6e31ba69d049fb101669001849; THSSESSID=2b5db7769f0df582059c5d2cbc;v="+ hexin_v,
            "hexin-v":hexin_v,
            }
    print headers
    return headers

def get_hexin(token_time):
    with io.open("hexin_v_get.js",encoding="utf-8")as f:
        js = f.read()
        #print token_time+"\n"+js
        JS = execjs.compile(token_time+"\n"+js) #读取时间拼接进入js代码中
        #print "hello "
        hexin_v = JS.call("rt.update")
        print hexin_v
        return hexin_v

def check_json(input_str):
    try:
        json.loads(input_str)
        return True
    except:
        return False

def parse_data(resp):
    #print type(dic)
    #resp=resp.decode('utf-8').encode('gbk')
    dic=json.loads(resp)

    if dic.has_key('status_code')  and dic.has_key('data'):
        data = dic['data']
        if dic.has_key('logid'):
            log_id = dic['logid']

        mdata = dic['data']
        #for k in mdata:
        #    print k, mdata[k]
        mkdata = mdata['answer']
        #print json.dumps(mkdata).decode("unicode-escape")
        for mk in mkdata:
            mm = mk['txt']
            for mmm in mm:
                mc = mmm['content']['components']
                #print json.dumps(mc).decode("unicode-escape")
                for mmc in mc:
                    #print mmc
                    mmd = mmc['data']['datas']
                    print json.dumps(mmd).decode("unicode-escape")
                    for md in mmd:
                        print json.dumps(md).decode("unicode-escape")

                #print mm, mk[mm]
    load_to_file()

##此脚本为根据同花顺问才抓取数据，运行需要安装node

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf8')
    #iprint urllib.quote("2022年11月13日人气排名".decode('gbk').encode('utf-8'))
    load_from_file()
    datas = get_datas(sys.argv[1])
    try:
        print url
        headers=  get_headers(hexin_v, log_id, len(datas))
        #resp = requests.post(url=url, data=datas,headers=headers).text.decode("unicode-escape")
        resp = requests.post(url=url, data=datas,headers=headers).text
        if "javascript" in resp:
            print resp.decode('utf-8', 'ignore')
            update_time()
            print(hexin_v, log_id)
            raise "出错"
        parse_data(resp)
    except:
        #headers=  get_headers(hexin_v, log_id, len(datas))
        #print headers
        #resp = requests.post(url=url, data=datas, headers=headers).text.encode('utf-8', 'ignore')
        #print "hello", type(resp), resp
        #log_id = resp["logid"]
        #print log_id
        headers=  get_headers(hexin_v, log_id, len(datas))
        resp = requests.post(url=url, data=datas, headers=headers).text
        #print resp.decode("unicode-escape")
        parse_data(resp)
