PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools
adb reboot bootloader
fastboot flash boot /home/lxu/boot.img
fastboot reboot

sleep 20 

adb shell sh /data/binder/tracefunctiongraph.sh & 
adb shell 'am start -n com.android.music/com.android.music.MusicBrowserActivity'
adb shell 'am start -n com.android.music/com.android.music.VideoBrowserActivity'
adb shell 'am start -n com.android.calendar/com.android.calendar.LaunchActivity'
adb shell 'am start -n com.android.calculator2/com.android.calculator2.Calculator'
adb shell 'am start -n com.android.music/com.android.music.MediaPlaybackActivity'


