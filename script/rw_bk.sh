#!/bin/bash
set -- `getopt -q -o scpfhe: --long rt:,rd:,wt:,wd: -- "$@" `

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
 	--rt)	rt_num=$(echo $2|tr -d "'")
         shift ;;
	--wt)	wt_num=$(echo $2|tr -d "'")
         shift ;;
	-e)    exe_time=$(echo $2|tr -d "'")
         shift ;;
    	--rd) rlock_delay=$(echo $2|tr -d "'")
         shift ;;	
	--wd) wlock_delay=$(echo $2|tr -d "'")
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

if [ -n "$exe_time" -a -n "$rlock_delay" -a -n "$wlock_delay" ];then
	if [ -n "$(echo $exe_time| sed -n "/^[0-9]\+$/p")" ]&&[ -n "$(echo $rlock_delay| sed -n "/^[0-9]\+$/p")" ]&&[ -n "$(echo $wlock_delay| sed -n "/^[0-9]\+$/p")" ] ;then
		if [ $mode -eq 8 ];then
			if [ -n "$rt_num" -a -n "$wt_num" ]; then
				#vim benchmark.conf
				#t_num=$(head -1 benchmark.conf)
				make file_name=../src/lock_code/test_rw_lock_nobind.c exe_name=../src/bin/test_rw_lock_nobind>/dev/null
				../src/bin/test_rw_lock_nobind $exe_time $rt_num $rlock_delay $wt_num $wlock_delay
				exit 3
			fi
		else
			if [ -n "$rt_num" ]&&[ -n "$wt_num" ]&&[ -n "$(echo $rt_num| sed -n "/^[0-9]\+$/p")" ]&&[ -n "$(echo $wt_num| sed -n "/^[0-9]\+$/p")" ];then
				echo
			else
				echo "-rt or -wt param invalid!"
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
make file_name=../src/lock_code/test_rw_lock.c exe_name=../src/bin/test_rw_lock>/dev/null
../src/bin/test_rw_lock $exe_time $rt_num $rlock_delay $wt_num $wlock_delay $mode









