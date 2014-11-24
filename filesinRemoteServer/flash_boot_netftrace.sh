source adb_location.sh
server_sh_location="/home/wjbang/ftrace/"
adb reboot bootloader
fastboot flash boot ${server_sh_location}/boot.img
fastboot reboot
sleep 15
