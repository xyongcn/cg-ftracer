#/bin/sh
cd `dirname $0`
export PATH=${PATH}:`cat adb_location.sh`
adb pull /sdcard/binder_ftrace.txt /home/wjbang/ftrace/binder_ftrace.txt
