#/bin/sh



mk=qf.mk

path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make -f $mk clean && make -j8 -f $mk
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make -f $mk clean
    elif [ $1 == "all" ]; then
        make -f $mk clean && make -j8 -f $mk
    elif [ $1 == "tar" ]; then
        tar -czvf $base_file.tar.gz $base_file core idl conf ${base_file}ctl.sh
    fi
fi
