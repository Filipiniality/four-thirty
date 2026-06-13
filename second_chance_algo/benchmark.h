#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h> // srand
#include <string>   // string
#include <time.h>   // clock

#include "file_cache.h"
#include "bare_file.h"

using namespace std;

class Benchmark {
public:
  Benchmark( string filename, bool enabled, int nlines, int testcas );
private:
  void startTimer( );
  void endTimer( );
  void getPerformance( string msg );
  char *read( int pNumber, char buffer[] );
  void write( int pNumber, char buffer[] );
  void basicRW( string test, int access_pattern[] );
  void randomAccess( );
  void localizedAccess( );
  void mixedAccess( );

  bool enabled;
  clock_t start;
  clock_t end;
  unsigned int seed = 0;

  FileCache *filecache;
  BareFile *barefile;
  int nLines; // # cache lines
  int nPages; // # pages of a filename

  char wbytes[4096];
  char rbytes[4096];
};
#endif // BENCHMARK_H
