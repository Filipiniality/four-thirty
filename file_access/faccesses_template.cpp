#include <fcntl.h>    // open
#include <unistd.h>   // write, close
#include <iostream>   // cerr
#include <stdlib.h>   // atoi
#include <sys/time.h> // gettimeofday

#define NCHARS    26
#define FILEBLOCK 4096

using namespace std;

int filewrite( const char* filename, int nblocks, char* block ) {
  // open a file for writing.

  // start timer
  struct timeval start_time, end_time;  

  // repetitively write n times, (nblocks) of block (4096B)

  close( fd ); // flush all in-cache data to disk  

  // stop timer
  gettimeofday( &end_time, NULL );
  time_t elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) * 1000000 + ( end_time.tv_usec - start_time.tv_usec );
  cerr << "nblocks = " << nblocks << ", total time = " <<  elapsed_time << ", time / block = "<< elapsed_time / nblocks << endl;
  
  return 0;
}

int fileread( const char* filename, int nblocks, char* block ) {
  // open a file for reading.

  // start timer
  struct timeval start_time, end_time;

  // repetitively read n times, (nblocks) of block (4096B)

  // stop timer
  gettimeofday( &end_time, NULL );
  time_t elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) * 1000000 + ( end_time.tv_usec - start_time.tv_usec );
  cerr << "nblocks = " << nblocks << ", total time = " <<  elapsed_time << ", time / block = "<< elapsed_time / nblocks << endl;

  close( fd ); // read closes after the timer stops as there are no data remained in cache.
  
  return 0;
}

// returns the name of a file that includes i blocks.
string filename( int i ){
  string num = to_string( i );
  string filename_str = "f_" + num + ".txt";
  return filename_str;
}

int main( int argc, char* argv[] ) {

  // initialize block
  char block[FILEBLOCK];
  for ( int i = 0; i < FILEBLOCK; i++ )
    block[i] = 'a' + ( i % NCHARS );

  cout << "writing to direct blocks ********************************************" << endl;
  filewrite( filename( 1 ).c_str(), 1, block );
  filewrite( filename( 12 ).c_str(), 12, block );

  cout << "writing to 1st indirect blocks **************************************" << endl;
  // filewrite all the way to the 1st block of the 1st indirect blocks
  // filewrite all the way to the last block of the 1st indirect blocks

  cout << "writing to the 2ndary indirect blocks *******************************" << endl;
  // filewrite all the way to the 1st block of the secandary indirect blocks
  // filewrite all the way to the last block of the secandary indirect blocks
  
  cout << "writing to the 3tiary indirect blocks *******************************" << endl;
  // filewrite all the way to the 1st block of the tartiary indirect blocks

  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

  cout << "reading to direct blocks ********************************************" << endl;
  fileread( filename( 1 ).c_str(), 1, block );
  fileread( filename( 12 ).c_str(), 12, block );  

  cout << "reading to 1st indirect blocks **************************************" << endl;
  // filewrite all the way to the 1st block of the 1st indirect blocks
  // filewrite all the way to the last block of the 1st indirect blocks

  cout << "reading to the 2ndary indirect blocks *******************************" << endl;
  // filewrite all the way to the 1st block of the secandary indirect blocks
  // filewrite all the way to the last block of the secandary indirect blocks

  cout << "reading to the 3tiary indirect blocks *******************************" << endl;
  // filewrite all the way to the 1st block of the tartiary indirect blocks
}
