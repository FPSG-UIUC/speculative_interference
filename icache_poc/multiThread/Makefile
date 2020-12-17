override CFLAGS += -std=gnu99 -O0 -fPIE -pie -Q
all: multiThreadIC

libdummy.so: dummy.c
	$(CC) $(CFLAGS) -fPIC -c dummy.c
	$(CC) $(CFLAGS) -shared -o libdummy.so dummy.o

%: %.c libdummy.so
	$(CC) $(CFLAGS) $< -o $@ -L. -ldummy -lpthread

clean:
	rm -f multiThreadIC dummy.o libdummy.so
