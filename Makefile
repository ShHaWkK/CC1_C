CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g -fsanitize=address -Iinclude/

OBJ_MAIN = src/main.o src/repl.o src/btree.o src/db.o src/utils.o
OBJ_TEST = src/test.o src/btree.o src/db.o

TARGET_MAIN = class_db
TARGET_TEST = class_db_tests

# Détection de l'OS pour suppression de fichiers
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
else
    RM = rm -f
    EXE =
endif

all: $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)

$(TARGET_MAIN)$(EXE): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $(OBJ_MAIN)

$(TARGET_TEST)$(EXE): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $(OBJ_TEST)

# Compilation des fichiers objets
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) src/*.o $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)

test: $(TARGET_TEST)
	./$(TARGET_TEST)$(EXE)

# Ajouter une règle pour vérifier la version du compilateur
version:
	@$(CC) --version
