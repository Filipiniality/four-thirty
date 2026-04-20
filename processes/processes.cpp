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
      }
      else { // Grand-child dies
        // execute grep
      }
    }
    else { // Child dies
      // execute wc
    }
  }
  else { // Parent dies
    // I'm a parent (represents command interpreter, excutes wait)
    wait( NULL );
    cout << "commands completed" << endl;
  }
}
