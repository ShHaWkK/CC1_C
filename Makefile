# Compilateur et options communes
CC = gcc
CFLAGS_COMMON = -Wall -Wextra -Werror -std=c99 -g -Iinclude/

# Fichiers objets pour la version principale et la version de test
OBJ_MAIN = src/main.o src/repl.o src/btree.o src/db.o src/utils.o src/table.o
OBJ_TEST = src/test.o src/repl.o src/btree.o src/db.o src/utils.o src/table.o

# Noms des cibles principales et de test
TARGET_MAIN = class_db
TARGET_TEST = class_db_tests

# Détection de l'OS pour suppression de fichiers et autres options spécifiques
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
    CFLAGS = $(CFLAGS_COMMON)  # Pas de -fsanitize=address sous Windows
else
    RM = rm -f
    EXE =
    CFLAGS = $(CFLAGS_COMMON) -fsanitize=address  # Ajoute -fsanitize=address sous Unix/Linux
endif

# Règle par défaut : compilation des deux cibles (main et test)
all: $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)

# Compilation de la cible principale (class_db)
$(TARGET_MAIN)$(EXE): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation de la cible de test (class_db_tests)
$(TARGET_TEST)$(EXE): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets (pour les sources .c)
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Commande pour nettoyer les fichiers objets et les exécutables
clean:
ifeq ($(OS),Windows_NT)
	-$(RM) src\*.o
	-$(RM) $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)
else
	$(RM) src/*.o $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)
endif

# Règle pour exécuter les tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)$(EXE)

# Règle pour afficher la version du compilateur
version:
	@$(CC) --version
