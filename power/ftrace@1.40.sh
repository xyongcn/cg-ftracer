cd /home/wjbang/power/
test_case=$1
test_name=$2
test_time=$3
test_serip=$4
echo $test_case and $test_name and $test_time and $test_serip 
port=1401
location="solaris.cs.tsinghua.edu.cn"

cp vmlinux /dev/shm/
ssh root@${location} -p ${port} '/home/wjbang/ftrace/powercat_server /home/wjbang/ftrace/power.dat' &
ssh root@${location} -p ${port} '/home/wjbang/ftrace/server_tcp_onefile /dev/shm/ftrace.data' & 

ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/flash_boot_netftrace.sh'
sleep 30
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/open_usbnet.sh'
sleep 3
ssh root@${location} -p ${port} "/home/wjbang/ftrace/get_power.sh ${test_name}" &
sleep 5
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/get_ftrace.sh' &

sleep 10
#set the running time of ftrace.
sleep 40

echo "stop ftace.."
#ssh root@192.168.1.40 'sh /home/wjbang/ftrace/stop_ftrace.sh'
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/stop_mm.sh'
sleep 50
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/stop_ftrace.sh'
sleep 20
echo "reboot the phone."
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/reboot_phone.sh'
ssh root@${location} -p ${port} 'sh /home/wjbang/ftrace/kill-server.sh'
sleep 5

scp -P ${port} root@${location}:/dev/shm/ftrace.data /home/wjbang/power/
ssh root@${location} -p ${port} 'rm /dev/shm/ftrace.data'
scp -P ${port} root@${location}:/home/wjbang/ftrace/power.dat /home/wjbang/power/
ssh root@${location} -p ${port} 'rm /home/wjbang/ftrace/power.dat'

#/home/wjbang/parse-for-all-funcgraph ftrace.data ftrace.parsed
X=/mnt/freenas/servers-transfer/powercat-ftrace-test
./parse-for-all-funcgraph ftrace.data powercat-ftrace
ruby /home/wjbang/power/db/EnterDynamic-Nexux5.rb /home/wjbang/power android-4.4.3 real arm-Nexus5-${test_case}

ruby /home/wjbang/power/db/power_analyse.rb android-4.4.3 real arm-Nexus5-${test_case}
