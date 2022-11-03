.POSIX:
CC = cc
ifdef DEBUGBUILD
	CFLAGS = -std=c99 -Wall -Werror -Wextra -fsanitize=address,undefined
	LDFLAGS = -g3
else
	CFLAGS = -std=c99 -Wall -Werror -Wextra -Os
	LDFLAGS = -g
endif


all: snowid

test: unit
	./unit

bench: benchmark
	./benchmark

unit: snowid.o snowid_util.o snowid_checkpoint.o snowid_test.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o snowid_checkpoint.o snowid_test.o

snowid_test.o: tests/snowid_test.c snowid.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c tests/snowid_test.c -o $@ -I$(PWD)

snowid: snowid.o snowid_util.o snowid_checkpoint.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o snowid_checkpoint.o main.o

snowid.o: snowid.c snowid.h
	$(CC) $(CFLAGS) -c snowid.c -o $@

snowid_util.o: snowid_util.c snowid_util.h
	$(CC) $(CFLAGS) -c snowid_util.c -o $@

snowid_checkpoint.o: snowid_checkpoint.c snowid_checkpoint.h
	$(CC) $(CFLAGS) -c snowid_checkpoint.c -o $@

main.o: main.c snowid.h
	$(CC) $(CFLAGS) -c main.c -o $@ -I$(PWD)

benchmark: snowid.o snowid_util.o snowid_checkpoint.o benchmark.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snowid.o snowid_util.o snowid_checkpoint.o benchmark.o

benchmark.o: perf/benchmark.c snowid.h
	$(CC) $(CFLAGS) -c perf/benchmark.c -o $@ -I$(PWD)

clean:
	rm -rf main.o snowid.o snowid_util.o snowid_checkpoint.o snowid_test.o benchmark.o
	rm -rf snowid benchmark unit timestamp.out 
.PHONY: clean