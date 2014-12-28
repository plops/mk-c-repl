CFLAGS=-ggdb -Wall -Wextra -O2 -march=native
LDFLAGS=-ldl

all: main librun.so

main: main.c api.h
	$(CC) $(CFLAGS) -ldl -o main main.c

librun.so: run.c api.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o librun.so -shared -fPIC run.c
