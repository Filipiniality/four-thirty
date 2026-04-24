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
  if ((pid = fork()) < 0) { // fork a child
    perror("fork error");
  }
  else if (pid == 0) { // Child is born

    if (pipe(fds[0]) < 0) {
      perror("pipe failed");
    }
    if ((pid = fork()) < 0) {
      perror("fork failed");
    }
    else if (pid == 0) { // Grand-child is born

      if (pipe(fds[1]) < 0) {
        perror("pipe failed");
      }
      if ((pid = fork()) < 0) {
        perror(" fork failed");
      }
      if (pid == 0) { // Great-grand-child is born and dies
        // 'STDOUT' of ps redirects to 'STDIN' of grep
        dup2(fds[1][1],1);
        close(fds[1][0]);

        execlp("cat", "cat", "Spleen.txt", NULL);

        // ps is purely a producer and does no reading
        close(fds[1][1]);
      }
      else { // Grand-child dies
        // 'STDOUT' of grep is redirected to 'STDIN' of wc
        dup2(fds[0][1], 1);
        // 'STDIN' of grep is redirected from 'STDOUT' of ps
        dup2(fds[1][0], 0);

        close(fds[1][1]);
        close(fds[0][0]);

        wait(NULL);
        execlp("grep", "grep", argv[1], NULL);

        // this end of pipe fulfilled its duty
        close(fds[0][1]);
      }
    }
    else { // Child dies
      // 'STDIN' of wc is redirected from 'STDOUT' of grep
      dup2(fds[0][0],0);
      close(fds[0][1]);

      wait(NULL);
      execlp("wc", "wc", "-l", NULL);
      // wc is purely a consumer and does no writing
    }
  }
  else { // Parent dies
    // I'm a parent (represents command interpreter)
    wait(NULL);
    cout << "commands completed" << endl;
  }
}