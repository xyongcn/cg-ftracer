#!/bin/bash
function ergodic(){
for file in `ls $1`
do
local path=$1
local name=$file 
local para
if [[ $name == webapps\\myapp* ]];then
 cat $1/$name
 para=$(cat $1/$name)
 test_case=$(echo $name|awk -F"\\" '{print $3}'|awk -F"." '{print $1}')
 test_name=$(echo $para|awk -F"\"" '{print $4}')
 test_time=$(echo $para|awk -F"\"" '{print $8}')
 test_serip=$(echo $para|awk -F"\"" '{print $12}')
 echo $test_case and $test_name and $test_time and $test_serip
 mv $path/$name /home/wjbang/
 sh /home/wjbang/ftrace@1.40.sh $test_case $test_name $test_time $test_serip
fi
done
}

count=1
while (( $count<=5 ))
do
INIT_PATH="/usr/local/apache-tomcat-7.0.56"
ergodic $INIT_PATH
done
