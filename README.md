cg-ftracer
==========

Android kernel dynamic callgraph tracer for ftrace

replace trace.c in kernel-rootdir/kernel/trace/ and build.

1. N5手机的连接方式
    * adb shell
2. 利用adb和fastboot替换N5手机的内核
    1. 编译内核生成zImage-dtb
    2. 利用仓库中提供的ramdisk和镜像制作程序制作boot.img
    3. ./mkbootimg --base 0 --pagesize 2048 --kernel_offset 0x00008000 --ramdisk_offset 0x02900000 --second_offset 0x00f00000 --tags_offset 0x02700000 --cmdline 'console=ttyHSL0,115200,n8 androidboot.hardware=hammerhead user_debug=31 maxcpus=2 msm_watchdog_v2.enable=1' --kernel {zImage-dtb} --ramdisk {ramdisk.cpio.gz} -o {boot.img}
    4. 替换N5手机内核:
      * adb reboot bootloader
      * fastboot flash boot boot.img
    5. 重启手机进入系统：fastboot reboot

3. 网络输出ftrace流程
    1. adb shell连接上手机，进入/data/ftrace/目录，运行以下脚本启动usb网络
      * sh usb_init.sh
    2. adb shell会自动退出，此时运行sudo ifconfig usb0 192.168.42.60配置服务器接口ip地址
    3. 运行服务器接收程序
      * 创建/dev/shm/test目录（接收程序写死了）
      * ./server_tcp
      * （数据会输出到/dev/shm/test/）
    4. 重新连接adb shell进入手机
    5. 按顺序运行以下脚本启动ftrace
      * sh ftrace_init.sh
      * sh ftrace_start.sh

4. 解析中间数据
    * 将数据整合到一个文件中
    * ./parse-for-all-graph {input file} {output file}
    
5. 连接出问题的调试方法：
    1. /home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools/adb shell
    2. 如果正常返回，就算是连到手机上了，可以查看手机上的文件了。
    3. 如果行为异常，可以用下面命令对手机进行复位。
      * /home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools/adb reboot
      * /home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools/adb shell
      * exit
      * /home/wjbang/adt-bundle-linux-x86-20131030/sdk/platform-tools/adb shell
    4. adb shell 连上后
      1. 进入/data/ftrace/
      2. 运行usb开头的脚本：sh usb....
      3. adb shell会自动退出
        * root@hammerhead:/ # cd /data/ftrace/
        * root@hammerhead:/data/ftrace # sh usbnet_init.sh
    5. 在外面机器运行sudo ifconfig usb0 192.168.42.60
    6. 然后ping 192.168.42.129
      1. sudo ifconfig usb0 192.168.42.60
      2. ping 192.168.42.129



