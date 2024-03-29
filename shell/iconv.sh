#! /bin/bash
ICONV=iconv

if ! which $ICONV &> /dev/null
then
    ICONV=piconv
fi

for i in qqwry.dat 
do
    $ICONV -f "gbk" -t "utf-8" < "$i" > "$i.utf8"
    ret=$?
    if [ $ret -eq 0 ] ; then
        mv -f "$i" "$i.backup"
        mv -f "$i.utf8" "$i"
    else
        echo "fail to convert $i from gbk to utf-8"
    fi
done
exit $ret

#end 
