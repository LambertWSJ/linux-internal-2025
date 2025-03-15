CC=gcc
BIN=prog
RM=rm -rf
DEBUG:=0
CFLAGS:=
LDLAGS:=
OBJS:=

CFLAGS += -I../../include
CFLAGS += -std=gnu99
LDLAGS += -lm

ifeq ($(DEBUG), 1)
CFLAGS += -g3
else
CFLAGS += -O2
endif

OBJS+=main.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) -o $(BIN) $^ $(LDLAGS)

clean:
	$(RM) $(OBJS) $(BIN)
