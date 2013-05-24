#/bin/bash

num=`cat tcpget.txt | wc -l`
echo $num

while read line
do
#echo $line
tax=`echo $line | grep '^TAX' |awk '{
	s1=\`echo "$3"|sed -n "s/^[a-zA-Z]*$//g"\`
	printf "%s,%s,%s\n",$s1,$5,$7}'`
echo $tax
done < tcpget.txt
