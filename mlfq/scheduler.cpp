#include <signal.h>   // kill
#include <unistd.h>   // usleep
#include <iostream>   // cout, cerr, endl
#include "scheduler.h"

Scheduler::Scheduler( int quantum ) {
  msec = ( quantum > 0 ) ? quantum : DEFAULT_QUANTUM;
}

bool Scheduler::addProcess( int pid ) {
  if ( kill( pid, SIGSTOP ) != 0 ) {
    cerr << "process[" << pid << "] can't be paused." << endl;
    return false;
  }
  queue[0].push( pid ); // enqueue this process
  return true;
}

void Scheduler::schedulerSleep( ) {
  usleep( msec );
  cerr << "scheduler: completed " << (++nQuantums) << " quantums" << endl;
}

void Scheduler::run_rr( ) {
  cerr << "scheduler (round robin): started" << endl;
  int current = 0;

  while ( true ) {
    if ( queue[0].size( ) == 0 )         // no more processes to terminate scheduler
      break;
    current = queue[0].front( );         // pick up the first process from the queue

    if ( kill( current, 0 ) == 0 ) {      // current process is alive
      cerr << "\nscheduler: resumed " << current << endl;
      kill( current, SIGCONT );           // run it for a next quantum
      schedulerSleep( );
      kill( current, SIGSTOP );
    }

    queue[0].pop( );
    if ( kill( current, 0 ) == 0 ) {     // current process is still alive
      queue[0].push( current );
    }
    else { // current process is dead
      cerr << "scheduler: confirmed " << current << "'s termination" << endl;
      current = 0; 
    }
  }
  cerr << "scheduler: has no more process to run" << endl;
}

void Scheduler::run_mfq( ) {
  cerr << "scheduler (multilevel feedback queue): started" << endl;
  int current = 0;  // current pid
  int previous = 0; // previous pid
  
  int slices[3];                          // slices[i] means that level i's current time slice.
  for ( int i = 0; i < 3; i++ )
    slices[i] = 0;                        // all levels start slice 0.

  while ( true ) {
    int level = 0;
    for ( ; level < 3; level++ ) {
      // if the level you are on has slice 0.
      if (slices[level] == 0) {
        // check this queue is empty.
        if (queue[level].size() == 0) {
          // if so, go to a next lower level queue.
          continue;
        }
        // otherwise, pick up the oldest pid from queue of level you are currently on
        current = queue[level].front();
        break;
      }
      // if the current level's slice is 1, 2, or 3, previous process was interrupted, and should now be run again
      else {
        current = previous;
        break;
      }
    }
    // if we reached level 3, (i.e., the lowest level) and found no processes to schedule
    if (level == 3 && current == 0) {
      // end scheduler object
      break;
    }

    // check if a process to run is still active.
    if (kill(current, 0) == 0) {
      cerr << "\nscheduler: resumed " << current << endl;
      kill(current, SIGCONT);
      schedulerSleep();
      kill(current, SIGSTOP);
      // reflect quantum usage by incrementing time slice
      slices[level]++;
    }

    /* 
    * Next block contains logic for moving to lower queues
    * queue[0] has threshold 1 : 2^0 = 1
    * queue[1] has threshold 2 : 2^1 = 2
    * queue[2] has threshold 4 : 2^2 = 4
    * generalized: 2^level = threshold
    */

    // check if this process is still active.
    if (kill(current, 0) == 0) {
      if (slices[level] >= pow(2, level)) {
        // next slice wrapped back to 0
        slices[level] = 0;
        // if in queue 0 or 1
        if (level < 2) {
          // go to the next level queue
          queue[level].pop();
          queue[level++].push(current);
        }
        else {
          // go back to the lowest level queue
          queue[level].pop();
          queue[level].push(current);
        }
      }
    }
    // process inactive
    else {
      slices[level] = 0;
      queue[level].pop();
      // current process is dead, print out:
      cerr << "scheduler: confirmed " << current << "'s termination" << endl;
    }
    // inverse of line 77
    previous = current;
  }
  cerr << "scheduler: has no more process to run" << endl;
}