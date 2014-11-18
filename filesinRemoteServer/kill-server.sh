server_sh_location="/home/wjbang/ftrace/"
pid=$(ps -ef | grep ${server_sh_location}/server_tcp_onefile | awk '{print $2}')
kill $pid
sleep 10
pid=$(ps -ef | grep ${server_sh_location} | awk '{print $2}')
kill $pid
pid=$(ps -ef | grep "sleep 9999" | awk '{print $2}')
kill $pid
