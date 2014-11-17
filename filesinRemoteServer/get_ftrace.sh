echo $PATH
PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools
adb shell sh /data/ftrace/ftrace_init.sh
adb shell sh /data/ftrace/ftrace_start.sh 

