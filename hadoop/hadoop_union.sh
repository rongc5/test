HOME_DIR=/app/ecom/hadoop/

DATE=$1
#DATE=20150722


#IN_DIR_XIAOSHUO=/logs/tupian/${DATE}/093/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_93=/logs/xiaoshuo/${DATE}/093/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_92=/logs/xiaoshuo/${DATE}/092/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_91=/logs/xiaoshuo/${DATE}/091/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_90=/logs/xiaoshuo/${DATE}/090/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_89=/logs/xiaoshuo/${DATE}/089/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_88=/logs/xiaoshuo/${DATE}/088/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_87=/logs/xiaoshuo/${DATE}/087/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_86=/logs/xiaoshuo/${DATE}/086/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_85=/logs/xiaoshuo/${DATE}/085/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_84=/logs/xiaoshuo/${DATE}/084/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_83=/logs/xiaoshuo/${DATE}/083/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_82=/logs/xiaoshuo/${DATE}/082/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_81=/logs/xiaoshuo/${DATE}/081/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_80=/logs/xiaoshuo/${DATE}/080/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_79=/logs/xiaoshuo/${DATE}/079/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_78=/logs/xiaoshuo/${DATE}/078/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_77=/logs/xiaoshuo/${DATE}/077/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_76=/logs/xiaoshuo/${DATE}/076/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_75=/logs/xiaoshuo/${DATE}/075/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_74=/logs/xiaoshuo/${DATE}/074/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN2=/logs/tupian/${DATE}/097/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN3=/logs/tupian/${DATE}/098/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN4=/logs/tupian/${DATE}/099/asp.log.nt.${DATE}.gz


OUT_DIR2=/user/ecom/mingz/${DATE}

$HOME_DIR/bin/hadoop fs -rmr $OUT_DIR2
#$HOME_DIR/bin/hadoop fs -mkdir $OUT_DIR2

#-reducer "python hal_new.py -r df/df.dict"    \
echo $DATE >> test

export PYPY=/app/ecom/pypy-1.9/bin/
$HOME_DIR/bin/hadoop jar $HOME_DIR/contrib/streaming/hadoop-streaming-1.0.3.jar \
 	-mapper  "python map_union_show.py"    \
	-reducer "python reduce_union_pvr.py"  \
	-file ./map_union_show.py \
	-file ./reduce_union_pvr.py \
  	-input /logs_lzo/xiaoshuo/$DATE/*/* \
  	-output $OUT_DIR2             \
  	-jobconf mapred.job.name='map_union_show'${DATE}    \
  	-jobconf mapred.map.tasks=500          \
  	-jobconf mapred.job.map.capacity=200     \
  	-jobconf mapred.reduce.tasks=1       \
  	-jobconf mapred.job.reduce.capacity=1 \
	-jobconf  stream.non.zero.exit.is.failure=false

#$HOME_DIR/bin/hadoop fs -getmerge $OUT_DIR2 stress_test
