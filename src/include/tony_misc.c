#ifndef __CYGWIN__
#include <sys/syscall.h>

pid_t gettid(void)

{

            return syscall(__NR_gettid);

}

#endif

void nsleep(double seconds, _Bool sleep_with_abs_time) 
	// The parameter type was changed from "bool" to _Bool type. Reason: C99 type compilation failed.	author:hh

{

        long int sec  = (long int)(seconds);

        long int nsec = (long int)((seconds - sec) * 1000000000);

 

        if (!sleep_with_abs_time) {  /* relative time sleep */

                struct timespec tv[2] = {

                        {

                                .tv_sec  = sec,

                                .tv_nsec = nsec,

                        },

 

                        {

                                .tv_sec  = 0,

                                .tv_nsec = 0,

                        }

                };

 

                int req = 0;

                int rem = 1;

                while (-1 == clock_nanosleep(CLOCK_MONOTONIC, 0, &tv[req], &tv[rem])) {

                        if (EINTR == errno) {

                                req ^= rem; rem ^= req; req ^= rem;

                                continue;

                        } else {

                                perror("clock_nanosleep");

                                return;

                        }

                }

 

        } else {  /* absolute time sleep */

                struct timespec req;

                struct timespec rem;

 

                if (clock_gettime(CLOCK_MONOTONIC, &req) != 0) {

                        perror("clock_gettime");

                        return;

                }

 

                req.tv_sec  += sec;

                req.tv_nsec += nsec;

 

                while (-1 == clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &req, &rem)) {

                        if (EINTR == errno) {

                                continue;

                        } else {

                                perror("clock_nanosleep");

                                return;

                        }

                }

        }

}

 

void nano_sleep(long long nano)

{

        long int sec  = (long int)(nano / 1000000000);

        long int nsec = (long int)(nano % 1000000000);

 

        struct timespec tv[2] = {

                {

                        .tv_sec  = sec,

                        .tv_nsec = nsec,

                },

 

                {

                        .tv_sec  = 0,

                        .tv_nsec = 0,

                }

        };

 

        int req = 0;

        int rem = 1;

        while (-1 == clock_nanosleep(CLOCK_MONOTONIC, 0, &tv[req], &tv[rem])) {

                if (EINTR == errno) {

                        req ^= rem; rem ^= req; req ^= rem;

                        continue;

                } else {

                        perror("clock_nanosleep");

                        return;

                }

        }

 

 

}
