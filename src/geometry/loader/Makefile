CC = gcc
CFLAGS = -Wall
PROG = loader

MAKE_DIR = $(PWD)
SRCS = obj.c
OBJS = obj.o


all : do_precomp target clean
.PHONY: all

test : debug_compile
.PHONY: test

debug_compile : CFLAGS += -DDEBUG
debug_compile : do_precomp target clean

do_precomp:
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE_PATHS)

target:
	ar rcs $(PROG).a $(OBJS)

clean:
	rm *.o