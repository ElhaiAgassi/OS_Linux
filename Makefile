CC = gcc
CFLAGS = -Wall
LDFLAGS =  -fPIC

all: cmp copy libcodecA libcodecB encode decode

cmp: cmp.c
	$(CC) $(CFLAGS) -o cmp cmp.c

copy: copy.c
	$(CC) $(CFLAGS) -o copy copy.c

encode: libcodecA libcodecB encode.c 
	$(CC) $(LDFLAGS) -o encode encode.c -L. -lcodecA -lcodecB

decode: libcodecA libcodecB decode.c
	$(CC) $(LDFLAGS) -o decode decode.c -L. -lcodecA -lcodecB

libcodecA: codecA.h codecA.c
	$(CC) $(CFLAGS) -shared codecA.c -o libcodecA.so

libcodecB: codecB.h codecB.c
	$(CC) $(CFLAGS) -shared codecB.c -o libcodecB.so

.PHONY: clean
clean:
	rm -f encode decode *.so cmp copy
