.POSIX:
CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Os
LDFLAGS = -g

all: snowid

snowid: snowid.o snowid_util.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o main.o

snowid.o: snowid.c snowid.h
	$(CC) $(CFLAGS) -c snowid.c -o $@

snowid_util.o: snowid_util.c snowid_util.h
	$(CC) $(CFLAGS) -c snowid_util.c -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@

clean:
	rm -rf main.o snowid.o snowid_util.o snowid
.PHONY: clean