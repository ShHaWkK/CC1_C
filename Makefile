CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude/
OBJ = src/main.o src/repl.o src/btree.o src/db.o src/test.o
TARGET = class_db

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/repl.o: src/repl.c
	$(CC) $(CFLAGS) -c $< -o $@

src/btree.o: src/btree.c
	$(CC) $(CFLAGS) -c $< -o $@

src/db.o: src/db.c
	$(CC) $(CFLAGS) -c $< -o $@

src/test.o: src/test.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
