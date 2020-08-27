#/bin/sh



mk=sk.mk

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



#
#wget https://www.openssl.org/source/openssl-1.1.0g.tar.gz
#tar -zxvf openssl-1.1.0g.tar.gz 
#cd openssl-1.1.0g/
#./config --prefix=/usr/local/openssl
#./config -t
#make
#make install
#cp /usr/local/openssl/lib/*.a /usr/lib64/



##dns
#yum -y install nscd
#systemctl status nscd
#systemctl start nscd
#strings /var/db/nscd/hosts

