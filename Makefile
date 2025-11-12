CFLAGS  = -std=c11 -O3 -g -Wall -Wextra -Wpedantic
LDFLAGS = -L. -Wl,-rpath,'$$ORIGIN' -lppm -lm

TARGETS = test mandel

all: $(TARGETS)

libppm.so: ppm.c ppm.h
	$(CC) $(CFLAGS) -fPIC -shared ppm.c -o libppm.so

test: main.c libppm.so
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o test

mandel: mandel.c libppm.so
	$(CC) $(CFLAGS) mandel.c $(LDFLAGS) -o mandel

clean:
	rm -f $(TARGETS) *.so *.o *.ppm
