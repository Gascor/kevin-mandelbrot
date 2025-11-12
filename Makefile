CFLAGS  = -std=c11 -O3 -g -Wall -Wextra -Wpedantic
LDFLAGS = -L. -Wl,-rpath,'$ORIGIN' -lppm -lm

TARGETS = test mandel

all: $(TARGETS)

libppm.so: ppm.c
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

test: main.c libppm.so
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o $@

mandel: mandel.c libppm.so
	$(CC) $(CFLAGS) mandel.c $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS) *.so *.o *.ppm
