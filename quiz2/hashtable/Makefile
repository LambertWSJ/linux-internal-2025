CC=gcc
BIN=prog
RM=rm -rf
DEBUG:=0
CFLAGS:=
OBJS:=

CFLAGS += -I../../include
CFLAGS += -std=gnu99

ifeq ($(DEBUG), 1)
CFLAGS += -g
else
CFLAGS += -O2
endif

OBJS+=main.o
OBJS+=map.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) -o $(BIN) $(CFLAGS) $^

clean:
	$(RM) $(OBJS) $(BIN)
