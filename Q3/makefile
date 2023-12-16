# Parameters
RM = rm -f
CC = gcc
CFLAGS = -Wall
TMP =  $(SRCS:.c=.c~)
OBJS =  $(SRCS:.c=.o)

tokenring: tokenring.c
	$(CC) $(CFLAGS) -o tokenring tokenring.c

clean:
	$(RM) -f $(OBJS)
	$(RM) -f $(TMP)
