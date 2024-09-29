// src/utils.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/database.h"

#define MAX_HISTORY 100
#define MAX_NAME_LENGTH 255

extern char* command_history[];
extern int history_count;

char* my_strdup(const char* str) {
    size_t len = strlen(str) + 1;
    if (len > MAX_NAME_LENGTH) {
        len = MAX_NAME_LENGTH;
    }
    char* copy = malloc(len);
    if (copy != NULL) {
        strncpy(copy, str, len - 1);
        copy[len - 1] = '\0';
    }
    return copy;
}

void store_command(const char* command) {
    if (history_count < MAX_HISTORY) {
        command_history[history_count] = my_strdup(command);
        history_count++;
    }
}

void save_command_history(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("\033[31m✗ Error: Unable to open file %s\033[0m\n", filename);
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", command_history[i]);
    }
    fclose(file);
    printf("\033[32m✓ Command history saved to %s\033[0m\n", filename);
}

void load_command_history(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL && history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = '\0';
        store_command(line);
    }
    fclose(file);
}

void print_history() {
    printf("\n\033[36m=== Command History ===\033[0m\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, command_history[i]);
    }
}

void print_help() {
    printf("\n\033[36m=== Available Commands ===\033[0m\n");
    printf("create table <table_name>\n");
    printf("add column <table_name> <column_name> <column_type>\n");
    printf("insert <id> <name>\n");
    printf("select\n");
    printf("select id <id>\n");
    printf("select <column1>, <column2>, ... from <table_name>\n");
    printf("update <name> where id = <id>\n");
    printf("delete <id>\n");
    printf("show tables\n");
    printf("show columns <table_name>\n");
    printf("join <table1> <table2> on <join_column>\n");
    printf("INSERT INTO <table_name> (column1, column2, ...) VALUES (value1, value2, ...)\n");
    printf("DROP TABLE <table_name>\n");
    printf("save <filename>\n");
    printf("load <filename>\n");
    printf("help\n");
    printf("history\n");
    printf(".exit\n");
}

void show_tables(Database* db) {
    printf("\n\033[36m=== Tables in the database ===\033[0m\n");
    for (int i = 0; i < db->num_tables; i++) {
        printf("- %s\n", db->tables[i].name);
    }
}

void show_columns(Database* db, const char* table_name) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31m✗ Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    printf("\n\033[36m=== Columns in table '%s' ===\033[0m\n", table_name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("- %s (%s)\n", table->columns[i].name, table->columns[i].type);
    }
}

int validate_insert(int id, char* name) {
    if (id <= 0) {
        printf("\033[31mError: ID must be a positive integer.\033[0m\n");
        return 0;
    }
    if (strlen(name) == 0) {
        printf("\033[31mError: Name cannot be empty.\033[0m\n");
        return 0;
    }
    // Vérification de l'ID (supposons que search_row est défini ailleurs)
    // TreeNode* existing_node = search_row(id);
    // if (existing_node != NULL) {
    //     printf("\033[31mError: ID %d already exists.\033[0m\n", id);
    //     return 0;
    // }
    return 1; 
}
