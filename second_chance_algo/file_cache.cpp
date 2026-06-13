#include "file_cache.h"

#include <sys/stat.h> // open, fstat
#include <fcntl.h>    // open, fstat
#include <iostream>   // cerr, endl
#include <unistd.h>   // sysconf, close, fstat
#include <sys/mman.h> // mmap, mumap, msync
#include <stdio.h>    // fprintf
#include <string.h>   // memcpy
#include <strings.h>  // bzero

FileCache::FileCache( ) {
  init( DEFAULT_FILENAME, DEFAULT_NLINES ); // test.txt and 10 cache lines
}

FileCache::FileCache( string filename, int nLines ) {
  // receives a file name to open and defines # cache lines.
  init( ( filename.empty( ) ) ? "file" : filename, ( nLines <= 0 ) ? DEFAULT_NLINES : nLines );
}

FileCache::~FileCache( ) {
  flush( );
  delete entries;
  if ( fd > 2 )
    close( fd );
}

void FileCache::init( string filename, int nLines ) {
  // open a given file in a read/write mode. if it doesn't exist, create a file.
  this->filename = filename.c_str( );
  fd = open( this->filename, O_RDWR | O_CREAT, (mode_t)0600 );
  if ( fd == -1) {
    cerr << filename << ": openning failed" << endl;
    exit( -1 );
  }

  // instantiate an entry object that serves as cache attributes
  this->nLines = nLines;
  entries = new Entry[nLines];
  victim = nLines - 1;
  page_len = sysconf( _SC_PAGESIZE ); // compute a memory page size.
  cerr << "page length: " << page_len << endl;

  struct stat file_info;
  if ( fstat( fd, &file_info ) == -1 ) {
    cerr << filename << ": error calling fstat" << endl;
    exit( -1 );
  }
  file_len = file_info.st_size; // detect the given file's size.
  cerr << "file length: " << file_len << endl;

  // now create a disk cache
  lines = new char *[nLines];
  for ( int i = 0; i < nLines; i++ )
    lines[i] = NULL;  
}

int FileCache::findFreeEntry( ) {
  // simply scan entries[] from top to down to search for an available entry.
  for (int i = 0; i < nLines; i++) {
    if (entries[i].pNumber == -1)
      return i;
  }
  
  return -1;
}

int FileCache::nextVictim( ) {
  // Use the second-chance algorithm to find next victim //
  while (true) {
    // circular buffer requires modulo
    victim = (victim + 1) % nLines;
    if (entries[victim].reference == false)
      return victim;
    entries[victim].reference = false;
  }
}

void FileCache::writeBack( int victimEntry ) {

  if (entries[victimEntry].pNumber != -1) {
    if (entries[victimEntry].dirty == true) {
      // use msync() to skip kernel's cache
      if ( msync( lines[victimEntry], page_len, MS_SYNC ) == -1 )
	cerr << "msync failed" << endl;
      entries[victimEntry].dirty = false;
    }
    // regardless of cleanness, to reuse the same address, release this page
    if ( munmap( lines[victimEntry], page_len ) == -1 )
	cerr << "munmap failed" << endl;
  }
}


void FileCache::sync( ) {
  for ( int i = 0; i < nLines; i++ )
    writeBack( i );
}

void FileCache::flush( ) {
  for ( int i = 0; i < nLines; i++ ) {
    writeBack( i );
    entries[i].reference = false;
    entries[i].pNumber = -1;

    if ( lines[i] != NULL ) {
      if ( munmap( lines[i], page_len ) == -1 )
	fprintf( stderr, "%x: munmap failed\n", lines[i] );
    }
  }    
}

int FileCache::getNPages( ) {
  // return the total number of pages that fit a given file
  return file_len / page_len;
}

char *FileCache::readPage( int pNumber ) {
  // check pNumber is within the file length
  if ( pNumber > getNPages( ) ) {
    cerr << pNumber << " is not within file length" << endl;
    return NULL;
  }

  // locate a valid page
  for ( int i = 0; i < nLines; i++ ) {
    if ( entries[i].pNumber == pNumber ) {
      entries[i].reference = true;
      return lines[i];
    }
  }

  // if page miss happens
  // find an invalid page
  int victimEntry;

  // if all lines are full
  if ( ( victimEntry = findFreeEntry( ) ) == -1 ) {
    // seek for a victim
    victimEntry = nextVictim( );
  }

  // write back a dirty copy
  writeBack( victimEntry );

  // read a requested page from disk
  lines[victimEntry] = (char*)mmap( lines[victimEntry], page_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, pNumber * page_len );

  // cache it and update the entry.
  entries[victimEntry].pNumber = pNumber;
  entries[victimEntry].reference = true;

  // return the corresponding line, say line[i] or line[victim]
  return lines[victimEntry];
}

bool FileCache::writePage( int pNumber, char contents[] ) {
  // check pNumber is within the file length and contents have some characters
  if ( pNumber >  getNPages( ) || contents == NULL )
    return false;
  int actualSize = (strlen(contents) > page_len )? page_len : strlen(contents);

  // locate a valid page
  for ( int i = 0; i < nLines; i++ ) {
    if ( entries[i].pNumber == pNumber ) {
      // copy contents[] to thte corresponding line[i].
      memcpy( lines[i], contents, actualSize );
      entries[i].reference = true;
      entries[i].dirty = true;
      return true;
    }
  }

  // if page miss happens
  int victimEntry

  // find an invalid page
  // if all lines are full.
  if ( ( victimEntry = findFreeEntry( ) ) == -1 ) {
    // seek for a victim
    victimEntry = nextVictim( );
  }

  // write back a dirty copy
  writeBack( victimEntry );

  // read a requested page from disk
  lines[victimEntry] = (char*)mmap( lines[victimEntry], page_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, pNumber * page_len );

  // copy contents[] to lines[victim].
  memcpy( lines[victimEntry], contents, actualSize );

  // update the corresponding entry's attributes
  entries[victimEntry].pNumber = pNumber;
  entries[victimEntry].reference = true;
  entries[victimEntry].dirty = true;    

  return true;
}


