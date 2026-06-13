#ifndef BARE_FILE_H
#define BARE_FILE_H

#include <string> // filename

#define DEFAULT_FILENAME "test.txt\0"

using namespace std;

class BareFile {
 public:
  BareFile( );
  BareFile( string filename );
  ~BareFile( );
  int readPage( int pNumber, char buffer[] );
  bool writePage( int pNumber, char contents[] );
  int getNPages( );
 private:
  const char* filename;
  int fd;
  off_t page_len;
  long file_len;

  void init( string filename );
};
#endif // BARE_FILE_H
