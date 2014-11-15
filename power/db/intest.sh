X=/mnt/freenas/servers-transfer/powercat-ftrace-test
#./parse-for-all-funcgraph ftrace.data powercat-ftrace
ruby EnterDynamic-Nexux5.rb . android-4.4.3 real arm-Nexus5-test

ruby power_analyse.rb android-4.4.3 real arm-Nexus5-test
