#include <pthread.h> // pthread
#include <stdio.h>
#include <stdlib.h>   // atoi( )
#include <unistd.h>   // sleep( )
#include <sys/time.h> // gettimeofday
#include <iostream>   // cout

#define PHILOSOPHERS 5
#define MEALS 3

using namespace std;

// table where up to 2 philosophers can eat simultaneously
class Table2 {
public:
  Table2( ) {
      // initialize with mutex
      pthread_mutex_init(&lock, NULL);
      // initialize every philosopher
      for (int i = 0; i < PHILOSOPHERS; i++) {
          pthread_cond_init(&self[i], NULL);
      }
      for (int i = 0; i < PHILOSOPHERS; i++) {
        state[i] = THINKING;
      }
  }
  void pickup( int i ) {
      // critical section start
      pthread_mutex_lock(&lock);
      // set state
      state[i] = HUNGRY;
      test(i);
      if (state[i] != EATING) {
          pthread_cond_wait(&self[i], &lock);
      }
      cout << "philosopher[" << i << "] picked up chopsticks" << endl;
      pthread_mutex_unlock(&lock);
      // critical section end
  }

  void putdown( int i ) {
      // crtical section start
      pthread_mutex_lock(&lock);
      state[i] = THINKING;
      test((i + 4) % PHILOSOPHERS);
      test((i + 1) % PHILOSOPHERS);
      cout << "philosopher[" << i << "] put down chopsticks" << endl;
      pthread_mutex_unlock(&lock);
      // critical section end
  }

private:
  enum { THINKING, HUNGRY, EATING } state[PHILOSOPHERS];
  pthread_mutex_t lock;
  pthread_cond_t self[PHILOSOPHERS];

  void test( int i ) {
      // look at left and right sticks
      if ((state[(i + 4) % PHILOSOPHERS] != EATING) &&
        (state[i] == HUNGRY) &&
        (state[(i + 1) % PHILOSOPHERS] != EATING)) {
          // set state
          state[i] = EATING;
          // signal to self
          pthread_cond_signal(&self[i]);
      }
  }
};

// Table of people who lock down all the chopsticks when one person is using them
class Table1 {
public:
  Table1( ) {
      // initialize the mutex lock
      pthread_mutex_init(&lock, NULL);
  }
  void pickup( int i ) {
      // lock by yourself
      pthread_mutex_lock(&lock);
      cout << "philosopher[" << i << "] picked up chopsticks" << endl;
  }

  void putdown( int i ) {
      cout << "philosopher[" << i << "] put down chopsticks" << endl;
      // unlock by yourself
      pthread_mutex_unlock(&lock);
  }

private:
    // define a mutex lock
    pthread_mutex_t lock;
};

// every below gets submitted as is
class Table0 {
public:
  void pickup( int i ) {
    cout << "philosopher[" << i << "] picked up chopsticks" << endl;
  }

  void putdown( int i ) {
    cout << "philosopher[" << i << "] put down chopsticks" << endl;
  }
};

static Table2 table2;
static Table1 table1;
static Table0 table0;

static int table_id = 0;

void *philosopher( void *arg ) {
  int id = *(int *)arg;
  
  for ( int i = 0; i < MEALS; i++ ) {
    switch( table_id ) {
    case 0:
      table0.pickup( id );
      sleep( 1 );
      table0.putdown( id );
      break;
    case 1:
      table1.pickup( id );
      sleep( 1 );
      table1.putdown( id );
      break;
    case 2:
      table2.pickup( id );
      sleep( 1 );
      table2.putdown( id );
    break;
    }
  }
  return NULL;
}

int main( int argc, char** argv ) {
  pthread_t threads[PHILOSOPHERS];
  pthread_attr_t attr;
  int id[PHILOSOPHERS];
  table_id = atoi( argv[1] );

  pthread_attr_init(&attr);
  
  struct timeval start_time, end_time;
  gettimeofday( &start_time, NULL );
  for ( int i = 0; i < PHILOSOPHERS; i++ ) {
    id[i] = i;
    pthread_create( &threads[i], &attr, philosopher, (void *)&id[i] );
  }

  for ( int i = 0; i < PHILOSOPHERS; i++ )
    pthread_join( threads[i], NULL );
  gettimeofday( &end_time, NULL );

  sleep( 1 );
  cout << "time = " << ( end_time.tv_sec - start_time.tv_sec ) * 1000000 + ( end_time.tv_usec - start_time.tv_usec ) << endl;

  return 0;
}
