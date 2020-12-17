CFLAGS = -std=gnu99 -O0
all: singleThreadIC

libdummy.so: dummy.c
	$(CC) $(CFLAGS) -fPIC -c dummy.c
	$(CC) $(CFLAGS) -shared -o libdummy.so dummy.o

%: %.c libdummy.so
	$(CC) $(CFLAGS) $< -o $@ -L. -ldummy

clean:
	rm -f singleThreadIC dummy.o libdummy.so
