# Copyright Duca Andrei-Rares, Grupa 311CA, ACS-CTI 2022-2023

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# pickp all sources
SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

# define targets
TARGETS = image_editor

build: $(OBJS) $(TARGETS)

my_octave: main.c functions.c
	$(CC) $(CFLAGS) $@.o -o $@

pack:
	zip -FSr 311CA_DucaAndreiRares_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean
