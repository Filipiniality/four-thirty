Hardware level can't suspend threads.

sem_wait and sem_post (aka signal)

wait() : "to test"
signal() : "to increment"

In the C++ language
Instead of having thread sleep, keep it in waiting state so it is good to go.

Running in parallel - WON'T be pre-empted (we have 6 CPUs)

[Producer]		[Consumer]
----------		----------
[CPU_Core_0]		[CPU_Core_1]
----------		----------
[cache]			[cache]
  lock			  lock
special registers	special registers
----------		----------
----------------------------------
|	   Shared Cache		  |
.---------------------------------.
|	      DRAM		  |
     *bounded buffer is here
     *state of lock is here
     *CPU can directly access
	latest value of lock
     *you can invalidate the contents
	of a cache you don't need
	   (cache thrashing)
._ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _.

Because of the spin-lock (constant checking), the hardware implementation takes longer

In OS version, the data is kept adding until buffer becomes full
	Meaning: producer may keep inserting until buffer says so
		 consumer may keep removing until empty

sem_wait		sem_wait
count++			count--
sem_post		sem_post

They look at the same shared memory

Item 3:

Why OS semaphore runs faster than HW while we are using a machine with 6 cores.

CPU's cache in relation to memory. CPU affinity. CACHE WRITE-BACK

