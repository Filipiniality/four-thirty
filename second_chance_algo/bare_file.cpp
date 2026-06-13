#include "bare_file.h"

#include <sys/stat.h> // open, fstat                                                                                               
#include <fcntl.h>    // open, fstat                                                                                               
#include <iostream>   // cerr, endl                                                                                                
#include <unistd.h>   // sysconf, close, fstat, lseek                                                                                     
#include <stdio.h>    // fprintf                                                                                                   
#include <string.h>   // memcpy                                                                                                    
#include <strings.h>  // bzero   

BareFile::BareFile( ) {
  init( DEFAULT_FILENAME );
}

BareFile::BareFile( string filename ) {
  init( ( filename.empty( ) ) ? "file" : filename );
}

void BareFile::init( string filename ) {
  this->filename = filename.c_str( );
  fd = open( this->filename, O_RDWR | O_CREAT, (mode_t)0600 );
  if ( fd == -1 ) {
    cerr << filename << ": openning failed" << endl;
    exit( -1 );
  }

  page_len = sysconf( _SC_PAGESIZE );
  cerr << "page length: " << page_len << endl;
  
  struct stat file_info;
  if ( fstat( fd, &file_info ) == -1 ) {
    cerr << filename << ": error calling fstat" << endl;
    exit( -1 );
  }
  file_len = file_info.st_size;
  cerr << "file length: " << file_len << endl;
}

BareFile::~BareFile( ) {
  if ( fd > 2 )
    close( fd );
}

int BareFile::getNPages( ) {
  return file_len / page_len;
}

int BareFile::readPage( int pNumber, char buffer[] ) {
  // check pNumber is within the file length
  if ( pNumber > file_len / page_len ) {
    cerr << pNumber << ": beyond max = " << getNPages( ) << endl;
    return 0;
  }
  lseek( fd, pNumber * page_len, SEEK_SET );
  return read( fd, buffer, page_len );
}

bool BareFile::writePage( int pNumber, char contents[] ) {
  // check pNumber is within the file length
  if ( pNumber > file_len / page_len ) {
    cerr << pNumber << ": beyond max = " << getNPages( ) << endl;
    return 0;
  }
  lseek( fd, pNumber * page_len, SEEK_SET );
  int actualSize = ( strlen( contents ) > page_len ) ? page_len : strlen( contents );
  return write( fd, contents, actualSize );  
}
