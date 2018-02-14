# Makefile with suffix rules

CC	= gcc
CFLAGS	= -g
TARGET1 = master
TARGET2 = consumer
OBJS1 	= master.o
OBJS2 	= consumer.o
LIBOBJS =
LIBS =
MYLIBS =


.SUFFIXES: .c .o


all: $(TARGET1) $(TARGET2)

master: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $@

consumer: $(OBJS2)
	  $(CC) $(CFLAGS) $(OBJS2) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o *~ $(TARGET1) $(TARGET2)

