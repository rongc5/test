#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys, random

#in_list = 'sid,esid,uid,pd,pt,sty,qry,qid0,qid1,url,pi,ref,cid,ip,iploc,moblie,mobloc,ngo,ngp,ngc,ipo,ipp,ipc,loc_src,phn,ua,mid,wvar,op,qry_src,muid,mmid,t,top,op_type,system_type,industry_id,customer_type,src,ad_type,tn,ads,pid,city,np,cbd,tm,at,rc,discount,indv_type,rep_query,appid,appt,gender,bhd,loc,filtered,req_sg,req_sg_cnt,req_sg_rs,sg_rtn_cnt,sg_all_cnt,lbsloc,sgad,req_gg,ds_cnt,kdad,ad,pclist'

#out_list = 'sid,esid,uid,pd,pt,st,qry,url,pi,ref,cid,ip,iploc,mob,mobloc,ua,mid,wver,t,lbsloc,qrySrc,appid,appt,gender,bhd,loc,ids,nett,muid,mmid,pclist,cnt,ct,cm,tgdsid,dl,cngo,loc_ds,ids,idfa,ngo,openud,udid,mac,sim,ngc,ngp,ngo,ver,fmt'
#out_in_dict = {'st':'sty', 'mob':'moblie', 'wver':'wvar', 'qrySrc':'qry_src', 'ids':'industry_id', 'nett':'np','cnt':'rq_cnt','cm':'rq_cm','tgdsid':'tgds','ids':'industry_id','ngo':'op'}

#0out_lists = out_list.split(',')

filter_string = 'pd,pt'
filter_list = filter_string.split(',')


input_string = 'sid,esid,uid,pd,pt,st,qry,url,pi,ref,cid,ip,iploc,mob,mobloc,ua,mid,wver,t,lbsloc,qrySrc,appid,appt,gender,bhd,loc,loc_ds' \
             'dl,cngo,ids,nett,fmt,gru,gqs,gup,gxuds,gxjd,gxd,gxus,gxju,gxeu,gv,ga,muid,mmid,pclist,ngo,np,ngp,ngc,cnt,ct,ver,ecpm,rideaid,' \
             'sim,mac,idfa,udid,openud,tgdsid,loadType,cm'

input_list = input_string.split(',')

out_in_dict = {'sty':'st','moblie':'mob','wvar':'wver','qry_src':'qrySrc','industry_id':'ids','np':'nett','rq_cnt':'cnt',
               'rq_cm':'cm','tgds':'tgdsid','op':'ngo'}

default_dirct = {'st':'0','cid':'es_10659_1429_2','qry':'搜索广告排序测试','pi':'0','ecpm':"4;5",'ver':"1.1.1",
                 'nett':'1','cnt':10,'ct':1,'fmt':'1','esid':'9wRV2Dj0MWyicmp','gender':'0','muid':'4194','mmid':'90',
                 'qrySrc':'0','appt':'0','ids':'0','iploc':'0','mobloc':'0','ip':"223.73.217.96",'pd':'29','pt':'2',
                 'sid':"b0b7287b-6237-4fc9-9ac7-64f732136c17"}


def handle_asp_log(line):
    out_string = ""
    items = line.split(" ")
    for item in items:
        item = item.strip()
        key = ""
        sub_items = item.split("=")
        if len(sub_items) < 2:
            continue
        #print(sub_items[0], sub_items[1]);

        if out_in_dict.has_key(sub_items[0]):
            key = out_in_dict.get(sub_items[0])
        else:
            for k in input_list:
                if sub_items[0] in k:
                    key = k;
                    break;

        if not key.strip(''):
            continue

        if not sub_items[1].strip(''):
            for k in filter_list:
                if key in k:
                    return

        out_string += ", \"%s\":\"%s\"" %(key, sub_items[1])

    out_string = out_string[2:]
    if out_string.strip(''):
        print "{%s}" %out_string



if '__main__' == __name__:
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        if line.startswith("NOTICE"):
            handle_asp_log(line)

