CC = gcc
CFLAGS = -Wall
PROG = render

MAKE_DIR = $(PWD)
SRCS = render.c 
OBJS = render.o common/shader/shader.a common/buffer/buffer.a ui/asdf.a


all : do_precomp target clean
.PHONY: all

test : debug_compile
.PHONY: test

debug_compile : CFLAGS += -DDEBUG
debug_compile : do_precomp target clean

do_precomp:
	@echo $(INCLUDE_PATHS)
	@$(MAKE) -C common/shader
	@$(MAKE) -C common/buffer
	@$(MAKE) -C ui
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE_PATHS)

target:
	ar rcs $(PROG).a $(OBJS)

clean:
	rm *.o