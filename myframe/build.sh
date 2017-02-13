#/bin/sh

protoc  --cpp_out=./idl/ ./idl/myframe.proto

mkdir -p include lib
cp src/*.h include


if [ $# == 0 ]; then
    make clean && make
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make clean
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make clean && make
        (cd test && make clean && make)
    fi
fi
