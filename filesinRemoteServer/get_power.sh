echo $PATH
PATH=$PATH:/home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools
#adb shell /data/powercat/powercat.sh sence_cpudelay 
case $1 in
	"msp" )
	test_name="sence_cpudelay" ;;
	* )
	test_name="" ;;
esac
adb shell /data/powercat/powercat.sh $test_name
sleep 999999
echo "powercat start!"
