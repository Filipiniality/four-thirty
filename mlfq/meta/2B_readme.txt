2B
https://pages.cs.wisc.edu/~remzi/OSTEP/

Implement MLFQ and compare to RR

Queue 0=1slice
Queue 1=2slice
Queue 3=4slice

Each slice per second, check if any processes exist in higher-priority queues

If current process used up slices of current queue, it will go to the next lower queue
or stay in the lowest queue

TQ of 2 seconds

Question: After proceeding, how do you get the process to run?
	check: is slice used? Pick up process.
		if queue has nothing, go down. Know when to pick up from slice

queue= list of PIDs
slice= how many quantums picked up?

//////////////////////////////////////////////////////////////////////////////////

DMA acts like another process, can lead to data inconsistencies
Data in DRAM must be consistent

//////////////////////////////////////////////////////////////////////////////////

don't forget to reset slices[level] when it hits the threshold
Make sure to resume only an active process.

If sig is 0, then no signal is sent, but error checking is still performed; this can be used to check for the existence of a process ID or process group ID.
	Kill returns -1 if error

//////////////////////////////////////////////////////////////////////////////////

1. It has three queues, numbered from 0 to 2, (i.e., queue[0], queue[1], and queue[2]). Level 0 is the highest
while level 2 is the lowest. (Already implemented.)
2. A PID is always enqueued into queue[0] in addProcess( pid ). (Already implemented.)
3. Your scheduler first executes all processes in queue 0. The queue 0's time quantum is 1 second. (Already implemented.)

4. If a process in queue[0] does not complete its execution for queue[0]’s time slice, (i.e., 1 sec ), the scheduler moves the corresponding pid to queue[1].

5. If queue[0] is empty, it will execute processes in queue[1].
Queue[1]'s time quantum is twice larger than queue[0]’s, (i.e. 2 seconds).
However, in order to respond to new processes in queue[0], your scheduler should execute a process in queue[1] for 1 sec and then check if queue[0] has new PIDs.
If so, it will execute all processes in queue[0] first,
and thereafter resume the execution of the same process in queue[1] for another 1 second.

6. If a process in queue[1] does not complete its execution for queue[1]'s time quantum, (i.e., 2 seconds),
the scheduler then moves its PID to queue[2].

7. If both queue[0] and queue[1] is empty, it can execute processes in queue[2]. The queue[2]'s time quantum is a double of queue[1], (i.e., 4 seconds). However, in order to respond to processes with higher priority in queue[0] and queue[1], your scheduler should execute a process in queue[2] for every 1 second and then check if queue[0] and queue[1] have new pids. The rest of the behavior is the same as that for queue[1].

8. If a process in queue[2] does not complete its execution for queue 2's time slice, (i.e., 4 seconds), the scheduler puts it back to the tail of queue[2]. (This is different from the textbook example that executes processes in queue 2 with FCFS.)