
CC=gcc

RM=rm

CFLAGS=-c -Wall -O3 -std=c99

LDFLAGS=-lm

HEADERS=$(wildcard *.h)

SOURCES=$(wildcard *.c)

OBJECTS=$(SOURCES:.c=.o)

EXECS=$(SOURCES:%.c=%)

.PHONY: all 
all: run

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@ 

run: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	-@ $(RM) *.o 
