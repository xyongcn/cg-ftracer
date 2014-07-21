#!/bin/bash
function ergodic(){
for file in `ls $1`
do
local path="/home/wu/Desktop/parse"
local name=$file 
echo $name
#解压缩
$path/zpipe -d <$path/test/$name> $path/resource/$name.bin
#后期处理
echo "process"
$path/parse  $path/resource/$name.bin $path/result/$name.txt

#处理完可删除zpiperesult文件夹下文件
#rm $path/test/$name
mv $path/test/$name $path/test/done/
done
}


count=1
while (( $count<=5))
do
INIT_PATH="/home/wu/Desktop/parse/test"
ergodic $INIT_PATH
done
