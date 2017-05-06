#COBJS = $(wildcard *.c)
#OBJS = $(patsubst %.c,%.o,$(COBJS))
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = -std=c99 -Wall -Werror --pedantic -I../../include
LIBNAME = libmy.a
LIBPATH = ../../lib
CC = gcc
all: $(OBJS)
	ar -rc $(LIBNAME) $(OBJS)
	ranlib $(LIBNAME)
	mv $(LIBNAME) $(LIBPATH)
clean:
	rm -f $(OBJS)	
fclean:
	rm -f $(LIBPATH)/$(LIBNAME) $(OBJS)
re:
	make fclean all 
