#include "bounded_buffer_os.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
BoundedBufferOS::BoundedBufferOS( int bufsize ) {
  if ( bufsize <= 0 )
    bufsize = BUFSIZE;
  buf = new int[bufsize];
  n = bufsize;            /* Buffer holds max of n items */
  front = rear = 0;       /* Empty buffer iff front == rear */
  sem_init(&mutex, 0, 1); /* Binary semaphore for locking */
  sem_init(&full, 0, n);  /* Initially, buf has n available slots */
  sem_init(&empty, 0, 0); /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
BoundedBufferOS::~BoundedBufferOS( ) {
  delete buf;
}

/* Insert item onto the rear of shared buffer sp */
void BoundedBufferOS::insert( int item ) {
    // check if there's no space
    sem_wait(&full);
    // critical section
    sem_wait(&mutex);
    // add next produced to buffer and grow buffer size by 1
    buf[++rear % n] = item;
    sem_post(&mutex);
    sem_post(&empty);
 }

/* Remove and return the first item from buffer sp */
int BoundedBufferOS::remove( ) {
    int item;

    // check if there's no data
    sem_wait(&empty);
    // critical section
    sem_wait(&mutex);
    // pop first item from buffer and shrink buffer size by 1
    item = buf[++front % n];
    sem_post(&mutex);
    sem_post(&full);

    return item;
 }

