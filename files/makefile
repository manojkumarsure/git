CC = gcc
SH =bash
all:musiclib
musiclib: new.o \
	   	  Queue.o
	$(CC) -g -o $@ $^ `pkg-config --libs --cflags gtk+-2.0` -lmp

new.o:music_player2.c \
			Queue.h
	$(CC) -g -o $@ -c $< `pkg-config --libs --cflags gtk+-2.0` -lmp

Queue.o: Queue.c \
	   Queue.h
	$(CC) -g  -o $@ -c $<

clean :
		rm -rf *.o *.out *.txt
