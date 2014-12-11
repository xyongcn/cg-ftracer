cd `dirname $0`
export PATH=${PATH}:`cat adb_location.sh`
adb reboot
