#include <unistd.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>

#include <sys/sysinfo.h>
#include <time.h>

#define __USE_GNU
#include <sched.h>
#include <pthread.h>
#include <errno.h>

// include/linux/compiler.h
#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)


long long lock_counter = 0;

int nr_parallel_threads = 0;

pthread_barrier_t global_barrier;
pthread_rwlock_t __lock;

int thread_stop=0;

#include "../include/tony_pthread_data.h"
#include "../include/tony_misc.c"
#include "../include/nop_delay.h"
// introduce  a header file to map cpu No 
#include "../include/map_cpu.h"
struct timespec tm_start;
struct timespec tm_ended;
//Introduction of global variables to calculate thread runtime author:hh
struct thread_data*  thread_param;

void *thread_routine_reader(void *arg)
{
    struct thread_data* parg = (struct thread_data*) arg;
    //int cpu_id = parg->cpu_id;
    //int i=0;
    long long counter=0;
    // bind to the core test to add the following code
    // tony: do not bind
    /*cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu_id, &set);
        //printf("tid:%d,cup_id%d",gettid(),cpu_id);
    if (sched_setaffinity(gettid(), sizeof(cpu_set_t), &set)) {
        perror("in:sched_setaffinity");
        return NULL;
    }*/


    /* The pthread_barrier_wait() function shall synchronize participating threads
     * at the barrier referenced by barrier.
     * The calling thread shall block until
     * the required number of threads have called pthread_barrier_wait() specifying the barrier.
     */
    pthread_barrier_wait(&global_barrier);

    // while (likely(thread_stop == 0)) {
    while(1){
        pthread_rwlock_rdlock(&__lock);
        parg->count++;
        pthread_rwlock_unlock(&__lock);
    }
        /*
    int i = 0;
    while(i<1000){

        __sync_fetch_and_add(&lock_counter, 1);
                i++;
        }*/
    /*
        for(long long i = 0; i < parg->lock_interval; i++)
        {
                NOP_DELAY_72
        }
        counter++;
    }

    parg->lock_times = counter;
    */
    return NULL;
}
//Introduce global variables to pass in the number of lock_counts for a single thread
void *thread_routine_writer(void *arg)
{
    struct thread_data* parg = (struct thread_data*) arg;
    //int cpu_id = parg->cpu_id;
    //int i=0;
    long long counter=0;
    // bind to the core test to add the following code
    // tony: do not bind
    /*cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu_id, &set);
        //printf("tid:%d,cup_id%d",gettid(),cpu_id);
    if (sched_setaffinity(gettid(), sizeof(cpu_set_t), &set)) {
        perror("in:sched_setaffinity");
        return NULL;
    }*/


    /* The pthread_barrier_wait() function shall synchronize participating threads
     * at the barrier referenced by barrier.
     * The calling thread shall block until
     * the required number of threads have called pthread_barrier_wait() specifying the barrier.
     */
    pthread_barrier_wait(&global_barrier);

    // while (likely(thread_stop == 0)) {
    while(1){
        pthread_rwlock_wrlock(&__lock);
        parg->count++;
        pthread_rwlock_unlock(&__lock);
    }
        /*
    int i = 0;
    while(i<1000){

        __sync_fetch_and_add(&lock_counter, 1);
                i++;
        }*/
    /*
        for(long long i = 0; i < parg->lock_interval; i++)
        {
                NOP_DELAY_72
        }
        counter++;
    }

    parg->lock_times = counter;
    */
    return NULL;
}



