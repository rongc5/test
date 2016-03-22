#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys,re

#in_list = 'sid,esid,uid,pd,pt,sty,qry,qid0,qid1,url,pi,ref,cid,ip,iploc,moblie,mobloc,ngo,ngp,ngc,ipo,ipp,ipc,loc_src,phn,ua,mid,wvar,op,qry_src,muid,mmid,t,top,op_type,system_type,industry_id,customer_type,src,ad_type,tn,ads,pid,city,np,cbd,tm,at,rc,discount,indv_type,rep_query,appid,appt,gender,bhd,loc,filtered,req_sg,req_sg_cnt,req_sg_rs,sg_rtn_cnt,sg_all_cnt,lbsloc,sgad,req_gg,ds_cnt,kdad,ad,pclist'

#out_list = 'sid,esid,uid,pd,pt,st,qry,url,pi,ref,cid,ip,iploc,mob,mobloc,ua,mid,wver,t,lbsloc,qrySrc,appid,appt,gender,bhd,loc,ids,nett,muid,mmid,pclist,cnt,ct,cm,tgdsid,dl,cngo,loc_ds,ids,idfa,ngo,openud,udid,mac,sim,ngc,ngp,ngo,ver,fmt'
#out_in_dict = {'st':'sty', 'mob':'moblie', 'wver':'wvar', 'qrySrc':'qry_src', 'ids':'industry_id', 'nett':'np','cnt':'rq_cnt','cm':'rq_cm','tgdsid':'tgds','ids':'industry_id','ngo':'op'}

#0out_lists = out_list.split(',')

input_string = 'sid,cid,ip'

input_list = input_string.split(',')


def get_hour(line):
    items = line.split(" ")
    p = r'(\d{2}:\d{2}:\d{2})'
    for subitems in items:
        tmp = re.findall(p, subitems)
        if len(tmp) == 1:
            print tmp
            break

    time_items = tmp[0].split(":")
    if len(time_items) < 3:
        return
    print time_items[1]


def handle_asp_log(line):
    out_string = ""
    items = line.split(" ")
    flag = 0
    records = {}
    get_hour(line)
    for item in items:
        item = item.strip()
        key = ""
        sub_items = item.split("=")
        if len(sub_items) < 2:
            continue

        #print(sub_items[0], sub_items[1]);

        if sub_items[0] == 'cmatch_ad_src_type' and '230,11' not in sub_items[1]:
            return
        
        flag = 0
        for k in input_list:
            if sub_items[0] == k:
                key = k;
                flag = 1
                break;


        if not key.strip(''):
            continue
        if flag == 1:
            records[key] = sub_items[1]

    if len(records) != len(input_list):
        return
    out_string += "%s\t%s#%s" %(records['sid'], records['cid'], records['ip'])

    if out_string.strip(''):
        print "%s" %out_string

if '__main__' == __name__:
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        if line.startswith("NOTICE"):
                handle_asp_log(line)

