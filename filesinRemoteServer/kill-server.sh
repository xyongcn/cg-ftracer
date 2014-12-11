server_sh_location="/home/wjbang/ftrace/"
back_up=`dirname $0`
pid=$(ps -ef | grep ${server_sh_location}/server_tcp_onefile | awk '{print $2}')
kill $pid
sleep 5
pid=$(ps -ef | grep ${server_sh_location} | awk '{print $2}')
kill $pid
pid=$(ps -ef | grep "sleep 9999" | awk '{print $2}')
kill $pid

pid=$(ps -ef | grep ${back_up}/server_tcp_onefile | awk '{print $2}')
kill $pid
sleep 5
pid=$(ps -ef | grep ${back_up} | awk '{print $2}')
kill $pid
pid=$(ps -ef | grep "sleep 9999" | awk '{print $2}')
kill $pid

