#include "benchmark.h"
#include <iostream>

int main( int argc, char* argv[] ) {
  if ( argc != 5 ) {
    cerr << "usage: ./driver filename e nlines testcase\n"
	 << "       where e = enabled or other = disabled\n"
	 << "             nlines = #lines in cache (>=10)\n"
         << "             testcase = 1: random, 2: local, 3: mixed"
	 << endl;
    exit( -1 );
  }
  char *filename = argv[1];
  bool enabled = ( argv[2][0] == 'e' || argv[2][0] == 'E' ) ? true : false;
  int nlines = atoi( argv[3] );
  if ( nlines < 10 ) {
    cerr << "nlines must be 10 or larger" << endl;
    exit( -1 );
  }
  int testcase = atoi( argv[4] );
  if ( testcase < 1 || testcase > 4 ) {
    cerr << "testcast must be 1, 2, 3, or 4" << endl;
    exit( -1 );
  }

  Benchmark *benchmark = new Benchmark( filename, enabled, nlines, testcase );
}
