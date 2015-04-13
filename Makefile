# This is the Makefile for DSA HW2 program part
# Author: b03902048
all: demo
CC = g++
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAGS = -g -Wall -ansi -O3

demo: main.o mymethod.o
	$(CC) -o demo main.o mymethod.o
main.o: main.cpp mymethod.h
	$(CC) -l$(INCLUDE) $(CFLAGS) -c main.cpp
mymethod.o: mymethod.h mymethod.cpp
	$(CC) -l$(INCLUDE) $(CFLAGS) -c mymethod.cpp

clean:
	rm main.o mymethod.o demo
