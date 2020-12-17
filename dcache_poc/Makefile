CC:= gcc
CFLAGS:= -O3 -D_POSIX_SOURCE -D_GNU_SOURCE -m64 -g
LIBS:= -lpthread -lrt -lm

all: obj bin out main

main: obj/main.o obj/util.o
	$(CC) -o bin/$@ $^ $(LIBS)

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

obj:
	mkdir -p $@

bin:
	mkdir -p $@

out:
	mkdir -p $@

clean:
	rm -rf bin obj

.PHONY: all clean
