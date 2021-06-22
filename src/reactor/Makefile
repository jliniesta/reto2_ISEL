CC=gcc
CFLAGS=-g -Wall -O -I. -DNDEBUG
LDFLAGS=-L.
LDLIBS=-lwiringPi -pthread -lrt

all: libwiringPi.a main

main: main.o light.o alarm.o code.o fsm.o kbd.o ttyraw.o timer.o reactor.o

clean:
	$(RM) *.o *~ main libwiringPi.a

libwiringPi.a: wiringPi.o
	ar rcs $@ $^