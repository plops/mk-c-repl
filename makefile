CFLAGS=-ggdb -Wall -Wextra -O2 -march=native
LDFLAGS=-ldl
main: main.c api.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o main main.c
