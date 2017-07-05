#/bin/sh




rm -rf lib
mkdir -p lib


mk=core.mk

path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make -f $mk clean && make -j8 -f $mk
    mv core/lib*.a lib
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make -f $mk clean
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make -f $mk clean && make -j8 -f $mk
        mv core/lib*.a lib
        (cd test && make clean && make)
    fi
fi
