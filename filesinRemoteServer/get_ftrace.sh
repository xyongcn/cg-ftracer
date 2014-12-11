cd `dirname $0`
export PATH=${PATH}:`cat adb_location.sh`
adb shell sh /data/ftrace/ftrace_init.sh
adb shell sh /data/ftrace/ftrace_start.sh 

