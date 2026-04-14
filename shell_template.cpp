#include <stdio.h>                 // printf, scanf                                                       
#include <string.h>                // bzero, strcmp                                                       
#include <stdlib.h>                // exit                                                                
#include <unistd.h>                // fork, execlp                                                        
#include <sys/types.h>             // wait                                                                
#include <sys/wait.h>              // wait
#include <iostream>                // cout, cint
#include <string>                  // c++ string, getline
#include <cstring>                 // strtok
#include <stack>                   // c++ stack

#define MAX_TOKENS 100             // the max # of tokens

using namespace std;

int main( ) {
  pid_t pid = 0;                    // a child process id to be returned from fork( )
  stack<pid_t> pid_stack;           // a pid stack of children in background
  string fullLine;                  // stores an fullLine from the keyboard.
  int status;                       // receives a termination status from a child
  char *token[MAX_TOKENS];          // tokenize fullLine
  bool background = false;          // true if a command is given with &

  while( true ) {
    printf( "$ " );                 // print out a shell prompt
    getline( cin,  fullLine );      // read one line
    if ( fullLine.empty( ) )        // go back to the top of while
      continue;

    // set background true if the last character is '&'.
    // if the last character is '&' or ';', replace it with a space ' '.
    
    const char* delimiter = " ";
    // tokenize fullLine with a space.
    token[0] = strtok( (char *)fullLine.c_str( ), delimiter); 
    for ( int i = 0; token[i] != NULL && i < 80; i++ )
      token[i + 1] = strtok(NULL, delimiter);

    // token[0] is a command.
    // check if it's "exit".
    // check if it's "fg" in which case wait for the pid on the stack top. then, pop it out.
	 
    // fork a child shell                                                 
    // child should replace it with token[0] as possing the entire token.
    // parent should wait for the child if !background.
    // otherwise put the child pid into pid_stack.
  }
  return 0;
}
