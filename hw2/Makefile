CC = gcc
CFLAGS = -Wall
TARGETS = memorysystem
SRCS = memorysystem.c

all: $(TARGETS)

memorysystem: memorysystem.o
	$(CC) $(CFLAGS) -o $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS) *.o

run: $(TARGETS)
	./memorysystem

