CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude/

# Objet du programme principal
OBJ_MAIN = src/main.o src/repl.o src/btree.o src/db.o src/utils.o
# Objet pour les tests
OBJ_TEST = src/test.o src/btree.o src/db.o

TARGET_MAIN = class_db
TARGET_TEST = class_db_tests

all: $(TARGET_MAIN) $(TARGET_TEST)

# Compilation du programme principal
$(TARGET_MAIN): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $(OBJ_MAIN)

# Compilation des tests
$(TARGET_TEST): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $(OBJ_TEST)

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
	rm -f src/*.o $(TARGET_MAIN) $(TARGET_TEST)
