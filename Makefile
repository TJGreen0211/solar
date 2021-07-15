CC = gcc
CFLAGS = -Wall -Wno-unused-function
PROG = solar

FILE_NAME = $(PROG)
BIN_DIR = bin
MAKE_DIR = $(PWD)
INCLUDE_DIR = $(MAKE_DIR)/include
SRCS = src/main.c \
	include/glad/glad.c \
	src/graphics/render.a \
	src/utility/matrixmath/matrixmath.a
OBJS = main.o

INCLUDE_PATHS = -I $(INCLUDE_DIR) -I /usr/include/python3.9
LIBRARY_PATHS = -L $(MAKE_DIR) -lglfw -lGL -lm -lrt -lasound -lpthread -lpython3.9

export INCLUDE_PATHS
export LIBRARY_PATHS

all : build_libs compile
.PHONY: all

test : debug_compile
.PHONY: test

debug_compile : CFLAGS += -DDEBUG
debug_compile : compile clean

build_libs:
	@$(MAKE) -C src/graphics
	@$(MAKE) -C src/utility/matrixmath

compile:
	@echo $(MAKE_DIR) 
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(INCLUDE_PATHS) $(LIBRARY_PATHS)

clean:
	cp solar bin/solar
	rm -f $(OBJS)