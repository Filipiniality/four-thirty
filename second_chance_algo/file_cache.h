#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include <string> // filename
#include "entry.h"

#define DEFAULT_FILENAME "test.txt\0"
#define DEFAULT_NLINES 10

using namespace std;

class FileCache {
public:
  FileCache( );
  FileCache( string filename, int nLines );
  ~FileCache( );
  char *readPage( int pNumber );
  bool writePage( int pNumber, char contents[] );
  void sync( );
  void flush( );
  int getNPages( );     // return the total # of pages that fit a given file.
private:
  const char* filename; // a file name to read and write
  int fd;               // a file descriptor to access this file
  int nLines;           // the number of cache lines
  int victim;           // a victim cache line
  off_t page_len;       // the system memory page length: 4096 bytes
  long file_len;        // the size of a file in bytes
  Entry *entries;       // an array of disk cache entries
  char **lines;         // actuall disk cache lines

  void init( string filename, int nLines ); // open a file
  int findFreeEntry( );
  int nextVictim( );
  void writeBack( int victimEntry );
};
#endif  // FILE_CACHE_H
