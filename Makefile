CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude/

# Objet du programme principal
OBJ_MAIN = src/main.o src/repl.o src/btree.o src/db.o src/utils.o
# Objet pour les tests
OBJ_TEST = src/test.o src/btree.o src/db.o

TARGET_MAIN = class_db
TARGET_TEST = class_db_tests

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
else
    RM = rm -f
    EXE =
endif

all: $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)

# Compilation du programme principal
$(TARGET_MAIN)$(EXE): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $(OBJ_MAIN)

# Compilation des tests
$(TARGET_TEST)$(EXE): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $(OBJ_TEST)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/repl.o: src/repl.c
	$(CC) $(CFLAGS) -c $< -o $@

src/btree.o: src/btree.c
	$(CC) $(CFLAGS) -c $< -o $@

src/test.o: src/test.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) src/*.o $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)
