#ifndef _TONY_PTHREAD_DATA_H_

#define _TONY_PTHREAD_DATA_H_ 1

 

#include <time.h>

struct thread_data

{

        int thread_id;

        int cpu_id;

        long long lock_interval;

        long long lock_times;

        struct timespec exe_duration;

        pthread_t pthread;
		
		int pad1;
		
		long long count;	//Add a single thread lock_count member variable. 	author:hh

};

 

#endif /* _TONY_PTHREAD_DATA_H_ */

