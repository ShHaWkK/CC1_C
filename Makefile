# Compilateur et options communes
CC = gcc
CFLAGS_COMMON = -Wall -Wextra -Werror -std=c99 -g -Iinclude/

# Fichiers objets pour la version principale et la version de test
OBJ_MAIN = src/main.o src/repl.o src/btree.o src/db.o src/utils.o src/table.o
OBJ_TEST = src/test.o src/repl.o src/btree.o src/db.o src/utils.o src/table.o

# Noms des cibles principales et de test
TARGET_MAIN = class_db
TARGET_TEST = class_db_tests

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
    CFLAGS = $(CFLAGS_COMMON)  
else
    RM = rm -f
    EXE =
    CFLAGS = $(CFLAGS_COMMON) -fsanitize=address 
endif

all: $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)

$(TARGET_MAIN)$(EXE): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_TEST)$(EXE): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) src\*.o
	-$(RM) $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)
else
	$(RM) src/*.o $(TARGET_MAIN)$(EXE) $(TARGET_TEST)$(EXE)
endif

test: $(TARGET_TEST)
	./$(TARGET_TEST)$(EXE)

version:
	@$(CC) --version
