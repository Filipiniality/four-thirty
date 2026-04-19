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
  else if ( pid == 0 ) {
    // I'm a child
    // This shell will load in wc

    if (pipe(fds[0] < 0) {
      perror( "pipe failed" );
    }
    //Pipe A now exists
    // point this stdin to grandchild's stdout
    // First fork
    if ((pid = fork()) < 0) { // error check
      perror( "fork failed" );
    }
    else if (pid == 0) {
    // Pipe A still exists
      // I'm a grand-child
      // This shell will execute grep

      // Another fork
      if (pipe(fds[1] < 0) {
        perror("pipe failed");
      }
      // Pipe B now exists
      if ((pid = fork()) < 0) { // error check
        perror(" fork failed");
      }
      // Close read-end of parent
      // Redirect with dup2
      // Close write-end of child
      if (pid == 0) {
        // Pipe B still exists
        // I'm a great-grand-child
        // this shell will implement ps

        // point this stdout to grandchild's stdin
      }
      else {
      }
    }
    else { // child
      // Pipe B still exists
    }
  }
  else {
    // I'm a parent (represents command interpreter)
    wait( NULL );
    cout << "commands completed" << endl;
  }
}
