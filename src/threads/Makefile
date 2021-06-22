CC=gcc
CFLAGS=-g -Wall -O2 $(DEBUG) -Iinclude
LDFLAGS=-L.
LDLIBS=-lwiringPi -pthread -lrt

all: libwiringPi.a main

main: main.o light.o alarm.o code.o fsm.o timer.o kbd.o ttyraw.o task.o

clean:
	$(RM) *.o *~ main libwiringPi.a

libwiringPi.a: wiringPi.o
	ar rcs $@ $^