#!/bin/bash

for ip in `grep "==>" /home/vic/work/scripts/testip | grep -v null | awk '{print $1}'`
do
(
	#echo $ip
	#取文件中的ip地址，ping ip看是否正常.
	lostrate=`ping -c 2 -w 2 $ip | grep 'packet loss' | awk -F'packet loss' '{print $1}' | awk '{print $NF}' | sed 's/%//g'`
	#echo $lostrate
	if [ $lostrate -eq 0 ];then
		msg="${ip}: is ok."
		echo $msg
	elif [ $lostrate -lt 100 ];then
		msg="${ip}: network is not perfect."
		echo $msg
	else
		msg="${ip}: is down."
		echo $msg;
		echo "change config file..start.."
		#grep $ip /home/vic/work/scripts/testip | 
		sudo sed -i 's/^'${ip}'/#'${ip}'/g' /home/vic/work/scripts/testip
		echo "change config file..end.."
	fi
)
done
