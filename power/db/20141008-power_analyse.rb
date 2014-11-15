#!/usr/bin/ruby -w
require 'power'
require 'mysql'

$kernel_version=ARGV[0] #"Android-4.4.3"
$directory_type=ARGV[1] #"R"
$platform=ARGV[2] #"arm-Nexus"
#$testcase=
if $directory_type == "real"
	$directory_type="R"
else
	$directory_type="V"
end

$option=$kernel_version+"_"+$directory_type+"_"+$platform+"_"

include Power
puts load_PowerData("power.dat");

	power_begin=(`cat power.dat | head -n 1`).split(" ")[0].to_i
	power_end=(`cat power.dat | tail -n 1`).split(" ")[0].to_i
#	puts power_begin
#	puts power_end

fname=""
ctime=""
rtime=""

mydb=Mysql.connect('localhost', 'cgrtl', '9-410', 'callgraph')	#connect to mysql
rsD=mydb.query("SELECT F_point, C_time, R_time, DLIST_id FROM `#{$option+"DLIST"}` WHERE F_point!=0")	#select infomation(F_point, C_time, R_time) from DLIST
rsD.each_hash do |rowD|
	rsF=mydb.query("SELECT f_name FROM `#{$option+"FDLIST"}` WHERE f_id=\"#{rowD['F_point']}\"")	#select f_name from FDLIST
	rsF.each_hash do |rowF|
		fname=rowF['f_name']
	end
	ctime=rowD['C_time'].to_i(16)*100
	rtime=rowD['R_time'].to_i(16)*100
#puts fname
#puts ctime
#puts rtime

	 	
	if (ctime>power_begin or ctime==power_begin) and (ctime<power_end or ctime==power_end)
#		puts ctime
#		puts fname
		consumption=get_Power(0, "#{fname}", "#{ctime}", "#{rtime}");
#		puts consumption
		mydb.query("UPDATE `#{$option+"DLIST"}` SET E_consumption=#{consumption} WHERE DLIST_id=#{rowD['DLIST_id']}")
	end

end

#puts get_Power(0, "name", "1694535421870", "1694535421960");
#puts get_Power(0, "#{fname}", "1236195630235", "1236196216605");
#puts get_Power(0, "name", "1236195630235", "1236196645791");
