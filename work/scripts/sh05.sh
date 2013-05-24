#!/bin/bash

read -p "please input your birthday-date(MMDD): " bir

now=`date +%m%d`

if [ "$bir" == "$now" ];then
	echo "today is your birthday."
elif [ "$bir" -gt "$now" ];then
	year=`date +%Y`
	daynum=$(($((`date --date="$year$bir" +%s`-`date +%s`))/24/60/60))
	echo "after $daynum days is your birthday!"
else
	year=$((`date +%Y`+1))
	daynum=$(($((`date --date="$year$bir" +%s`-`date +%s`))/24/60/60))
	echo "after $daynum days is your birthday!"
fi

#read -p "Pleas input your birthday (MMDD, ex> 0709): " bir
#now=`date +%m%d`
#if [ "$bir" == "$now" ]; then
#	echo "Happy Birthday to you!!!"
#elif [ "$bir" -gt "$now" ]; then
#	year=`date +%Y`
#	total_d=$(($((`date --date="$year$bir" +%s`-`date +%s`))/60/60/24))
#	echo "Your birthday will be $total_d later"
#else
#	year=$((`date +%Y`+1))
#	total_d=$(($((`date --date="$year$bir" +%s`-`date +%s`))/60/60/24))
#	echo "Your birthday will be $total_d later"
#fi

