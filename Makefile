PROGRAM = matrices

CC = gcc
WFLAGS = -Wall
SYMFLAGS = -g
CFLAGS = $(WFLAGS) $(SYMFLAGS)
VGLOG   = valgrind.log
VGFLAGS = --leak-check=full --log-file=$(VGLOG)
VG = valgrind

SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)

all: $(PROGRAM)

$(PROGRAM): $(SRCS)
	@$(CC) $(CFLAGS) $(SRCS) -o $(PROGRAM)

test: $(PROGRAM)
	./$(PROGRAM)

memtest: $(PROGRAM)
	@$(VG) $(VGFLAGS) ./$(PROGRAM)
	@less $(VGLOG)

clean:
	@echo "  CLEANING    "
	@rm -f $(OBJS) $(PROGRAM)

