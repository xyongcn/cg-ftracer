pid=$(ps -ef | grep /home/wjbang/ftrace/server_tcp_onefile | awk '{print $2}')
kill $pid
sleep 10
pid=$(ps -ef | grep /home/wjbang/ftrace | awk '{print $2}')
kill $pid
pid=$(ps -ef | grep "sleep 9999" | awk '{print $2}')
kill $pid
