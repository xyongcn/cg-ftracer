cd `dirname $0`
export PATH=${PATH}:`cat adb_location.sh`
adb shell sh /data/ftrace/stop_ftrace.sh
