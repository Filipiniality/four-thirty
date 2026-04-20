* https://man7.org/linux/man-pages/man2/fork.2.html
* https://man7.org/linux/man-pages/man3/exec.3.html
* https://man7.org/linux/man-pages/man2/pipe.2.html
* https://man7.org/linux/man-pages/man2/dup.2.html
* https://www.rozmichelle.com/pipes-forks-dups/

* fork() {returns pid_t}
* execlp(path, executable, additional arguments) {returns integer to respresent status}
* pipe(integer array with size 2 but no entries YET)
* dup2(file descriptor to original pipe end, [0/1] decide if read/write )
* wait(pointer) {returns PID of child}

>  We start off by creating a pipe. Then we fork a child process. The parent will use the pipe for command output. That means it needs to change its standard output file descriptor (1) to the writing end of the pipe ([row of fds array][1]). It does this via the dup2 system call: dup2([row of fds array][1], 1). Then it executes the command in cmd1.
> 
> The child will use the pipe for command input. It needs to change its standard input file descriptor (0) to the reading end of the pipe (pfd[0]). It also does this via the dup2 system call: dup2(pfd[0], 0). Then it executes the command in cmd2.
https://web.archive.org/web/20240318155912/https://people.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html


> You will have to create as many pipe variables as there are "diversion". then create a list of commands.  
** if you want parent process leave,you would fork a process for each command. otherwise one less.**  
for the very first command, dup or 'dup2' only for STDOUT.  
and for the very last command dup or 'dup2' only for STDIN.  
for the rest, dup or 'dup2' for STDIN and STDOUT both.  
https://stackoverflow.com/questions/20826595/two-redirect-in-program

# Goal
To get familiar with with Linux programming using system calls such as fork, execlp,
wait, [pipe](https://www.guru99.com/linux-pipe-grep.html), [dup2](https://www.geeksforgeeks.org/c/dup-dup2-linux-system-call/), close, write, read

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