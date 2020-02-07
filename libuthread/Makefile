# Target library
lib := libuthread.a 

all: $(lib)
	ar rcs libuthread.a queue.o context.o uthread.o preempt.o

## TODO: Phase 1.1
targets := libuthread.a
objs    := queue.o uthread.o preempt.o context.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g
PANDOC  := pandoc

libuthread.a: $(objs)
	ar rcs libuthread.a  $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(targets) $(objs)
