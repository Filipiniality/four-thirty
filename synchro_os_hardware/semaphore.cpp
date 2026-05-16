#include "semaphore.h"

// initializes sem to a given value. The var named 'pshared' was chosen to mimic Linux
int Semaphore::init( int *sem, int pshared, int value ) {
  *sem = value;
  return 0;
}

int Semaphore::wait( int *sem ) {
	while (true) {
		// obtain lock via test-and-set. Spin-loop until lock is free.
		while (atomic_flag_test_and_set(&lock));
		// if semaphore is greater than 0, decrement
		if (*sem > 0) {
			*sem--;
			// unlock
			atomic_flag_clear(&lock);
			break;
		}
		else {
			// release
			atomic_flag_clear(&lock);
			// stay in while(true) to reset obtaining the lock
			continue;
		}
	}
  return 0;
}

int Semaphore::post( int *sem ) {
	// obtain the lock
	while (atomic_flag_test_and_set(&lock));
	// increments 'sem'
	*sem++;
	// releases the lock
	atomic_flag_clear(&lock);
	return 0;
}
