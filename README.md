
#cd script
 
 
Usage£ºbenchmark [OPTION]... [PARM]...
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
