#include "benchmark.h"

#include <iostream> // cerr

Benchmark::Benchmark( string filename, bool enabled, int nlines, int testcase ) {
  this->enabled = enabled;
  this->nLines = nlines;
  if ( enabled ) {
    filecache = new FileCache( filename, nLines );
    nPages = filecache->getNPages( );
  }
  else {
    barefile = new BareFile( filename );
    nPages = barefile->getNPages( );
  }
 
  start = clock( );
  switch( testcase ) {
  case 1:
    randomAccess( );
    end = clock( );
    getPerformance( "random access" );
    break;
  case 2:
    localizedAccess( );
    end = clock( );
    getPerformance( "localized access" );
    break;
  case 3:
    mixedAccess( );
    end = clock( );
    getPerformance( "mixed access" );
    break;
  case 4:
    randomAccess( );
    end = clock( );
    getPerformance( "random access" );

    start = clock( );
    localizedAccess( );
    end = clock( );
    getPerformance( "localized access" );

    start = clock( );
    mixedAccess( );
    end = clock( );
    getPerformance( "mixed access" );
    break;
  }
}

void Benchmark::startTimer( ) {
  start = clock( );
}

void Benchmark::endTimer( ) {
  end = clock( );
}

void Benchmark::getPerformance( string msg ) {
  if ( enabled == true )
    printf ( "Test %s(cache enabled): %lf sec\n", msg.c_str( ), 
	     ( (double)(end - start) ) / CLOCKS_PER_SEC );
  else
    printf ( "Test %s(cache disabled): %lf sec\n", msg.c_str( ), 
	     ( (double)(end - start) ) / CLOCKS_PER_SEC );    
}

char *Benchmark::read( int pNumber, char buffer[] ) {
  
  if ( enabled == true )
    return filecache->readPage( pNumber );
  else {
    barefile->readPage( pNumber, buffer );
    return buffer;
  }
}

void Benchmark::write( int pNumber, char buffer[] ) {
  if ( enabled == true )
    filecache->writePage( pNumber, buffer );
  else
    barefile->writePage( pNumber, buffer );
}

void Benchmark::basicRW( string test, int access_pattern[] ) {
  for ( int i = 0; i < 200; i++ ) {
    for ( int j = 0; j < 4096; j++ )
      wbytes[j] = 40 + ( j % 50 );
    write( access_pattern[i], wbytes );
  }
  
  for ( int i = 0; i < 200; i++ ) {
    char* rbytes_return = read( access_pattern[i], rbytes );
    for ( int k = 0; k < 4096; k++ )
      if ( rbytes_return[k] != wbytes[k] ) {
	cerr << test << ": rbytes[" << k << "] = " << rbytes[k]
	     << "wbytes[" << k << "] = " << wbytes[k];
	exit( -1 );
      }
  }  
}

void Benchmark::randomAccess( ) {
  int access_pattern[200];

  for ( int i = 0; i < 200; i++ )
    access_pattern[i] = rand( ) % nPages;

  basicRW( "randomAccess", access_pattern );
}

void Benchmark::localizedAccess( ) {
  int access_pattern[200];

  for ( int i = 0; i < 200; i++ )
    access_pattern[i] = i % ( ( nPages < nLines ) ? nPages : nLines );

  basicRW( "localizedAccess", access_pattern );
}

void Benchmark::mixedAccess( ) {
  int access_pattern[200];

  for ( int i = 0; i < 200; i++ ) {
    if ( abs( rand( ) % 10 ) > 8 ) {
      access_pattern[i] = rand( ) % nPages;
    }
    else {
      access_pattern[i] = i % ( ( nPages < nLines ) ? nPages : nLines );
    }
  }

  basicRW( "mixeddAccess", access_pattern );  
}
