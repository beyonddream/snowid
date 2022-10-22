.POSIX:
CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Os
LDFLAGS = -g

all: snowid

test: unit
	./unit

unit: snowid.o snowid_util.o snowid_test.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o snowid_test.o

snowid_test.o: tests/snowid_test.c snowid.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c tests/snowid_test.c -o $@ -I$(PWD)

snowid: snowid.o snowid_util.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o main.o

snowid.o: snowid.c snowid.h
	$(CC) $(CFLAGS) -c snowid.c -o $@

snowid_util.o: snowid_util.c snowid_util.h
	$(CC) $(CFLAGS) -c snowid_util.c -o $@

main.o: examples/main.c snowid.h
	$(CC) $(CFLAGS) -c examples/main.c -o $@ -I$(PWD)

clean:
	rm -rf main.o snowid.o snowid_util.o snowid snowid_test.o unit
.PHONY: clean