#include <sys/types.h>   // for fork, wait
#include <sys/wait.h>    // for wait
#include <unistd.h>      // for fork, pipe, dup, close
#include <stdio.h>       // for NULL, perror
#include <stdlib.h>      // for exit

#include <iostream>      // for cout

using namespace std;

int main( int argc, char** argv ) {
  int fds[2][2];
  int pid;

  if ( argc != 2 ) {
    cerr << "Usage: processes command" << endl;
    exit( -1 );
  }

  // fork a child
  if ( ( pid = fork( ) ) < 0 ) {
    perror( "fork error" );
  }
  else if ( pid == 0 ) { // Child is born, will execute 'who'
    // Pipe A
    if (pipe(fds[0]) < 0) {
      perror( "pipe failed" );
    }

    if (pid = fork() < 0) {
      perror( "fork failed" );
    }
    else if (pid == 0) { // Grand-child is born, will execute 'grep'
      // Pipe B
      if (pipe(fds[1]) < 0) {
        perror("pipe failed");
      }

      if (pid = fork() < 0) {
        perror(" fork failed");
      }

      if (pid == 0) { // Great-grand-child is born, executes 'ps', dies
        // close every connection to Pipe B
        close(fds[1][0]);
        close(fds[1][1]);
        // purely a consumer that ignores the read-able end of Pipe A
        close(fds[0][0]);
        // File Descriptor 'STDOUT' of GGC is redirected to GC's 'STDIN'
        dup2(fds[0][1], 1);
        // execute ps
        execlp("usr/bin/ps", "ps", "-A", NULL);

      }
      else { // Grand-child dies
        // producer-consumer that ignore's write-able end of Pipe A and read-able ends of Pipe B
        close(fds[0][1]);
        close(fds[1][0]);
        // FD 'STDIN' of GC is redirected from GCC 'STDOUT'
        dup2(fds[0][0],0);
        // FD 'STDOUT' of GC is redirected to C's 'STDIN'
        dup2(fds[1][1], 1);

        // execute grep
        execlp("usr/bin/grep", "grep", argv[1], nullptr);
      }
    }
    else { // Child dies
      // close every connection to Pipe A
      close(fds[0][0]);
      close(fds[0][1]);
      //purely a producer that ignores the read-able end of Pipe A
      close(fds[1][1]);
      // FD 'STDIN' of C is redirected from GC's 'STDOUT'
      dup2(fds[1][0],0);

      // execute wc
      execlp("usr/bin/wc", "wc", "-l", nullptr);
    }
  }
  else { // Parent dies
    wait( NULL );
    cout << "commands completed" << endl;
  }
}
