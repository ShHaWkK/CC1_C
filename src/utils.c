/*
*  ---------------------------------------------------------------------------------
*  Fichier: utils.c
*  Author : ShHawk
*  Date : 25/09/2024
*  Description : Sert à valider les entrées utilisateur
*  ---------------------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/database.h"

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
        printf("Error: Unable to open file %s\n", filename);
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", command_history[i]);
    }
    fclose(file);
    printf("Command history saved to %s\n", filename);
}

void load_command_history(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        store_command(line);
    }
    fclose(file);
}

void print_history() {
    printf("\n=== Command History ===\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, command_history[i]);
    }
}

void print_help() {
    printf("\n=== Available Commands ===\n");
    printf("create table <table_name>\n");
    printf("add column <table_name> <column_name> <column_type>\n");
    printf("insert <table_name> <value1> <value2> ...\n");
    printf("select <table_name>\n");
    printf("select <table_name> where <column_name> = <value>\n");
    printf("update <table_name> set <column_name> = <value> where id = <row_id>\n");
    printf("delete from <table_name> where id = <row_id>\n");
    printf("show tables\n");
    printf("save <filename>\n");
    printf("load <filename>\n");
    printf("help\n");
    printf("history\n");
    printf("exit\n");
}

void show_tables(Database* db) {
    printf("Tables in the database:\n");
    for (int i = 0; i < db->num_tables; i++) {
        printf("- %s\n", db->tables[i].name);
    }
}
