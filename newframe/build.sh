#/bin/sh




rm -rf lib
mkdir -p lib


cd idl
protoc  --cpp_out=. ./idl.proto
cd ..


path=`pwd`
base_file=`basename $path`

if [ $# == 0 ]; then
    make clean && make -j8
    mv core/lib*.a lib
elif [ $# == 1 ];then
    if [ $1 == "clean" ];then
        make clean
        (cd test && make clean)
    elif [ $1 == "all" ]; then
        make clean && make -j8
        mv core/lib*.a lib
        (cd test && make clean && make)
    elif [ $1 == "tar" ]; then
        tar -czvf $base_file.tar.gz $base_file core idl conf ${base_file}ctl.sh
    fi
fi
