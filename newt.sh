#!/bin/bash
result=""
t_num=4
result=$result$(./bk.sh -e 5 -t $t_num -d 0 -s -p)
sleep 1
result=$result$(./bk.sh -e 5 -t $t_num -d 0 -s -c)
sleep 1
echo $result>>multi.txt
