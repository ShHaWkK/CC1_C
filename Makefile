# Compilateur et options communes
CC = gcc
CFLAGS_COMMON = -Wall -Wextra -Werror -std=c99 -g -Iinclude/
CC_AFL = afl-clang-fast
CFLAGS_AFL = $(CFLAGS_COMMON) -fsanitize=address,fuzzer

# Fichiers objets pour la version principale
OBJ_MAIN = src/main.o src/repl.o src/btree.o src/database.o src/utils.o src/table.o

# nom .exe
TARGET_MAIN = class_db

ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXE = .exe
	CFLAGS = $(CFLAGS_COMMON)  
else
	RM = rm -f
	EXE =
	CFLAGS = $(CFLAGS_COMMON) -fsanitize=address 
endif

all: $(TARGET_MAIN)$(EXE) 

$(TARGET_MAIN)$(EXE): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) src\*.o
	-$(RM) $(TARGET_MAIN)$(EXE) 
else
	$(RM) src/*.o $(TARGET_MAIN)$(EXE) $(TARGET_MAIN)_fuzz fuzz_out
endif

class_db: $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

memory_check: $(TARGET_MAIN)
ifeq ($(OS),Windows_NT)
	drmemory -- ./$(TARGET_MAIN)
else
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET_MAIN)
endif

valgrind: $(TARGET_MAIN)
ifeq ($(OS),Windows_NT)
	@echo "Valgrind is not available on Windows"
else
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET_MAIN)
endif


fuzz: $(OBJ_MAIN:%.o=%_fuzz.o)
	$(CC_AFL) $(CFLAGS_AFL) -o $(TARGET_MAIN)_fuzz $^

%_fuzz.o: %.c
	$(CC_AFL) $(CFLAGS_AFL) -c $< -o $@

fuzz_in:
	mkdir -p fuzz_in
	echo "Sample input" > fuzz_in/sample.txt

run_fuzzer: fuzz fuzz_in
	mkdir -p fuzz_out
	afl-fuzz -i fuzz_in -o fuzz_out ./$(TARGET_MAIN)_fuzz

test: valgrind run_fuzzer

version:
	@$(CC) --version

.PHONY: all clean valgrind fuzz run_fuzzer test version
