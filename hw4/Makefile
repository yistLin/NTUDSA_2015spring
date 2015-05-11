# This is the Makefile for DSA HW4 program part
# Author: b03902048
target1 = tree
target2 = forest
CC = g++
INSTDIR = /usr/local/bin
CFLAGS = -O2 -Wall

all: $(target1) $(target2)

$(target1): 4-2-2.cpp
	$(CC) $(CFLAGS) -o tree 4-2-2.cpp

$(target2): 4-2-6.cpp
	$(CC) $(CFLAGS) -o forest 4-2-6.cpp

clean:
	rm -rf tree forest

