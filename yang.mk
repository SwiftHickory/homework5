# Makefile for yang.cpp

CC = g++

# Flags to the compiler

CFLAGS = 

all: yang

yang: yang.cpp
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -rf yang
