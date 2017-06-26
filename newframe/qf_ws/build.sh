#/bin/sh






cd idl
protoc  --cpp_out=. ./ws_ser.proto
cd ..



path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make clean && make -j8
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make clean
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make clean && make -j8
        (cd test && make clean && make)
    elif [ $1 == "tar" ]; then
        tar -czvf $base_file.tar.gz $base_file core idl conf ${base_file}ctl.sh
    fi
fi
