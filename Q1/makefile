# Parameters
RM = rm -f
CC = gcc
CFLAGS = -Wall
TMP =  $(SRCS:.c=.c~)
OBJS =  $(SRCS:.c=.o)

samples: samples.c
	$(CC) $(CFLAGS) -o samples samples.c

clean:
	$(RM) -f $(OBJS)
	$(RM) -f $(TMP)
