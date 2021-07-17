CC = gcc
CFLAGS = -Wall
PROG = buffer

MAKE_DIR = $(PWD)
SRCS = buffer.c
OBJS = buffer.o


all : do_precomp target clean
.PHONY: all

test : debug_compile
.PHONY: test

debug_compile : CFLAGS += -DDEBUG
debug_compile : do_precomp target clean

do_precomp:
	@echo $(INCLUDE_PATHS)
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE_PATHS)

target:
	ar rcs $(PROG).a $(OBJS)

clean:
	rm *.o