cg-ftracer
==========
author：wujingbang@gmail.com

QQ: 19882934

Any questions please ask freely.

注: android版本为4.4.3，代码编号为KTU84m（google官方代码）， 内核为msm，分支为android-msm-hammerhead-3.4-kitkat-mr2。
https://android.googlesource.com/kernel/msm/+/android-msm-hammerhead-3.4-kitkat-mr2
# 服务器、客户端的概念：
  1. 关系：实际上都是服务器，一台（机器A）连接了手机负责数据的采集，另一台（机器B）集成了数据库、画图工具和web，简单的流程是用户通过web访问机器B上的测试web，选定对应的测试用例点击保存，B会通过ssh调用A上的脚本（启动A上的数据接收程序与手机上的相关测试程序），测试完成（超时结束）后数据传送至B，进行后续的解析、入库工作。
  2. 在以下的文档中，“服务器"指的是机器A（连接了手机），“客户端"指的是机器B

# 仓库文件说明：
 * android_boot_tools_bin：包镜像文件的制作、拆包、解包等工具
 * filesinN5：
    1. apk文件用于自动打开N5手机的usb网络（前提：手机与电脑通过usb链接）
    2. data_ftrace：将其中文件存入手机目录/data/ftrace/，是手机中ftrace相关脚本，由服务器脚本调用
    3. data_powercat：将其中文件存入手机目录/data/powercat/，是手机中能耗监测与传输相关脚本，有服务器调用
 * filesinRemoteServer：包括服务器端（链接了手机）操作手机的各种原子操作
    1. adb_location.sh中包含adb、fastboot工具所在目录（仓库中也有，为adb.tar.gz）
    1. flash_boot_netftrace.sh与kill_server.sh中有server_sh_location变量，要保持和下面的一致
    
 * power：包括客户端（web端）中与能耗相关的脚本
    1. db：
       1. EnterDynamic-Nexux5.rb：动态数据入库的ruby脚本，由脚本自动调用，调用方式为：ruby db/EnterDynamic-Nexux5.rb <动态数据保存的目录> android-4.4.3 real arm-Nexus5-${test_case}
       2. power_analyse.rb：动态数据入库后调用该脚本进行能耗数据的解析，由脚本自动调用，调用方式为：ruby power_analyse.rb android-4.4.3 real arm-Nexus5-${test_case}，这些参数需要和之前Enter...脚本的参数保持一致。
    2. auto.sh：循环扫描指定目录下由web界面生成的配置文件，并根据配置文件调用ftrace@1.40.sh进行测试与数据获取、解析、入库的操作。
    3. ftrace@1.40.sh：实际进测试、数据获取、解析、入库的主要脚本，调用方式为：sh ftrace@1.40.sh <test_case> <test_name> <test_time,可空> <test_serip,可空>，举例：sh ftrace@1.40.sh test msp
       * 修改脚本中的相关环境变量：
            1. location和port为服务器的ssh地址与端口
            2. receiver_location为服务器中数据接收程序位置
            3. data_location_clint为客户端中间数据的存放位置
            4. server_sh_location为服务器脚本存放位置
            5. clint_sh_location为客户端脚本存放位置
       * 注：在该脚本中指定了运行的时间（测试用）
       
    4. parse-for-all-funcgraph：ftrace中间数据解析程序，需要提前将编译的内核镜像对应的vmlinux拷贝至/dev/shm/目录下。调用方式为parse-for-all-funcgraph <input> <output>
    5. power.so：power_analyse.rb依赖的库文件
  * 20141104-ftrace-stable-boot.img和20141104-vmlinux.tar.gz为编译好的内核镜像与对应的vmlinux
  * adb.tar.gz为adb、fastboot工具包

# 环境部署（默认操作系统为ubuntu 12.04）
  1. 手机刷ROM（ROM包还未上传，需要后续补充），从头编译的话请参照”关于nexus5手机内核的修改“一节。安装busybox与com.example.androidtestusbtethering-1.apk，自行编译并签名需要参照”关于系统级APK的签名“一节
  2. 手机连上服务器，在服务器与客户端配置ssh的无密码连接，参考其他->第四条
  3. 在客户端上配置tomcat与测试用例网页（补充）；配置mysql与ruby的运行环境
     * 在数据入库的ruby脚本中制定了使用的数据库名，可以在里面自行修改。
  4. 在服务器端安装：sudo apt-get install ia32-libs，（64位机器需要安装），将编译好的内核镜像文件放置于server_sh_location，镜像文件的编译参考其他
  5. 关于用户权限：连接手机的PC上使用的是root用户，配置web、数据库的PC上使用的是普通用户。
  6. 将相关文件拷贝至指定的位置，参考仓库文件说明
     * 将编译的内核镜像对应的vmlinux拷贝到客户端的clint_sh_location
     * 将内核镜像重命名为boot.img并拷贝到服务器的server_sh_location
     * 将adb、fastboot工具（解压缩adb.tar.gz）的目录加入adb_location.sh文件中。
  7. 修改ftrace@1.40.sh脚本中的相关环境变量，见仓库文件说明一节power/ftrace@1.40.sh的说明。
  8. 注：在open_usbnet.sh脚本中的IP地址最好不要修改，这不会影响其他网络的连接。
