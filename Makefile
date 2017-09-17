PROGRAM = matrices

CC = gcc
WFLAGS = -Wall
SYMFLAGS = -g
CFLAGS = $(WFLAGS) $(SYMFLAGS)

SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

all: $(PROGRAM)

$(PROGRAM): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(PROGRAM)

test: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(OBJS) $(PROGRAM)

