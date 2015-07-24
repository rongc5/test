HOME_DIR=/app/ecom/hadoop/

DATE=20150604


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
 	-mapper  "python log_parser.py -c log_paser.conf"    \
	-reducer "awk -f awk_cpm.sh"  \
  	-input $IN_DIR_XIAOSHUO_CAI         \
	-file ./script/log_parser.py \
	-file ./script/log_paser.conf \
	-file ./script/awk_cpm.sh \
  	-output $OUT_DIR2             \
  	-jobconf mapred.job.name='hadoop_test_'${DATE}    \
  	-jobconf mapred.map.tasks=500          \
  	-jobconf mapred.job.map.capacity=100     \
  	-jobconf mapred.reduce.tasks=100       \
	-jobconf mapred.job.reduce.capacity=50 \

#$HOME_DIR/bin/hadoop fs -getmerge $OUT_DIR2 stress_test
