\# Goals

To help understand various functionalities of the shell:  From the kernel's perspective, the shell is another application program that uses system calls to spawn and to terminate other user programs, e.g., sequentially or concurrently.



Implement a simple shell that:

* reads a single command line :construction:
* spawns a child process :construction:
* has the child execute the command :construction:
* checks if the shell needs to wait for the child termination with checks if the shell needs to wait for the child termination with ‘;’ or ‘\&’ delimiters :construction:
* bring back the latest background process to foreground execution with “fg” :construction:



\# Workloop

1. Display a prompt to indicate that it is ready to accept a next command (aka an affordance)
2. Reading a line of input from the keyboard

   1. Split into commands as needed
3. Spawning and having a new process execute the command



\# Shell

1. The shell locates an executable file whose name is specified in the first string given from a keyboard input.

2\. It creates a child process by duplicating itself.

3\. The duplicated shell overloads its process image with the executable file.

4\. The overloaded process receives all the remaining strings given from a keyboard input, and starts a command execution.



\# shell.cpp Rubric

* Check 'exit'
* Check fg and move background process to foreground
* Fork a child
* Execvlp
* waitpid
* push into stack if '\&'

