* https://man7.org/linux/man-pages/man2/fork.2.html
* https://man7.org/linux/man-pages/man3/exec.3.html
* https://man7.org/linux/man-pages/man2/pipe.2.html
* https://man7.org/linux/man-pages/man2/dup.2.html
# Goal
To get familiar with with Linux programming using system calls such as fork, execlp,
wait, [pipe](https://www.guru99.com/linux-pipe-grep.html), [dup2](https://www.geeksforgeeks.org/c/dup-dup2-linux-system-call/), close, write, read

## Shell Implementation
Whenever the shell executes a new command, it spawns a child shell and lets the child execute the command. This behavior is implemented with the fork and execlp system calls.  
If the shell receives ";" as a command delimiter or receives no delimiter, it must wait for the termination of the spawned child, which is implemented with the wait system call.  
If it receives "&" as a command delimiter, it does not have to wait for the child to be terminated. If the shell receives a sequence of commands, each concatenated with "|", it must recursively create children whose number is the same as that of commands.

# Report
* Provide a diagram like those done in class (in the processes topic) including all the processes, pipe connections, file descriptors, system call, etc. Provide explanation as needed, to help explain your diagram or support your design choices.    
* Explain how you tested your processes program (consider all possible scenarios, positive and negative) 
* Output:  Include screenshots of the output from testing your processes program.
	* `ps -A | grep argv[1] | wc -l`

# Rubric
* Code correctness: A parent waits for a child
* Code correctness: A child executes `wc -l`
* Code correctness: A grand-child executes `grep argv[1]`
* Code correctness: A great-grand-child executes `ps -A`
* Report: The same output as `ps -A | grep argv[1] | wc -l`
* Report: Illustration of all processes and pipe connections
* Report: well organised and includes brief design discussion