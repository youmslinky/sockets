CC=gcc
#CFLAGS=-Wall 
#-fdiagnostics-color

SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS)  -o $@ $<

clean:
	rm -f $(PROGS)
