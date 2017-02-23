#/bin/sh




rm -rf lib
mkdir -p lib


cd idl
protoc  --cpp_out=. ./myframe.proto
cd ..


if [ $# == 0 ]; then
    make clean && make
    mv core/lib*.a lib
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make clean
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make clean && make
        mv core/lib*.a lib
        (cd test && make clean && make)
    fi
fi