void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		int i=0;
		for(i=0;i<nr_parallel_threads;i++){
			printf("\nThe thread %d,count:%d\n",i,thread_param[i].count);
		}//same as no-binding core test
		printf("lock_counter=%ld\n", lock_counter);
		clock_gettime(CLOCK_MONOTONIC, &tm_ended);  // Get the monotonic time at the end of the thread      author£ºhh
		printf("starttime:sec:%lld,nsec:%lld\n",tm_start.tv_sec,tm_start.tv_nsec);
		printf("endtime:sec:%lld,nsec:%lld\n",tm_ended.tv_sec,tm_ended.tv_nsec);
		printf("running time:%lld\n",(tm_ended.tv_sec-tm_start.tv_sec)*1000000000+tm_ended.tv_nsec-tm_start.tv_nsec);
		exit(0);
	}
}



int main(int argc, char *argv[])
{


    if (argc < 6)
	{
		//      0 ,1 ,2 ,3 ,4 ,5 ,6 ,7
        printf("argv[0] $execute_duration $r_threads $rlock_delay $w_threads $wlock_delay\n");
        exit(EXIT_FAILURE);
    }

        /* how long this program to run in seconds */
	int exe_duration = atoi(argv[1]);

	/* read lock thread number */
	int nr_r_threads = atoi(argv[2]);

	/* read lock delay to adjust lock request frequency in nanoseconds */
	int rlock_delay = atoi(argv[3]);

	/* write lock thread number */
	int nr_w_threads = atoi(argv[4]);

	/* write lock delay to adjust lock request frequency in nanoseconds */
	int wlock_delay = atoi(argv[5]);
	
	//int bind_type = atoi(argv[6]);
	nr_parallel_threads = nr_r_threads + nr_w_threads;
    

	/*if(bind_type<0 || bind_type>8)
	{
			perror("bind_type");
			exit(EXIT_FAILURE);
	}*/
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
			printf("failed to install signal handler\n");
			exit(1);
	}

	pthread_barrier_init(&global_barrier, NULL, nr_parallel_threads);
	
	pthread_rwlock_init(&__lock, NULL);
	
	pthread_t *thread_list = (pthread_t *)malloc(sizeof(pthread_t) * nr_parallel_threads);
	if (thread_list == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
	}

	thread_param = (struct thread_data*) calloc(nr_parallel_threads, sizeof(struct thread_data));

	if (thread_param == NULL) {
			perror("calloc");
			exit(EXIT_FAILURE);
	}
	/*Index* index = mapping(nr_parallel_threads, bind_type);
	if(index==NULL) {
			perror("mapping");
			exit(EXIT_FAILURE);
	}*/
	clock_gettime(CLOCK_MONOTONIC, &tm_start);
	
	for (int i = 0; i < nr_r_threads; ++i) {
			//thread_param[i].cpu_id = get_proc_no(index, i);
			thread_param[i].thread_id=i;
			thread_param[i].lock_interval=rlock_delay;
			pthread_create(&thread_list[i], NULL, thread_routine_reader, thread_param + i);
	}
	
	
	for (int i = nr_r_threads; i < nr_parallel_threads; ++i) {
			//thread_param[i].cpu_id = get_proc_no(index, i);
			thread_param[i].thread_id=i;
			thread_param[i].lock_interval=wlock_delay;
			pthread_create(&thread_list[i], NULL, thread_routine_writer, thread_param + i);
	}
	
	nsleep(exe_duration, 0);  // Initially the second false parameter does not pass, so changed to 0 author:hh
													  // notify all threads stop and exit
	thread_stop = 1;

	clock_gettime(CLOCK_MONOTONIC, &tm_ended);
 	long long nr_rd_lock = 0;
	long long nr_wr_lock = 0;

	for (int i = 0; i < nr_r_threads; i++)
	{
		nr_rd_lock += thread_param[i].count;
	}

	for (int i = nr_r_threads; i < nr_parallel_threads; i++)
	{
		nr_wr_lock += thread_param[i].count;
	}
	
	lock_counter = nr_rd_lock + nr_wr_lock;
    printf("%lld,%lld,%lld,",nr_rd_lock/exe_duration,nr_wr_lock/exe_duration,lock_counter/exe_duration);

    return 0;
}
