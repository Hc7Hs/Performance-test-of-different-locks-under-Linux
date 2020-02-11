#Performance test of different locks under Linux(lock-perf-benchmark)
 
Usage: benchmark [OPTION]... [PARM]...
Test the extreme performance of multi-threaded CPUs. Provide multiple modes: bind (not bind) Socket, bind (not bind) Core, bind (not bind) hyperthread; you can set parameters such as test time, delay, number of threads.


The following is a detailed description of the OPTION and PARM for this test program:

-s 		socket		The value of PARM is 0 or 1, 0 means that the test is not bound to Socket, and 1 means the test of binding Socket;

-c 		core		The value of PARM is 0 or 1, 0 means that the test is not bound to the Core, and 1 means the test of binding the Core;

-p		processor	The value of PARM is 0 or 1, 0 means that the test is not bound to the Processor, and 1 means the test of binding the Processor;

-t		thread		The value of PARM is a positive integer, which is the number of threads you need to test;

-e		excute_duration		The value of PARM is a positive integer, the unit is Second, which is the time when the test program runs;

-d		Lock_delay 		The value of PARM is a positive integer in seconds, which is the time delay for the test program to execute.

-f		float mode 	provide a mode to not set the binding-process, core or socket.

-h		help		Open the help document

#Linux下不同锁的性能测试（lock-perf-benchmark）
##用法：benchmark [OPTION] ... [PARM] ...测试多线程CPU的极限性能。提供多种模式：绑定（不绑定）套接字，绑定（不绑定）核心，绑定（不绑定）超线程；您可以设置参数，例如测试时间，延迟，线程数。

##以下是此测试程序的OPTION和PARM的详细说明：

-s socket PARM的值为0或1，0表示测试未绑定到Socket，1表示绑定Socket的测试；

-c core PARM的值为0或1，0表示测试未绑定到Core，1表示绑定Core的测试；

-p处理器PARM的值为0或1，0表示测试未绑定到处理器，而1表示绑定处理器的测试；

-t thread PARM的值是一个正整数，这是您需要测试的线程数；

-e excute_duration PARM的值是一个正整数，单位是Second，它是测试程序运行的时间；

-d Lock_delay PARM的值是以秒为单位的正整数，这是测试程序执行的时间延迟。

-f 浮动模式提供一种不设置绑定进程，核心或socket的模式。

-h help打开帮助文档
