2B
https://pages.cs.wisc.edu/~remzi/OSTEP/

https://www.skills.google/catalog?event=Your&keyworks=

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