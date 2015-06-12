## reverse_number.sh          ##
#! /bin/sh
n=$#       //“$#”是取在终端中输入的参数的个数。如你在终端中输入sh reverse_number.sh a b 3 1 d e 7
                  则$#的值就是7，.sh文件后面紧跟的，以空格分隔的所有参数的个数
i=1
j=$n
declare -a array         //定义一个数组array
until [ $j -eq 0 ]
do
        while [   $i -le $n ]
        do 
       array[$i]=`echo $1`    //将位置1上的参数存入数组中，即例子中的“a”
     shift 1                       //所有的参数向左移动一位，最左侧的数据丢失，目前的数据变成
                                                "b 3 1 d e 7",a 被丢掉了，目前位置1上的参数变成“b”
     i=`expr $i + 1`         //即 i=i+1的shell 版～
        done
        echo ${array[$j]}         
        j=`expr $j － 1`
done 
