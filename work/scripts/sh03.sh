#! /bin/bash

s=0
i=0

while [ "$i" != "100" ]
do
	i=$(($i+1))
	s=$(($s+$i))
done
echo "sum is $s"

#s=0 # 这是加总的数值发数
#i=0 # 这是累计的数值,亦即是 1, 2, 3....
#while [ "$i" != "100" ]
#do
#i=$(($i+1)) # 每次 i 都会增加 1
#s=$(($s+$i)) # 每次都会加总一次!
#done
#echo "The result of '1+2+3+...+100' is ==> $s"

