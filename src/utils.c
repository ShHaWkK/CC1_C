#include "../include/utils.h"
#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_NAME_LENGTH 255

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_RESET   "\033[0m"

char* command_history[MAX_HISTORY];
int history_count = 0;

char* my_strdup(const char* s);

char* my_strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str == NULL) return NULL;
    return memcpy(new_str, s, len);
}

void store_command(const char* command) {
    if (history_count < MAX_HISTORY) {
        command_history[history_count] = my_strdup(command);
        history_count++;
    } else {
        free(command_history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            command_history[i-1] = command_history[i];
        }
        command_history[MAX_HISTORY-1] = my_strdup(command);
    }
}

void save_command_history(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("%sError: Unable to open file for saving command history.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", command_history[i]);
    }
    fclose(file);
    printf("%sCommand history saved successfully.%s\n", COLOR_GREEN, COLOR_RESET);
}

void load_command_history(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("%sNo previous command history found.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    char line[MAX_NAME_LENGTH];
    while (fgets(line, sizeof(line), file) && history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = 0;  // Remove newline
        command_history[history_count] = my_strdup(line);
        history_count++;
    }
    fclose(file);
    printf("%sCommand history loaded successfully.%s\n", COLOR_GREEN, COLOR_RESET);
}

void print_history() {
    printf("%s+---------------------------------------+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|             Command History           |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s+---------------------------------------+%s\n", COLOR_CYAN, COLOR_RESET);
    for (int i = 0; i < history_count; i++) {
        printf("%s| %3d: %-35s |%s\n", COLOR_CYAN, i+1, command_history[i], COLOR_RESET);
    }
    printf("%s+---------------------------------------+%s\n", COLOR_CYAN, COLOR_RESET);
}

void print_help() {
    printf("%s+---------------------------------------+%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s|           Available Commands          |%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s+---------------------------------------+%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "create table <table_name>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "add column <table> <column> <type>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "insert <id> <name>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "select [cols] from <table> [where]", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "update <table> set <col> = <val>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "delete from <table> where id = <id>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "show tables", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "show columns <table>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "join <table1> <table2> on <column>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "save <filename>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "load <filename>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "INSERT INTO <table> (cols) VALUES", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "DROP TABLE <table>", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "help", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, "history", COLOR_RESET);
    printf("%s| %-37s |%s\n", COLOR_MAGENTA, ".exit", COLOR_RESET);
    printf("%s+---------------------------------------+%s\n", COLOR_MAGENTA, COLOR_RESET);
}

int validate_insert(int id, char* name) {
    if (id <= 0) {
        printf("%sError: ID must be a positive integer.%s\n", COLOR_RED, COLOR_RESET);
        return 0;
    }
    if (strlen(name) == 0 || strlen(name) >= MAX_NAME_LENGTH) {
        printf("%sError: Name must be between 1 and %d characters.%s\n", COLOR_RED, MAX_NAME_LENGTH - 1, COLOR_RESET);
        return 0;
    }
    return 1;
}

void free_command_history() {
    for (int i = 0; i < history_count; i++) {
        free(command_history[i]);
    }
}
