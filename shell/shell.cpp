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
    if (fullLine.back()=='&')
    {
      background = true;
    }
    // if the last character is '&' or ';', replace it with a space ' '.
    if ((fullLine.back() == '&') || (fullLine.back() == ';'))
    {
      fullLine.back() = ' ';
    }
    
    const char* delimiter = " ";
    // tokenize fullLine with a space.
    token[0] = strtok( (char *)fullLine.c_str( ), delimiter); 
    for ( int i = 0; token[i] != NULL && i < 80; i++ )
      token[i + 1] = strtok(NULL, delimiter);
    // Ensure array is NULL-terminated
    
    // token[0] is a command.
    // check if it's "exit" and terminate shell
    if (strcmp(token[0], "exit") == 0)
    {
      exit(0);
    }
    
    // check if it's "fg" in which case wait for the pid on the stack top. then, pop it out.
	 if (strcmp(token[0], "fg") == 0)
    {
      waitpid(pid_stack.top(), &status, 0);
      pid_stack.pop();
      continue;
    }
    
    // fork a child shell
    pid = fork();
    if (pid < 0)
    {
      fprintf(stderr, "Fork failed.");
      exit(-1);
    }
    else if (pid == 0)
    {
      // child should replace it with token[0] as possing the entire token.
      execvp(token[0],token);
    }
    else
    {
      // parent should wait for the child if !background.
      if (!background)
      {
        waitpid(pid, &status, 0);
      }
      else
      {
        // otherwise put the child pid into pid_stack.
        pid_stack.push(pid);
        background = false;
        // reset the boolean so execution becomes synchronous
      }
    }
  }
  return 0;
}
