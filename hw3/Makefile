# This is the Makefile for DSA HW3 program part
# Author: b03902048
all: 3_4_1.out 3_4_2.out
CC = g++
INSTDIR = /usr/local/bin
CFLAGS = -O2 -std=c++11 -Wall

3_4_1.out: integer_calculator.cpp
	$(CC) $(CFLAGS) -o 3_4_1.out integer_calculator.cpp

3_4_2.out: floatpoint_calculator.cpp
	$(CC) $(CFLAGS) -o 3_4_2.out floatpoint_calculator.cpp

clean:
	rm 3_4_1.out 3_4_2.out

run3_4_1:
	./3_4_1.out

run3_4_2:
	./3_4_2.out
