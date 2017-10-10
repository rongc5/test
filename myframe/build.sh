#/bin/sh


arr=('str2sign')


rm -rf lib include
mkdir -p lib include


mk=core.mk

path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make -f $mk clean && make -j8 -f $mk
    mv core/lib*.a lib
    cp core/*.h include
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
    elif [ $1 == "all" ]; then
        make -f $mk clean && make -j8 -f $mk
        mv core/lib*.a lib
        cp core/*.h include
        (cd test && make clean && make)
        i=0
        while [ $i -lt ${#arr[@]} ]
        do
            cd  ${arr[$i]} && ./build.sh all
            let i+=1
            cd ..
        done
    fi
fi
