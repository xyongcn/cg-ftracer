#adb shell /data/powercat/powercat.sh sence_cpudelay 
source adb_location.sh
case $1 in
	"msp" )
	test_name="sence_cpudelay" ;;
	* )
	test_name="" ;;
esac
adb shell /data/powercat/powercat.sh $test_name
sleep 999999
echo "powercat start!"
