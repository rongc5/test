HOME_DIR=/app/ecom/hadoop/

DATE=20150615


#IN_DIR_XIAOSHUO=/logs/tupian/${DATE}/093/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_CHU=/logs/xiaoshuo/${DATE}/093/asp.log.nt.${DATE}.gz
IN_DIR_XIAOSHUO_CAI=/logs/xiaoshuo/${DATE}/080/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN2=/logs/tupian/${DATE}/097/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN3=/logs/tupian/${DATE}/098/asp.log.nt.${DATE}.gz
#IN_DIR_TUPIAN4=/logs/tupian/${DATE}/099/asp.log.nt.${DATE}.gz

OUT_DIR2=/user/ecom/mingz/${DATE}

$HOME_DIR/bin/hadoop fs -rmr $OUT_DIR2
#$HOME_DIR/bin/hadoop fs -mkdir $OUT_DIR2

#-reducer "python hal_new.py -r df/df.dict"    \

export PYPY=/app/ecom/pypy-1.9/bin/
$HOME_DIR/bin/hadoop jar $HOME_DIR/contrib/streaming/hadoop-streaming-1.0.3.jar \
 	-mapper  "awk -f awk_baidu_rate.sh"    \
	-file ./awk_baidu_rate.sh	     \
  	-input $IN_DIR_XIAOSHUO_CAI         \
  	-output $OUT_DIR2             \
  	-jobconf mapred.job.name='awk_baidu_rate'${DATE}    \
  	-jobconf mapred.map.tasks=1          \
  	-jobconf mapred.job.map.capacity=1     \
  	-jobconf mapred.reduce.tasks=0       \
  	-jobconf mapred.job.reduce.capacity=0 \
	-jobconf  stream.non.zero.exit.is.failure=false

#$HOME_DIR/bin/hadoop fs -getmerge $OUT_DIR2 stress_test
