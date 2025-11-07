CFLAGS=-O3 -g

TARGET=test

all: $(TARGET)

libppm.so: ppm.c
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

test: main.c libppm.so
	$(CC) $(CFLAGS) $(LDFLAGS) -lppm -L. $< -o $@

clean:
	rm -fr $(TARGET) *.so
