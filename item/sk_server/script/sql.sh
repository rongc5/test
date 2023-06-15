#!/bin/bash

HOST="127.0.0.1"
PORT=3306
USERNAME="mingz"
PASSWORD="mingz@025251"
DBNAME="sk_info"
SQL="select * from max_main_funds where sk_id not like 'sh688%' and sk_id not like 'sz300%' and sum_change_rate < 100 and end_hlow < 1.1 order by force_diff_cir_value desc, end_hlow asc, force_diff_total_price desc, cir_value asc;"

SQL="SELECT  trade_date from  single_t GROUP BY trade_date ORDER BY trade_date DESC LIMIT 1;"

mysql -h ${HOST} -u ${USERNAME} -p${PASSWORD} -D ${DBNAME} -e "${SQL}" > ._last_date

day=`tail -1 ./._last_date`
echo ${day}
#day=`date -d '0 day' +%Y-%m-%d`
#day=2021-04-16


SQL="select * from max_main_funds where sk_id not like 'sh688%' and sk_id not like 'sz300%' and sum_change_rate < 100 and end_hlow < 1.08 and DATE_FORMAT(update_time,'%Y-%m-%d') = '${day}' and force_diff_cir_value >=  60 order by force_diff_cir_value desc, end_hlow asc, force_diff_total_price desc, cir_value asc;"

SQL="select * from max_main_funds where sk_id not like 'sh688%' and sum_change_rate < 100 and end_hlow < 1.08 and update_date = '${day}' and sum_force_diff_cir_value >=  60 order by sum_force_diff_cir_value desc, end_hlow asc, sum_force_diff_total_price desc, cir_value asc;"

SQL="select * from max_main_funds where sk_id not like 'sh688%' and sk_id not like 'sz3%' and scrate < 100 and end_hlow <= 1.08 and update_date = '${day}' and sfd_cvalue >=  50 order by sfd_cvalue desc, end_hlow asc, sfd_tprice desc, cvalue asc; "

#SQL="SELECT * from finance_t;"

mysql -h ${HOST} -u ${USERNAME} -p${PASSWORD} -D ${DBNAME} -e "${SQL}"


###常用

call mmfunds_force_sratio_dpointer_ehlow_fdcvalue(300, 0.45, 0, 1.11, 50)
call mmfunds_force_dpointer_ehlow_fdcvalue(300, 0, 1.1, 30)
Call mmfunds_sratio_dpointer_ehlow_fdcvalue(0.45, 0, 1.2, 50)
call mmfunds_dpointer_ehlow_fdcvalue_fdtprice(0, 1.2, 50, 0.6)

call force_sratio_dpointer(300, 0.45, 0)
call force_sratio_dpointer_holder(300, 0.45, 0, '%社保%')

call rps_mmfunds_sratio_dpointer(60, 900, 0.45, 0)
call rps_mmfunds_force_sratio_dpointer(60, 900, 300, 0.45, 0)
call rps_force_sratio_dpointer(60, 800, 300, 0.45, 0)
call rps_force_sratio_dpointer_holder(250, 800, 300, 0.45, 0, '%社保%')
call rps_mmfunds_force_sratio_dpointer_holder(250, 800, 300, 0.45, 0, '%社保%')
call mmfunds_force_dpointer_ehlow_fdcvalue_holder(300, 0, 1.1, 30, '%社保%')
call mmfunds_force_sratio_dpointer_ehlow_fdcvalue_holder(300, 0.45, 0, 1.3, 50, '%社保%')
call rps_mmfunds_sratio_dpointer_holder(250, 800, 0.45, 0, '%社保%')
call rps_mmfunds_dpointer_holder(60, 800, 0.45, 0, '%社保%')
call hrpercent_days_nums_plate(0, 1, 5)
call mmfunds_dpointer_ehlow_fdcvalue_holder(0, 1.11, 50, '%社保%')
call rps_mmfunds_dpointer_ehlow_fdcvalue_holder(250, 650, 0, 1.3, 30, '%社保%')
call mmfunds_sratio_dpointer_ehlow_fdcvalue_holder(0.45, 0, 1.3, 50, '%社保%')
call rps_sratio_dpointer_holder(120, 650, 0.45, 0, '%社保%')
call mmfunds_dpointer_ehlow_fdcvalue_start_ftpgratio(0, 1.2, 50, 1, 0.2)
call mmfunds_dpointer_ehlow_fdcvalue_start_ftpgratio_sfdgratio(0, 1.2, 50, 1, 0.01, 0.1)
call rps_mmfunds_dpointer_ehlow_fdcvalue_start_ftpgratio(60, 1000, 0, 1.2, 50, 1, 0.2)
call rps_mmfunds_dpointer_ehlow_fdcvalue_start_ftpgratio_sfdgratio(60, 1000, 0, 1.2, 50, 1, 0.3, 0.1)
call mfquantify_days_sratio_gepercent_lepercent_dpointer(2, 300, 0.3, -2, 2, 0.6)




select sk_id id, sk_name 名字, sfdiff 主力资金净量, mstart_date 主力建仓开始时间, mmin_date 持仓期间最低价日期, scrate 持仓到当前累积换手率, end 当前价格, hlow 主力建仓期间最低价,end_hlow 当前价格与建仓期间最低价比值, cvalue 流通市值, sfd_cvalue 主力资金除以流通市值   from max_main_funds where sk_id ='sh600837' order by update_date;


select sk_name 名字, sfdiff 主力资金净量, mstart_date 主力建仓开始时间, mmin_date 持仓期间最低价日期, scrate 到当前累积换手率, end 当前价格, hlow 主力建仓期间最低价,end_hlow 当前价格与建仓期间最低价比值, sfd_cvalue 资金除以流通市值, sfd_tprice 资金除以当前成交量, update_date 更新日期  from max_main_funds where sk_id ='sz000063' order by update_date;

#

select * from concept_plate_t where update_date = current_date() and sk_id = 'sz301005';
select * from industry_plate_t where update_date = current_date() and sk_id = 'sz301005'; 

select bk_id, bk_name, range_percent from concept_plate_t where update_date = current_date() and range_percent > 0 group by bk_id, bk_name, range_percent order by range_percent desc;

select * from concept_plate_t where update_date = current_date() and bk_id = '021455'; 

select bk_id, bk_name, range_percent from industry_plate_t where update_date = current_date() and range_percent > 0 group by bk_id, bk_name, range_percent order by     range_percent desc;

call mmfunds_single_concept_bkid('021415')
call mmfunds_single_industry_bkid('012096')

select * from max_single where sk_id = 'sz002594' order by update_date desc limit 10;
select * from max_main_funds where sk_id = 'sh601038' order by update_date asc;

