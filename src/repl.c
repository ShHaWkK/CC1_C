/*
*  Fichier: repl.c
*  Author : ShHawk
*  Date : 26/09/2024
* ---------------------------------------------------------------------------------
*  Description : Sert à lire les commandes de l'utilisateur et les exécuter.
*  Fonctions importantes :
*  - print_prompt : Affiche le prompt
*  - read_input : Lit l'entrée utilisateur
*  - prepare_statement : Prépare une commande
*  - print_help : Affiche les commandes disponibles
*  - execute_statement : Exécute une commande
*  - store_command : Stocke les commandes dans un historique
*  - print_history : Affiche l'historique des commandes
*  - save_command_history : Sauvegarde l'historique dans un fichier
*  - load_command_history : Charge l'historique depuis un fichier
*  - my_strdup : Duplique une chaîne de caractères
* ---------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/database.h"
#include "../include/repl.h"
#include "../include/utils.h"
#include "../include/btree.h"

#define MAX_INPUT 1000
#define MAX_HISTORY 100
#define MAX_NAME_LENGTH 255

typedef enum {
    STATEMENT_CREATE_TABLE,
    STATEMENT_ADD_COLUMN,
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_SELECT_WHERE,
    STATEMENT_UPDATE,
    STATEMENT_DELETE,
    STATEMENT_HELP,
    STATEMENT_HISTORY,
    STATEMENT_SHOW_TABLES,
    STATEMENT_SHOW_COLUMNS,
    STATEMENT_SELECT_FROM,
    STATEMENT_JOIN,
    STATEMENT_SAVE,
    STATEMENT_LOAD,
    STATEMENT_INSERT_INTO,
    STATEMENT_DROP_TABLE,
    STATEMENT_EXIT
} StatementType;

typedef struct {
    StatementType type;
    char table_name[MAX_NAME_LENGTH];
    char column_name[MAX_NAME_LENGTH];
    char column_type[20];
    char values[MAX_COLUMNS][MAX_NAME_LENGTH];
    char where_column[MAX_NAME_LENGTH];
    char where_value[MAX_NAME_LENGTH];
    int row_id;
    int id;
    char name[MAX_NAME_LENGTH];
    int where_id;
    int has_where;
    int num_columns;
    char column_names[MAX_COLUMNS][MAX_NAME_LENGTH];
} Statement;

char* command_history[MAX_HISTORY];
int history_count = 0;

Database db;
TreeNode* root = NULL;

void init_repl() {
    init_database(&db);
    load_command_history("command_history.txt");
    load_tree("db_save.txt");
    printf("Database and command history loaded successfully.\n");
}

void print_prompt() {
    printf("\033[36mdb_alex > \033[0m");
}

void read_input(char* buffer, size_t buffer_length) {
    if (fgets(buffer, buffer_length, stdin) == NULL) {
        printf("\033[31m✗ Error reading input\033[0m\n");
        exit(EXIT_FAILURE);
    }
    buffer[strcspn(buffer, "\n")] = 0;
}

int prepare_statement(char* input, Statement* statement) {
    statement->has_where = 0;

    if (strncmp(input, "create table", 12) == 0) {
        statement->type = STATEMENT_CREATE_TABLE;
        return sscanf(input, "create table %s", statement->table_name) == 1;
    } else if (strncmp(input, "add column", 10) == 0) {
        statement->type = STATEMENT_ADD_COLUMN;
        return sscanf(input, "add column %s %s %s", statement->table_name, statement->column_name, statement->column_type) == 3;
    } else if (strncmp(input, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(input, "insert %d %254s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(input, "select id", 9) == 0) {
        statement->type = STATEMENT_SELECT_WHERE;
        return sscanf(input, "select id %d", &(statement->id)) == 1;
    } else if (strncmp(input, "select", 6) == 0) {
        char* from = strstr(input, "from");
        if (from) {
            statement->type = STATEMENT_SELECT_FROM;
            char columns[MAX_INPUT];
            sscanf(input, "select %[^from] from %s", columns, statement->table_name);
            char* token = strtok(columns, ",");
            int i = 0;
            while (token != NULL && i < MAX_COLUMNS) {
                while (*token == ' ') token++; // skip leading spaces
                char* end = token + strlen(token) - 1;
                while (end > token && *end == ' ') end--; // remove trailing spaces
                *(end + 1) = '\0';
                strcpy(statement->values[i], token);
                i++;
                token = strtok(NULL, ",");
            }
            statement->num_columns = i;
            return 1;
        } else {
            statement->type = STATEMENT_SELECT;
            char* where = strstr(input, "where");
            if (where) {
                statement->has_where = 1;
                return sscanf(where, "where id = %d", &(statement->where_id)) == 1;
            }
            return 1;
        }
    } else if (strncmp(input, "update", 6) == 0) {
        statement->type = STATEMENT_UPDATE;
        return sscanf(input, "update %254s where id = %d", statement->name, &(statement->id)) == 2;
    } else if (strncmp(input, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        return sscanf(input, "delete %d", &(statement->id)) == 1;
    } else if (strcmp(input, "help") == 0) {
        statement->type = STATEMENT_HELP;
        return 1;
    } else if (strcmp(input, "history") == 0) {
        statement->type = STATEMENT_HISTORY;
        return 1;
    } else if (strcmp(input, "show tables") == 0) {
        statement->type = STATEMENT_SHOW_TABLES;
        return 1;
    } else if (strncmp(input, "show columns", 12) == 0) {
        statement->type = STATEMENT_SHOW_COLUMNS;
        return sscanf(input, "show columns %s", statement->table_name) == 1;
    } else if (strncmp(input, "join", 4) == 0) {
        statement->type = STATEMENT_JOIN;
        return sscanf(input, "join %s %s on %s", statement->table_name, statement->column_name, statement->column_type) == 3;
    } else if (strncmp(input, "save", 4) == 0) {
        statement->type = STATEMENT_SAVE;
        return sscanf(input, "save %s", statement->table_name) == 1;
    } else if (strncmp(input, "load", 4) == 0) {
        statement->type = STATEMENT_LOAD;
        return sscanf(input, "load %s", statement->table_name) == 1;
    } else if (strncmp(input, "INSERT INTO", 11) == 0) {
        statement->type = STATEMENT_INSERT_INTO;
        char* table_name_start = input + 12;
        char* columns_start = strchr(table_name_start, '(');
        if (columns_start == NULL) {
            return 0;
        }
        *columns_start = '\0';
        sscanf(table_name_start, "%s", statement->table_name);
        char* values_start = strstr(columns_start + 1, ") VALUES (");
        if (values_start == NULL) {
            return 0;
        }
        *values_start = '\0';
        values_start += 10;
        char* values_end = strchr(values_start, ')');
        if (values_end == NULL) {
            return 0;
        }
        *values_end = '\0';
        
        char* column = strtok(columns_start + 1, ",");
        int i = 0;
        while (column != NULL && i < MAX_COLUMNS) {
            while (*column == ' ') column++;
            strcpy(statement->column_names[i], column);
            column = strtok(NULL, ",");
            i++;
        }
        statement->num_columns = i;
        
        char* value = strtok(values_start, ",");
        i = 0;
        while (value != NULL && i < MAX_COLUMNS) {
            while (*value == ' ') value++;
            strcpy(statement->values[i], value);
            value = strtok(NULL, ",");
            i++;
        }
        
        return statement->num_columns == i;
    } else if (strncmp(input, "DROP TABLE", 10) == 0) {
        statement->type = STATEMENT_DROP_TABLE;
        return sscanf(input, "DROP TABLE %s", statement->table_name) == 1;
    } else if (strcmp(input, ".exit") == 0) {
        statement->type = STATEMENT_EXIT;
        return 1;
    }
    return 0;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case STATEMENT_CREATE_TABLE:
            create_table(&db, statement->table_name);
            break;
        case STATEMENT_ADD_COLUMN:
            add_column(&db, statement->table_name, statement->column_name, statement->column_type);
            break;
        case STATEMENT_INSERT:
            insert_row(statement->id, statement->name);
            break;
        case STATEMENT_SELECT:
            if (statement->has_where) {
                select_row_by_id(statement->where_id);
            } else {
                select_row();
            }
            break;
        case STATEMENT_SELECT_WHERE:
            select_row_by_id(statement->id);
            break;
        case STATEMENT_UPDATE:
            update_row(statement->id, statement->name);
            break;
        case STATEMENT_DELETE:
            delete_row(statement->id);
            break;
        case STATEMENT_HELP:
            print_help();
            break;
        case STATEMENT_HISTORY:
            print_history();
            break;
        case STATEMENT_SHOW_TABLES:
            show_tables(&db);
            break;
        case STATEMENT_SHOW_COLUMNS:
            show_columns(&db, statement->table_name);
            break;
        case STATEMENT_SELECT_FROM:
            select_from(&db, statement->table_name, statement->values, statement->num_columns);
            break;
        case STATEMENT_JOIN:
            join_tables(&db, statement->table_name, statement->column_name, statement->column_type);
            break;
        case STATEMENT_SAVE:
            save_database(&db, statement->table_name);
            break;
        case STATEMENT_LOAD:
            load_database(&db, statement->table_name);
            break;
        case STATEMENT_INSERT_INTO:
            insert_into(&db, statement->table_name, statement->column_names, statement->values, statement->num_columns);
            break;
        case STATEMENT_DROP_TABLE:
            drop_table(&db, statement->table_name);
            break;
        case STATEMENT_EXIT:
            printf("Saving history...\n");
            save_command_history("command_history.txt");
            printf("Saving database...\n");
            FILE* file = fopen("db_save.txt", "w");
            assert(file != NULL);
            save_tree(file, root);
            fclose(file);
            printf("\033[32m✓ Database and history saved successfully.\033[0m\n");
            exit(EXIT_SUCCESS);
        default:
            printf("\033[31m✗ Unknown statement type\033[0m\n");
            break;
    }
}

void repl() {
    char input[MAX_INPUT];
    Statement statement;

    printf("-------------------------------------------------\n");
    printf("\033[32mWelcome to the database!\033[0m\nType 'help' for available commands.\n");
    printf("-------------------------------------------------\n");

    init_repl();

    while (1) {
        print_prompt();
        read_input(input, sizeof(input));
        store_command(input);

        if (prepare_statement(input, &statement)) {
            execute_statement(&statement);
        } else {
            printf("\033[31m✗ Unrecognized statement: %s\033[0m\n", input);
        }
    }
}
