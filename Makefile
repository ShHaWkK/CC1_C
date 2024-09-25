# Makefile

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude/

# Fichiers source et objets
SRC = src/main.c src/repl.c src/btree.c src/db.c src/test.c
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
TARGET = class_db

# Règle par défaut pour compiler tout le projet
all: $(TARGET)

# Compilation de l'exécutable final
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers compilés
clean:
	rm -f $(SRCDIR)/*.o $(TARGET)

.PHONY: all clean
