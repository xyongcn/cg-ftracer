PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools

server_sh_location="/home/wjbang/ftrace/"
adb reboot bootloader
fastboot flash boot ${server_sh_location}/boot.img
fastboot reboot
sleep 15
