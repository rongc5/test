#!/bin/bash


EMAIL_TO='mingz_zhang@easou.cn'
path='/data1/ecom/dsp/'

function check_redis() {
	local idea_id=$1;
	local day=$2;
	local title=$3;
	local name=${path}dsp_access.log_${day};
 	
	
	#展现
	echo $day, $idea_id, $name
	show=`cat ${name} | grep dsp_src=${idea_id} | grep 'ad_act=1' |awk -F'?' '{print $2}' | awk -F'&' '{print $1}' | uniq | wc -l`
	#cmd="grep 'dsp_src=${idea_id}' $name | grep 'ad_act=1' |awk -F'?' '{print $2}' | awk -F'&' '{print $1}' | uniq | wc -l"
    	echo $show

	if [ ! "$show" ];then
		show=0
	fi

    echo $idea_id, $day, $show
    #printf "%-40s\t%-40s\t%-40s\n%-33s\t%-40d\n" 日期 展现量 $day ${show} > ${path}dspad.txt
    printf "%-40s\t%-40s\n%-33s\t%-40d\n" 日期 展现量 $day ${show} > ${path}dspad.txt
    #printf  ${name}
    #mail -s ${name}   -r "<wapage@easou.cn>" mingz_zhang@easou.cn zhixin_chen@easou.cn < /data/mingz/dspad.txt
    mail -s ${title}   -r "<wapage@easou.cn>" mingz_zhang@easou.cn < ${path}dspad.txt
}





#id_arr=('182262973' '182262976' '182262979')
id_arr=('ad_luomi')
name_arr=('洛米科技日报表')
#name_arr=('中文在线日报表' "微卷日报表" "咪咕日报表")
yesterday=`date  +"%Y%m%d" -d  "-1 days"`
#yesterday='20180323'


source /etc/profile

i=0
while [ $i -lt ${#id_arr[@]} ]
do
  check_redis ${id_arr[$i]} $yesterday ${name_arr[$i]}
  let i+=1
done
