#COBJS = $(wildcard *.c)
#OBJS = $(patsubst %.c,%.o,$(COBJS))
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = -std=c99 -Wall -Werror --pedantic -I../../include
LIBNAME = liblist.a
LIBPATH = ../../lib
REQ_LIB = ../my
#listen, library merging is hard, this is easier
MOBJS = $(patsubst %.c,%.o,$(wildcard $(REQ_LIB)/*.c))
CC = gcc
all: $(OBJS)
	make re -C $(REQ_LIB)
	ar -rc $(LIBNAME) $(OBJS) $(MOBJS)
	ranlib $(LIBNAME)
	mv $(LIBNAME) $(LIBPATH)
clean:
	rm -f $(OBJS) $(MOBJS)
fclean:
	rm -f $(LIBPATH)/$(LIBNAME) $(OBJS) $(MOBJS)
re:
	make fclean all 
