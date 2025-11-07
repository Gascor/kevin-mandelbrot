CC=gcc
CFLAGS=-std=c11 -O3 -g -Wall -Wextra -Wpedantic
LDFLAGS=-L. -Wl,-rpath,'$$ORIGIN'
LDLIBS=-lppm -lm

TARGETS=test mandel

all: $(TARGETS)

libppm.so: ppm.c ppm.h
	$(CC) $(CFLAGS) -fPIC -shared ppm.c -o libppm.so

test: main.c libppm.so ppm.h
	$(CC) $(CFLAGS) main.c $(LDFLAGS) $(LDLIBS) -o test

mandel: mandel.c libppm.so ppm.h
	$(CC) $(CFLAGS) mandel.c $(LDFLAGS) $(LDLIBS) -o mandel

clean:
	rm -f $(TARGETS) *.so *.o *.ppm
