# Parameters
RM = rm -f
CC = gcc
CFLAGS = -Wall
TMP =  $(SRCS:.c=.c~)
OBJS =  $(SRCS:.c=.o)

txt2epub: txt2epub.c
	$(CC) $(CFLAGS) -o txt2epub txt2epub.c

clean:
	$(RM) -f $(OBJS)
	$(RM) -f $(TMP)
