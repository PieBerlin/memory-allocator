flags=-O2 -Wall -std=c23 -no-pie
ldflags=-L/usr/local/lib -lpieutils

.PHONY: clean

all: clean memory-allocator

memory-allocator: memory-allocator.o heap.o 
	cc ${flags} $^ -o $@ ${ldflags}

memory-allocator.o: memory-allocator.c memory-allocator.h
	cc ${flags} -c $<
heap.o: heap.asm
	nasm -f elf64 $^
clean:
	rm -f *.o memory-allocator 
