#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys, random

in_list = 'sid,esid,uid,pd,pt,sty,qry,qid0,qid1,url,pi,ref,cid,ip,iploc,moblie,mobloc,ngo,ngp,ngc,ipo,ipp,ipc,loc_src,phn,ua,mid,wvar,op,qry_src,muid,mmid,t,top,op_type,system_type,industry_id,customer_type,src,ad_type,tn,ads,pid,city,np,cbd,tm,at,rc,discount,indv_type,rep_query,appid,appt,gender,bhd,loc,filtered,req_sg,req_sg_cnt,req_sg_rs,sg_rtn_cnt,sg_all_cnt,lbsloc,sgad,req_gg,ds_cnt,kdad,ad,pclist'

out_list = 'sid,esid,uid,pd,pt,st,qry,url,pi,ref,cid,ip,iploc,mob,mobloc,ua,mid,wver,t,lbsloc,qrySrc,appid,appt,gender,bhd,loc,ids,nett,muid,mmid,pclist,cnt,ct,cm,tgdsid,dl,cngo,loc_ds,ids,idfa,ngo,openud,udid,mac,sim,ngc,ngp,ngo,ver,fmt'
out_in_dict = {'st':'sty', 'mob':'moblie', 'wver':'wvar', 'qrySrc':'qry_src', 'ids':'industry_id', 'nett':'np','cnt':'rq_cnt','cm':'rq_cm','tgdsid':'tgds','ids':'industry_id','ngo':'op'} 

out_lists = out_list.split(',')

filter_list = 'pd,pt'
filter_dict = filter_list.split(',')

#num_lsit = 'st,pi,iploc,mobloc,t,lbsloc,qrySrc,appt,gender,dl,cngo,ids,nett,muid,mmid,ngo,np,ngp,ngc,cnt,ct,loadType,cm'
#num_dict = num_lsit.split(',')

default_dirct = {'st':'0','cid':'es_10659_1429_2','qry':'搜索广告排序测试','pi':'0','ecpm':"4;5",'ver':"1.1.1",
                 'nett':'1','cnt':10,'ct':1,'fmt':'1','esid':'9wRV2Dj0MWyicmp','gender':'0','muid':'4194','mmid':'90',
                 'qrySrc':'0','appt':'0','ids':'0','iploc':'0','mobloc':'0','ip':"223.73.217.96",'pd':'29','pt':'2',
                 'sid':"b0b7287b-6237-4fc9-9ac7-64f732136c17"}

def handle_asp_log(line):
    log_dict = {}
    items = line.split(" ")
    for item in items:
        item = item.strip()
        sub_items = item.split("=")
        if len(sub_items) < 2:
            continue
        #print(sub_items[0], sub_items[1]);
        log_dict[sub_items[0]] = sub_items[1]
    if len(log_dict) == 0:
        return
    out_string = ""
    value =""

    for item in out_lists:
        if item in log_dict:
            value = log_dict[item]
        elif item in out_in_dict:
            if out_in_dict[item] in log_dict:
                value = log_dict[out_in_dict[item]]

        if  (item in filter_dict and not value.strip('')):
            return

        if (not value.strip('') and item in default_dirct):
            value = default_dirct[item];
            #print(item, value);
            #continue;
            #value="0";
        out_string += ", \"%s\":\"%s\"" %(item, value)
        value = ""
    #out_string += ", \"%s\":\"%s\"" %("fmt", "1")
    out_string = out_string[2:]
    print "{%s}" %out_string
    




if '__main__' == __name__:
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        if line.startswith("NOTICE"):
            handle_asp_log(line)

