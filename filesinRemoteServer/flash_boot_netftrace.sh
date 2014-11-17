PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools
adb reboot bootloader
fastboot flash boot /home/wjbang/20141104-ftrace-stable-boot.img
fastboot reboot
sleep 15
