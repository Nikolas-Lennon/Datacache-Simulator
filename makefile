# makefile for DOT project
# Nikolas Lennon
# Project 3 DOT project
# 10/20/17
#

LIB	= /home/courses/cop3330p/LIB
CC	= g++ -std=c++11 -I. -I$(LIB)/cpp -Wall -Wextra

all:	stester.x vtester.x

stester.x: stester.o shapes.o
	$(CC) -o stester.x stester.o shapes.o

vtester.x: vtester.o vehicles.o shapes.o
	$(CC) -o vtester.x vtester.o vehicles.o shapes.o

stester.o: stester.cpp
	$(CC) -c stester.cpp

vtester.o: vtester.cpp
	$(CC) -c vtester.cpp

shapes.o: shapes.cpp
	$(CC) -c shapes.cpp

vehicles.o: vehicles.cpp
	$(CC) -c vehicles.cpp
