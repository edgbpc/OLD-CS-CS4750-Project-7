# Makefile with suffix rules

CC	= gcc
CFLAGS	= -g
TARGET1 = master
TARGET2 = consumer
TARGET3 = producer
OBJS1 	= master.o
OBJS2 	= consumer.o
OBJS3   = producer.o
LIBOBJS =
LIBS =
MYLIBS =


.SUFFIXES: .c .o


all: $(TARGET1) $(TARGET2) $(TARGET3)

master: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $@


consumer: $(OBJS2)
	  $(CC) $(CFLAGS) $(OBJS2) -o $@

producer: $(OBJS3)
	  $(CC) $(CFLAGS) $(OBJS3) -o $@
.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.log *.o *~ $(TARGET1) $(TARGET2) $(TARGET3)

