#!/bin/sh
g++ -c file_cache.cpp
g++ -c bare_file.cpp
g++ -c benchmark.cpp
 g++ driver.cpp file_cache.o bare_file.o benchmark.o -o driver

