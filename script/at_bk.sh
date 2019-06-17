#!/bin/bash
set -- `getopt -q scpt:e:d:fh "$@" `

#echo "param = $@"
is_tran_s=0
is_tran_c=0
is_tran_p=0
is_float=0
while [ -n "$1" ]
do
    case "$1" in 
    -s) is_tran_s=1
         	;;
	-c) is_tran_c=1
         	;;
	-p) is_tran_p=1
         	;;
 	-t)	t_num=$(echo $2|tr -d "'")
         shift ;;
	-e) exe_time=$(echo $2|tr -d "'")
         shift ;;
    -d) lock_delay=$(echo $2|tr -d "'")
         shift ;;	
	-f) is_float=1
		 ;;
	-h) more ../HELP.txt 
		exit 1		
		;;
    --) ;;

    *) echo "invalid param!" 
		exit 0
        break ;;
    esac
    shift	
done

if [ $is_float -eq 1 ];then
	mode=8
else
	mode=$((2#$is_tran_s$is_tran_c$is_tran_p))
fi

if [ -n "$exe_time" -a -n "$lock_delay" ];then
	if [ -n "$(echo $exe_time| sed -n "/^[0-9]\+$/p")" ]&&[ -n "$(echo $lock_delay| sed -n "/^[0-9]\+$/p")" ] ;then
		if [ $mode -eq 8 ];then
			if [ -n "$t_num" ]; then
				#vim benchmark.conf
				#t_num=$(head -1 benchmark.conf)
				make file_name=../src/lock_code/test_contest_nobind.c exe_name=../src/bin/test_contest_nobind>/dev/null
				../src/bin/test_contest_nobind $exe_time $t_num $lock_delay
				exit 3
			fi
		else
			if [ -n "$t_num" ]&&[ -n "$(echo $t_num| sed -n "/^[0-9]\+$/p")" ];then
				echo
			else
				echo "-t param invalid!"
				exit 3
			fi
		fi
	else
		echo "-e or -t or -d require int param!"
		exit 3
	fi
else 
	echo "please input enough param!"
	exit 4
fi

#echo "mode is $mode exe_time:$exe_time t_num:$t_num lock_delay:$lock_delay"
bash info_deal.sh
make file_name=../src/lock_code/test_contest.c exe_name=../src/bin/test_contest>/dev/null
../src/bin/test_contest $exe_time $t_num $lock_delay $mode









