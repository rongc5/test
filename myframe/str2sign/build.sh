#/bin/sh




rm -rf lib include
mkdir -p lib include


mk=core.mk

path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make -f $mk clean && make -j8 -f $mk
    cp core/lib*.a lib
    cp core/*.h include
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make -f $mk clean
        rm -rf lib include
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make -f $mk clean && make -j8 -f $mk
        cp core/lib*.a lib
        cp core/*.h include
        (cd test && make clean && make)
    fi
fi
