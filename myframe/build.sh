#/bin/sh


arr=('str2sign')


rm -rf lib include
mkdir -p lib include


mk=core.mk

path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    i=0
    while [ $i -lt ${#arr[@]} ]
    do
        cd  ${arr[$i]} && ./build.sh all
        cp lib/lib*.a ../lib
        cp core/*.h ../include
        let i+=1
        cd ..
    done
    cp core/*.h include
    make -f $mk clean && make -j8 -f $mk
    rm -rf lib/lib*.a
    cp core/lib*.a lib
    (cd test && make clean && make)
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make -f $mk clean
        rm -rf lib include
        (cd test && make clean)
        i=0
        while [ $i -lt ${#arr[@]} ]
        do
            cd  ${arr[$i]} && ./build.sh clean
            let i+=1
            cd ..
        done
    fi
fi
