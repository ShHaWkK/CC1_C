#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "../include/database.h"
#include "../include/repl.h"
#include "../include/utils.h"
#include "../include/btree.h"

#define MAX_INPUT 1000
#define MAX_HISTORY 100
#define MAX_NAME_LENGTH 255
#define MAX_COLUMNS 10

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
    char** column_names;
} Statement;

extern char* command_history[MAX_HISTORY];
extern int history_count;

Database db;
extern TreeNode* g_root;

// Function prototypes
char* my_strdup(const char* s);
void repl(void) __attribute__((unused));

Database db;
extern TreeNode* g_root;

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

char* trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int prepare_statement(char* input, Statement* statement) {
    statement->has_where = 0;
    statement->num_columns = 0;
    statement->column_names = NULL;

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
    }else if (strncmp(input, "select", 6) == 0) {
    char* from = strstr(input, "from");
    if (from) {
        statement->type = STATEMENT_SELECT_FROM;
        char columns[MAX_INPUT] = {0};
        char* table_name_start = from + 5; // Skip "from "
        
        // Extract column names
        size_t column_length = from - (input + 7); // 7 is the length of "select "
        strncpy(columns, input + 7, column_length);
        columns[column_length] = '\0';
        
        // Extract table name
        sscanf(table_name_start, "%s", statement->table_name);
        
        // Process column names
        if (strncmp(columns, "*", 1) == 0) {
            statement->num_columns = 0; // 0 indicates all columns
        } else {
            char* token = strtok(columns, ",");
            statement->num_columns = 0;
            statement->column_names = malloc(MAX_COLUMNS * sizeof(char*));
            
            while (token != NULL && statement->num_columns < MAX_COLUMNS) {
                token = trim(token);
                statement->column_names[statement->num_columns] = my_strdup(token);
                statement->num_columns++;
                token = strtok(NULL, ",");
            }
        }
        
        // Debug print
        printf("Debug (prepare_statement): Number of columns: %d\n", statement->num_columns);
        if (statement->num_columns > 0) {
            for (int i = 0; i < statement->num_columns; i++) {
                printf("Debug (prepare_statement): Column %d: %s\n", i, statement->column_names[i]);
            }
        } else {
            printf("Debug (prepare_statement): Selecting all columns\n");
        }
        printf("Debug (prepare_statement): Table name: %s\n", statement->table_name);
        
        return 1;
    }
}

else if (strncmp(input, "update", 6) == 0) {
        statement->type = STATEMENT_UPDATE;
        char value[MAX_NAME_LENGTH];
        int result = sscanf(input, "update %s set %s = %s where id = %d", 
                      statement->table_name, statement->column_name, 
                      value, &(statement->id));
        if (result == 4) {
            // Remove quotes if present
            if (value[0] == '\'' || value[0] == '"') {
                memmove(value, value + 1, strlen(value));
                value[strlen(value) - 1] = '\0';
            }
            strncpy(statement->name, value, MAX_NAME_LENGTH - 1);
            statement->name[MAX_NAME_LENGTH - 1] = '\0';
            return 1;
        }
        return 0;
    } else if (strncmp(input, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        return sscanf(input, "delete from %s where id = %d", 
                      statement->table_name, &(statement->id)) == 2;
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
            if (i == 0) {
                statement->column_names = malloc(MAX_COLUMNS * sizeof(char*));
                if (statement->column_names == NULL) {
                    return 0;
                }
            }
            statement->column_names[i] = malloc(MAX_NAME_LENGTH * sizeof(char));
            if (statement->column_names[i] == NULL) {
                for (int j = 0; j < i; j++) {
                    free(statement->column_names[j]);
                }
                free(statement->column_names);
                return 0;
            }
            column = trim(column);
            strncpy(statement->column_names[i], column, MAX_NAME_LENGTH - 1);
            statement->column_names[i][MAX_NAME_LENGTH - 1] = '\0';
            column = strtok(NULL, ",");
            i++;
        }
        statement->num_columns = i;
        
        char* value = strtok(values_start, ",");
        i = 0;
        while (value != NULL && i < MAX_COLUMNS) {
            value = trim(value);
            // Remove quotes if present
            if (value[0] == '\'' || value[0] == '"') {
                memmove(value, value + 1, strlen(value));
                value[strlen(value) - 1] = '\0';
            }
            strncpy(statement->values[i], value, MAX_NAME_LENGTH - 1);
            statement->values[i][MAX_NAME_LENGTH - 1] = '\0';
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


void free_statement(Statement* statement) {
    if (statement->column_names != NULL) {
        for (int i = 0; i < statement->num_columns; i++) {
            free(statement->column_names[i]);
        }
        free(statement->column_names);
        statement->column_names = NULL;
    }
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
            {
                char* values[] = {statement->name};
                insert_row(&db, "main", values);
            }
            break;
        case STATEMENT_SELECT:
        case STATEMENT_SELECT_WHERE:
            if (statement->has_where) {
                char id_str[20];
                snprintf(id_str, sizeof(id_str), "%d", statement->where_id);
                select_where(&db, statement->table_name, "id", id_str);
            } else {
                select_all(&db, statement->table_name);
            }
            break;
        case STATEMENT_SELECT_FROM:
   		 if (statement->num_columns == 0) {
       	 select_all(&db, statement->table_name);
   		 } else {
       		 select_from(&db, statement->table_name, (const char**)statement->column_names, statement->num_columns);
   		 }
   	 break;

        case STATEMENT_UPDATE:
            update_row(&db, statement->table_name, statement->id, statement->column_name, statement->name);
            break;
        case STATEMENT_DELETE:
            delete_row(&db, statement->table_name, statement->id);
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
            {
                char* column_ptrs[MAX_COLUMNS];
                char* value_ptrs[MAX_COLUMNS];
                for (int i = 0; i < statement->num_columns; i++) {
                    column_ptrs[i] = statement->column_names[i];
                    value_ptrs[i] = statement->values[i];
                }
                insert_into(&db, statement->table_name, column_ptrs, value_ptrs, statement->num_columns);
            }
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
            save_tree(file, g_root);
            fclose(file);
            printf("\033[32m✓ Database and history saved successfully.\033[0m\n");
            exit(EXIT_SUCCESS);
        default:
            printf("\033[31m✗ Unknown statement type\033[0m\n");
            break;
    }
}

void repl(void) {
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
            free_statement(&statement);
        } else {
            printf("\033[31m✗ Unrecognized statement: %s\033[0m\n", input);
        }
    }
}
