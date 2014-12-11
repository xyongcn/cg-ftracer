#/bin/sh
cd `dirname $0`
export PATH=${PATH}:`cat adb_location.sh`
echo ${PATH}
adb reboot bootloader
fastboot flash boot boot.img
fastboot reboot
sleep 15
