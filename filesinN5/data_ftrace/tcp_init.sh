#!/system/bin/sh
echo 12582912 > /proc/sys/net/core/wmem_max
echo 12582912 > /proc/sys/net/core/rmem_max
echo 2582912 12582912 22582912 > /proc/sys/net/ipv4/tcp_rmem
echo 2582912 12582912 22582912 > /proc/sys/net/ipv4/tcp_wmem
echo "TCP init secceed..."
