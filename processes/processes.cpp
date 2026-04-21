#include <sys/types.h>   // for fork, wait
#include <sys/wait.h>    // for wait
#include <unistd.h>      // for fork, pipe, dup, close
#include <stdio.h>       // for NULL, perror
#include <stdlib.h>      // for exit

#include <iostream>      // for cout

using namespace std;

int main(int argc, char** argv) {
  int fds[2][2];
  int pid;

  if (argc != 2) {
    cerr << "Usage: processes command" << endl;
    exit(-1);
  }

  // fork a child
  if ((pid = fork()) < 0) {
    perror("fork error");
  }
  else if (pid == 0) { // Child is born, will execute 'who'
    if (pipe(fds[0]) < 0) {
      perror("pipe failed");
    }

    if (pid = fork() < 0) {
      perror("fork failed");
    }
    else if (pid == 0) { // Grand-child is born, will execute 'grep'
      if (pipe(fds[1]) < 0) {
        perror("pipe failed");
      }

      if (pid = fork() < 0) {
        perror(" fork failed");
      }

      if (pid == 0) { // Great-grand-child is born, executes 'ps', dies
        // 'STDOUT' of GGC is redirected to GC's 'STDIN'
        dup2(fds[0][1], 1);
        close(fds[0][0]);
        
        execlp("usr/bin/ps", "ps", "-A", NULL);
      }
      else { // Grand-child executes grep, dies
        // FD 'STDOUT' of GC is redirected to C's 'STDIN'
        dup2(fds[0][0], 0);
        close(fds[0][1]);
        //'STDIN' of GC is redirected from GCC 'STDOUT'
        dup2(fds[1][1], 1);
        close(fds[1][0]);

        execlp("usr/bin/grep", "grep", argv[1], NULL);
      }
    }
    else { // Child executes wc, dies
      // 'STDIN' of C is redirected from GC's 'STDOUT'
      dup2(fds[1][0], 0);
      close(fds[0][1]);

      execlp("usr/bin/wc", "wc", "-l", NULL);
    }
  }
  else { // Parent dies
    // I'm a parent (represents command interpreter, excutes wait)
    wait(NULL);
    cout << "commands completed" << endl;
  }
}