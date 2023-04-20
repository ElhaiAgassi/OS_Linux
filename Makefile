CC = gcc
CFLAGS = -g -Wall
LDFLAGS =  -fPIC

all: cmp copy libcodec encode decode stshell

cmp: cmp.c
	$(CC) $(CFLAGS) -o cmp cmp.c

copy: copy.c
	$(CC) $(CFLAGS) -o copy copy.c

encode: libcodec encode.c 
	$(CC) $(LDFLAGS) -o encode encode.c -L. -l codec 

decode: libcodec decode.c
	$(CC) $(LDFLAGS) -o decode decode.c -L. -l codec

libcodec: codecA.h codecA.c codecB.h codecB.c
	$(CC) $(CFLAGS) -shared codecA.c codecB.c -o libcodec.so

stshell: stshell.c
	$(CC) $(CFLAGS) -o stshell stshell3.c -lreadline

.PHONY: clean
clean:
	rm -f encode decode stshell *.so cmp copy
