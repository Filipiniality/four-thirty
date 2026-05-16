#!/bin/sh

rm *~
g++ -lpthread tas.cpp -o tas
g++ -c semaphore.cpp
g++ -c bounded_buffer_hw.cpp
g++ -c bounded_buffer_os.cpp
g++ -lpthread driver.cpp semaphore.o bounded_buffer_hw.o bounded_buffer_os.o -o driver
