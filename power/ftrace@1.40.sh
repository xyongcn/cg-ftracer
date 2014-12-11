test_case=$1
test_name=$2
test_time=$3
test_serip=$4
echo $test_case and $test_name and $test_time and $test_serip 
port=1401
location="solaris.cs.tsinghua.edu.cn"
data_location_clint="home/wjbang/power/"
server_sh_location="/home/wjbang/ftrace/"
clint_sh_location="/home/wjbang/power/"
receiver_location=${server_sh_location}

cp vmlinux /dev/shm/
ssh root@${location} -p ${port} "${receiver_location}/powercat_server ${receiver_location}/power.dat" &
ssh root@${location} -p ${port} "${receiver_location}/server_tcp_onefile /dev/shm/ftrace.data" & 

ssh root@${location} -p ${port} "sh ${server_sh_location}/flash_boot_netftrace.sh"
sleep 30
ssh root@${location} -p ${port} "sh ${server_sh_location}/open_usbnet.sh"
sleep 3
ssh root@${location} -p ${port} "${server_sh_location}/get_power.sh ${test_name}" &
sleep 5
ssh root@${location} -p ${port} "sh ${server_sh_location}/get_ftrace.sh" &

sleep 10
#set the running time of ftrace.
sleep 40

echo "stop ftace.."
#ssh root@192.168.1.40 'sh /home/wjbang/ftrace/stop_ftrace.sh'
ssh root@${location} -p ${port} "sh ${server_sh_location}/stop_mm.sh"
sleep 50
ssh root@${location} -p ${port} "sh ${server_sh_location}/stop_ftrace.sh"
sleep 20
echo "reboot the phone."
ssh root@${location} -p ${port} "sh ${server_sh_location}/reboot_phone.sh"
ssh root@${location} -p ${port} "sh ${server_sh_location}/kill-server.sh"
sleep 5

mkdir -p $data_location_clint
scp -P ${port} root@${location}:/dev/shm/ftrace.data $data_location_clint
ssh root@${location} -p ${port} 'rm /dev/shm/ftrace.data'
scp -P ${port} root@${location}:${receiver_location}/power.dat $data_location_clint
ssh root@${location} -p ${port} "rm ${receiver_location}/power.dat"

#/home/wjbang/parse-for-all-funcgraph ftrace.data ftrace.parsed
/${clint_sh_location}/parse-for-all-funcgraph ${data_location_clint}/ftrace.data ${data_location_clint}/powercat-ftrace
ruby ${clint_sh_location}/db/EnterDynamic-Nexux5.rb ${data_location_clint} android-4.4.3 real arm-Nexus5-${test_case}

ruby ${clint_sh_location}/db/power_analyse.rb android-4.4.3 real arm-Nexus5-${test_case}
