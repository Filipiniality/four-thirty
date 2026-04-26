The process is the ADDRESS SPACE and OS RESOURCES
Threads run in the CONTEXT of the PROCESS

A process is a container of threads
	It only needs to save Program Counter and other registers

Preemptive- in RR, the scheduler interrupts a job if it is not completed by x quanta

Standard API POSIX threads, or Pthreads

* https://pubs.opengroup.org/onlinepubs/009695399/basedefs/pthread.h.html
* https://en.cppreference.com/c/program/jmp_buf
* https://man7.org/linux/man-pages/man3/longjmp.3.html
* https://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/notes/Memory/lecture.html
* https://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/notes/Malloc1/lecture.html

---

Thread needs its own stack
Think about recursion

You want the contents of stack space to go into heap

Should be 14-16 lines

g++ driver.cpp

./a.out

---

capturing current status of thread
passing execution to next thread

---

Already have scheduler implemented
User-level thread control block is already defined
create() and exit() implemented

---

UNNECESSARY:
Keep track of BP (base pointer)
    Keep base function @ original thread function
```
scheduler_init scheduler_start PROPER scheduler()
>> main_env << 						global variable, visible everywhere (static keyword)
	SP, BP, PC, 	Check pointer
and scheduler_env are set to				global
	SP, BP, PC, 
cur_tcb_env						thread private
	SP, BP, PC, 
```
---

schedule_init is just a continuation of main()

driver.cpp
	schedule_init
		memorize where main thread is running (setjmp)
		call schedule
		accept timer signals

Back to schedule_init
Is done, now go to func1.
	go to sthread_create (execute in context of main func)
		memorize main’s context
		memorize address to function and argument
		thread is being created
		create new tcb
	jump back to scheduler (longjmp)
		in scheduler, return 1, not 0
		returns to scheduler_env
			will NOT execute the acceptance of signals.
			actual if block: check if thread was created
				START THIS THREAD
				thread created flag is reset to FALSE
				Thread timer is NOT restarted

Scheduler_start
	memorize current main environment
	in first invocation, longjmp to scheduler() context
	jump to next if-block
		check top of q
		KICK OUT TCB 1
	activate timer in 5 seconds
	longjmp to this thread’s environment


---

capture()
	captures sp bp stack
	pushes current tcb into queue
yield()
	works only when alarmed
	correctly uses setjmp/longjmp
	retrieves new thread's stack

setjmp =
longjump = CPU's activation record

Report: Including RR context switch snapshots

Report: Include illustration of stack layers