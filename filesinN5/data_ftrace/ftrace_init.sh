#!/system/bin/sh
echo 12582912 > /proc/sys/net/core/wmem_max
echo 12582912 > /proc/sys/net/core/rmem_max
echo 12582912 12582912 22582912 > /proc/sys/net/ipv4/tcp_rmem
echo 12582912 12582912 22582912 > /proc/sys/net/ipv4/tcp_wmem
echo "TCP init secceed..."

#echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
#echo 300000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed

echo "0" > /sys/devices/system/cpu/cpu1/online
chmod 444 /sys/devices/system/cpu/cpu1/online
echo "0" > /sys/devices/system/cpu/cpu2/online
chmod 444 /sys/devices/system/cpu/cpu2/online 
echo "0" > /sys/devices/system/cpu/cpu3/online
chmod 444 /sys/devices/system/cpu/cpu3/online 
echo "CPU init secceed..."
