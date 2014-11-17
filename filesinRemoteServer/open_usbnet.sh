PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools
adb shell sh /data/ftrace/usbnet_init.sh
sleep 5
ifconfig usb0 192.168.42.60
ping -c 5 192.168.42.129
