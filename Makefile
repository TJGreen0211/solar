CC = gcc
CFLAGS = -Wall -Wno-unused-function
PROG = solar

FILE_NAME = $(PROG)
BIN_DIR = bin
MAKE_DIR = $(PWD)
INCLUDE_DIR = $(MAKE_DIR)/include
SRCS = src/main.c
OBJS = main.o

INCLUDE_PATHS = -I $(INCLUDE_DIR) -I /usr/include/python3.9
LIBRARY_PATHS = -L $(MAKE_DIR) -lglfw -lGL -lm -lrt -lasound -lpthread -lpython3.9 -lGLU

all : compile
.PHONY: all

test : debug_compile
.PHONY: test

debug_compile : CFLAGS += -DDEBUG
debug_compile : compile clean

compile:
	@echo $(MAKE_DIR) 
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(INCLUDE_PATHS) $(LIBRARY_PATHS)

clean:
	cp solar bin/solar
	rm -f $(OBJS)