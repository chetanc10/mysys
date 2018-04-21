
CFLAGS ?= -O3

# gprof requires -pg cflag
CFLAGS += -pg

all:
	gcc $(CFLAGS) -Wall -lrt -pthread *.c

clean:
	rm -rf *.o a.out
