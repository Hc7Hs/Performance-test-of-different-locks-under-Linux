#!/bin/bash

for i in $(seq 1200000 100000 2800000 )
#for i in 1500000 2500000
do	
	sudo ../../micro-benchmark/tools/powerManagement.sh  -f $i
	#for j in '0x0c0c' '0x1818' '0x0c18'
	#do
		#wrmsr -p 0 0x620 $j
		sleep 8
		./test_at.sh
		sleep 8
		./test_sl.sh
		sleep 8
		./test_mt.sh
		sleep 8
		./test_rw.sh
	#done
done
