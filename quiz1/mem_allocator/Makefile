ROOT=../..
CC=gcc
BIN=prog
RM=rm -rf
DEBUG:=0
CFLAGS:=
OBJS:=

CFLAGS += -std=gnu99
CFLAGS += -Wall -Wextra

ifeq ($(DEBUG), 1)
CFLAGS += -g
else
CFLAGS += -O2
endif

OBJS+=main.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) $^ -o $(BIN)

clean:
	$(RM) $(OBJS) $(BIN)