# 脚本调用方式
  1. 在客户端上，如果配置了测试端网页则运行auto.sh：
      * ./auto.sh
  2. 如果没有配置网页则直接运行数据获取脚本：
      * sh ftrace@1.40.sh test msp
      
# 运行结果
  1. 在数据库中生成两个表：android-4.4.3_R_arm-Nexus5-${test_case}_DLIST与android-4.4.3_R_arm-Nexus5-${test_case}_DOLIST，后续画SVG图会可以根据这两个表来完成。

# 关于Android的编译：
  1. 参考网页：http://source.android.com/source/index.html
  2. 我们现在用的版本：
     * android版本为4.4.3，代码编号为KTU84m（google官方代码）， 
     * 内核为msm，分支为android-msm-hammerhead-3.4-kitkat-mr2。 https://android.googlesource.com/kernel/msm/+/android-msm-hammerhead-3.4-kitkat-mr2
  3. 代码下载与编译流程请请根据参考网页中的步骤进行，以下是备注。
     * repo init时记得加入-b选项：repo init -u https://android.googlesource.com/platform/manifest -b android-4.4.3_r1.1
     * 预计代码量为15G左右，根据网络环境可能需要重新运行多次repo sync命令（能够断点续传），建议使用ipv6下载（可以断开ipv4下载，正常情况下不用额外配置）
     * 代码下载成功后，首先修改<Android源码根目录>/system/core/adb/adb.c中的should_drop_privileges()为：`static int should_drop_privileges(){ return 0; }`(即将原函数中所有有效行都注释掉，直接返回0）
     * 使用lunch aosp_hammerhead-userdebug
        
# 关于nexus5手机内核的修改
  1. 我们使用的内核版本为android-msm-hammerhead-3.4-kitkat-mr2，请在https://android.googlesource.com/kernel/msm/自行下载。
  2. 主要的修改在trace.c文件中，编译时需要替换<内核根目录>/kernel/trace/trace.c，编译。
  3. 实验性的修改在ring_buffer.c中，可以实现更多的内存方面的控制，但不稳定，可以不包括在内核的编译中。

# 关于系统级APK的签名
  1. 需要文件：编译好的apk、签名工具signapk.jar、android代码中的platform.pk8与platform.x509.pem
     * apk的源代码在本仓库下AndroidTestUSBTethering.zip，apk的编译不再另行说明
     * signapk.jar由<Android源码根目录>/build/tools/signapk/编译产出,可以在/out/host/...../framework/中找到，本仓库中也有（根目录下）。
     * 可以在<Android源码根目录>/build/target/product/security/中找到platform.pk8 platform.x509.pem等签名文件
  2. 运行命令：java -jar signapk.jar platform.x509.pem platform.pk8 MyDemo.apk MyDemo_signed.apk 得到具有对应权限的APK
        
# 其他
1. N5手机的连接方式
    * adb shell
2. 利用adb和fastboot替换N5手机的内核
    1. 编译内核生成zImage-dtb
    2. 利用仓库中提供的ramdisk和镜像制作程序制作boot.img
       * 请确保ramdisk中default.prop里的ro.secure字段为ro.secure=0
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
4. 在192.168.1.37虚拟机上调用192.168.1.40机器上N5手机ftrace跟踪的方法
    1. 建立免密码登陆的ssh，参考：http://www.2cto.com/os/201301/186673.html
    2. 在1.40上运行ftrace数据接收程序（直接写文件的话不需要这一步）
    3. 在1.37上利用如下两条命令就可以实现远程调用与数据取回。
      * ssh root@192.168.1.40 'sh /home/wjbang/test.sh'
      * scp root@192.168.1.40:/dev/shm/test/ftrace.data /home/wjbang/
      * 注：test.sh是在1.40机器上对应的ftrace启动脚本
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



