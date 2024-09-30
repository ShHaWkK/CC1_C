#include "../include/utils.h"
#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_NAME_LENGTH 255

char* command_history[MAX_HISTORY];
int history_count = 0;

// Function to replace strdup
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
        printf("\033[31mError: Unable to open file for saving command history.\033[0m\n");
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", command_history[i]);
    }
    fclose(file);
    printf("\033[32mCommand history saved successfully.\033[0m\n");
}

void load_command_history(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("\033[33mNo previous command history found.\033[0m\n");
        return;
    }
    char line[MAX_NAME_LENGTH];
    while (fgets(line, sizeof(line), file) && history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = 0;  // Remove newline
        command_history[history_count] = my_strdup(line);
        history_count++;
    }
    fclose(file);
    printf("\033[32mCommand history loaded successfully.\033[0m\n");
}

void print_history() {
    printf("\033[1m\033[36mCommand History:\033[0m\n");
    for (int i = 0; i < history_count; i++) {
        printf("%3d: %s\n", i+1, command_history[i]);
    }
}

void print_help() {
    printf("\033[1m\033[36mAvailable commands:\033[0m\n");
    printf("  \033[1mcreate table\033[0m <table_name>\n");
    printf("  \033[1madd column\033[0m <table_name> <column_name> <column_type>\n");
    printf("  \033[1minsert\033[0m <id> <name>\n");
    printf("  \033[1mselect\033[0m [<column1>, <column2>, ...] from <table_name> [where <condition>]\n");
    printf("  \033[1mupdate\033[0m <table_name> set <column_name> = <new_value> where id = <id>\n");
    printf("  \033[1mdelete\033[0m from <table_name> where id = <id>\n");
    printf("  \033[1mhelp\033[0m\n");
    printf("  \033[1mhistory\033[0m\n");
    printf("  \033[1mshow tables\033[0m\n");
    printf("  \033[1mshow columns\033[0m <table_name>\n");
    printf("  \033[1mjoin\033[0m <table1> <table2> on <join_column>\n");
    printf("  \033[1msave\033[0m <filename>\n");
    printf("  \033[1mload\033[0m <filename>\n");
    printf("  \033[1mINSERT INTO\033[0m <table_name> (column1, column2, ...) VALUES (value1, value2, ...)\n");
    printf("  \033[1mDROP TABLE\033[0m <table_name>\n");
    printf("  \033[1m.exit\033[0m\n");
}

int validate_insert(int id, char* name) {
    if (id <= 0) {
        printf("\033[31mError: ID must be a positive integer.\033[0m\n");
        return 0;
    }
    if (strlen(name) == 0 || strlen(name) >= MAX_NAME_LENGTH) {
        printf("\033[31mError: Name must be between 1 and %d characters.\033[0m\n", MAX_NAME_LENGTH - 1);
        return 0;
    }
    return 1;
}

void free_command_history() {
    for (int i = 0; i < history_count; i++) {
        free(command_history[i]);
    }
}
