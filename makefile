CC = gcc
CFLAGS = -g -c -Wall

all: warmup1

warmup1: warmup1.o my402list.o
	$(CC) warmup1.o my402list.o -o warmup1

warmup1.o: warmup1.c
	$(CC) $(CFLAGS) warmup1.c

my402list.o: my402list.c
	$(CC) $(CFLAGS) my402list.c

clean:
	rm -rf *.o warmup1



