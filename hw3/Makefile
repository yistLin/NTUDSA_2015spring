# This is the Makefile for DSA HW3 program part
# Author: b03902048
all: 3_4_1.out 3_4_2.out
CC = g++
INSTDIR = /usr/local/bin
CFLAGS = -O2 -std=c++11 -Wall

3_4_1.out: 3_4_1.cpp
	$(CC) $(CFLAGS) -o 3_4_1.out 3_4_1.cpp

3_4_2.out: 3_4_2.cpp
	$(CC) $(CFLAGS) -o 3_4_2.out 3_4_2.cpp

clean:
	rm 3_4_1.out 3_4_2.out

run3_4_1:
	./3_4_1.out < input > output

run3_4_2:
	./3_4_2.out < input2
