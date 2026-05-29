#include <unistd.h> // sbrk( )
#include <limits.h> // LONG_MAX

using namespace std;

static bool initialized = false;
static void *heap_top; // the beginning of the heap space
static void *heap_end; // the current boundary of the heap space, obtained from sbrk( 0 )

class MCB { // memory control block
public:
  int available; // true(1): this memory partition is available, false(0) unavailalbe.
  int size;      // MCB size + the user data size
};

void free_( void *dealloc_space ) {
  MCB *mcb;

  // locate this partition's mcb address from dealloc_space
  // Task 4: implement by yourself (just in one line).
  mcb->available = true;
  return;
}

/* The following will search from the top to bottom of the heap until it finds an empty partition. */
void *malloc_f( long size ) {
  struct MCB *cur_mcb;    // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user

  if( !initialized )   {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk( 0 );
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof( MCB );

  /* Announce a variable for the current address. Look at its MCB.
   * It will have a flag telling you if it's available. If unavailable,
   * move forward by the size of the MCB + its listed size.
   * To hold all 4 billion possible addresses, cast as (unsigned long long int)
   */
  for (void* cur = heap_top;
    cur < heap_end;
    cur = (void*)((unsigned long long int)cur + cur_mcb->size)) {
    // let cur_mcb point to each mcb you are scanning
    cur_mcb = (MCB*)cur;
    // if cur_mcb->available and cur_mcb->size fits size
    if (cur_mcb->available &&
      cur_mcb->size >= size) {
      // set aside this MCB for new_space
      cur_mcb->available = false;
      new_space = cur;
      break;
    }
  }

  // no space found yet
  if ( new_space == NULL ) {
    // get a space from OS
    sbrk(size);
    // old boundary now becomes new_space
    new_space = heap_end;
    // heap_end will go down by size: this is represented by ADDING to the value
    heap_end = (void*)((long long int)heap_end + size);
    // initialize cur_mcb with new_space and size.
    cur_mcb = (MCB*)new_space;
    // move down the heap boundary
    cur_mcb->available = 0;
    cur_mcb->size = size;
  }

  // new space is after new MCB
  return (void *)( ( long long int )new_space + sizeof( MCB ) );
}

/* The following will scan the entire heap. It has more variables
 * than malloc_f because it needs to track which MCB is the smallest
 * possible size for the process. Otherwise it behaves the same.
 */
void *malloc_b( long size ) {
  struct MCB *cur_mcb;    // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user

  if( !initialized )   {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk( 0 );
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof( MCB );

  // declare variables to handle the 'best fit' logic
  long best_fit_yet = LONG_MAX;
  struct MCB* best_mcb_yet = NULL;

  // scan each mcb from the top to the bottom of the heap (same as malloc_f)
  for (void* cur = heap_top;
    cur < heap_end;
    cur = (void*)((unsigned long long int)cur + cur_mcb->size)) {
      // let cur_mcb point to each mcb you are scanning
      cur_mcb = (MCB*)cur;
      // if cur_mcb->available and cur_mcb->size fits size and cur_mcb->size is the best size so far
      if (cur_mcb->available &&
        cur_mcb->size >= size &&
        cur_mcb->size <= best_fit_yet) {
        // temporarily memorize this best size so far and this best mcb so far
        best_fit_yet = cur_mcb->size;
        best_mcb_yet = cur_mcb;
      }
  }

  // After scan, check the best mcb so far. If it is not null
  if (best_mcb_yet != NULL) {
    // new_space points to this best mcb so 
    best_mcb_yet->available = false;
    new_space = (void*)best_mcb_yet;
  }

  // no space found yet
  if ( new_space == NULL ) {
    // same as malloc_f
    sbrk(size);
    new_space = heap_end;
    heap_end = (void*)((long long int)heap_end + size);

    cur_mcb = (MCB*)new_space;
    cur_mcb->available = 0;
    cur_mcb->size = size;
  }

  // new space is after new MCB
  return (void *)( ( long long int )new_space + sizeof( MCB ) );
}