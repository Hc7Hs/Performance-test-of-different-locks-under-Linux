#!/bin/bash

#author :H&H

socket_num=$(cat /proc/cpuinfo| grep 'physical id'| sort| uniq| wc -l)
core_num=$(cat /proc/cpuinfo| grep 'core id'| sort| uniq| wc -l)
proc_num=$(cat /proc/cpuinfo| grep 'processor'| sort| uniq| wc -l)
ht_num=$(($proc_num/($core_num*$socket_num)))
#k is loop variable
k=0
for((i=0;i<$socket_num;i++))
do
	index=$index" "$i
	for((j=0;j<$core_num;j++))
		do
			index=$index" "$j
			step=0;
			for((t=0;t<ht_num;t++))
				do
					index=$index" "$(($k+$step))
					step=$(($step+$core_num*$ht_num))
				done
			k=$(($k+1))
		done
done
#echo $index
printf "%d\n%d\n%d\n%d\n" $socket_num $core_num $proc_num $ht_num>../src/out.txt 
echo $index>>../src/out.txt

