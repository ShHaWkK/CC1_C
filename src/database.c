#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_database(Database* db) {
    db->num_tables = 0;
}

void create_table(Database* db, const char* table_name) {
    if (db->num_tables >= MAX_TABLES) {
        printf("\033[31mError: Maximum number of tables reached.\033[0m\n");
        return;
    }
    
    Table* new_table = &db->tables[db->num_tables];
    strncpy(new_table->name, table_name, MAX_NAME_LENGTH - 1);
    new_table->name[MAX_NAME_LENGTH - 1] = '\0';
    new_table->num_columns = 0;
    new_table->num_rows = 0;
    new_table->root = NULL;
    db->num_tables++;
    
    printf("\033[32mTable '%s' created successfully.\033[0m\n", table_name);
}

void add_column(Database* db, const char* table_name, const char* column_name, const char* column_type) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    if (table->num_columns >= MAX_COLUMNS) {
        printf("\033[31mError: Maximum number of columns reached for table '%s'.\033[0m\n", table_name);
        return;
    }
    
    Column* new_column = &table->columns[table->num_columns];
    strncpy(new_column->name, column_name, MAX_NAME_LENGTH - 1);
    new_column->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(new_column->type, column_type, 19);
    new_column->type[19] = '\0';
    table->num_columns++;
    
    printf("\033[32mColumn '%s' added to table '%s' successfully.\033[0m\n", column_name, table_name);
}

void insert_row(Database* db, const char* table_name, char* values[]) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    if (table->num_rows >= MAX_ROWS) {
        printf("\033[31mError: Maximum number of rows reached for table '%s'.\033[0m\n", table_name);
        return;
    }
    
    Row* new_row = &table->rows[table->num_rows];
    for (int i = 0; i < table->num_columns; i++) {
        strncpy(new_row->values[i], values[i], MAX_NAME_LENGTH - 1);
        new_row->values[i][MAX_NAME_LENGTH - 1] = '\0';
    }
    table->num_rows++;
    
    printf("\033[32mRow inserted into table '%s' successfully.\033[0m\n", table_name);
}

void select_all(Database* db, const char* table_name) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    printf("\033[1m\033[36mContents of table '%s':\033[0m\n", table_name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("\033[1m%-20s\033[0m", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        for (int j = 0; j < table->num_columns; j++) {
            printf("%-20s", table->rows[i].values[j]);
        }
        printf("\n");
    }
}

void select_where(Database* db, const char* table_name, const char* column_name, const char* value) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    int column_index = -1;
    for (int i = 0; i < table->num_columns; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            column_index = i;
            break;
        }
    }
    
    if (column_index == -1) {
        printf("\033[31mError: Column '%s' not found in table '%s'.\033[0m\n", column_name, table_name);
        return;
    }
    
    printf("\033[1m\033[36mRows in table '%s' where %s = %s:\033[0m\n", table_name, column_name, value);
    for (int i = 0; i < table->num_columns; i++) {
        printf("\033[1m%-20s\033[0m", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        if (strcmp(table->rows[i].values[column_index], value) == 0) {
            for (int j = 0; j < table->num_columns; j++) {
                printf("%-20s", table->rows[i].values[j]);
            }
            printf("\n");
        }
    }
}

void update_row(Database* db, const char* table_name, int row_id, const char* column_name, const char* new_value) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    int column_index = -1;
    for (int i = 0; i < table->num_columns; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            column_index = i;
            break;
        }
    }
    
    if (column_index == -1) {
        printf("\033[31mError: Column '%s' not found in table '%s'.\033[0m\n", column_name, table_name);
        return;
    }
    
    int row_index = -1;
    for (int i = 0; i < table->num_rows; i++) {
        if (atoi(table->rows[i].values[0]) == row_id) {
            row_index = i;
            break;
        }
    }
    
    if (row_index == -1) {
        printf("\033[31mError: Row with ID %d not found in table '%s'.\033[0m\n", row_id, table_name);
        return;
    }
    
    strncpy(table->rows[row_index].values[column_index], new_value, MAX_NAME_LENGTH - 1);
    table->rows[row_index].values[column_index][MAX_NAME_LENGTH - 1] = '\0';
    
    printf("\033[32mRow with ID %d updated successfully in table '%s'.\033[0m\n", row_id, table_name);
}

void delete_row(Database* db, const char* table_name, int row_id) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    int row_index = -1;
    for (int i = 0; i < table->num_rows; i++) {
        if (atoi(table->rows[i].values[0]) == row_id) {
            row_index = i;
            break;
        }
    }
    
    if (row_index == -1) {
        printf("\033[31mError: Row with ID %d not found in table '%s'.\033[0m\n", row_id, table_name);
        return;
    }
    
    for (int i = row_index; i < table->num_rows - 1; i++) {
        table->rows[i] = table->rows[i + 1];
    }
    table->num_rows--;
    
    printf("\033[32mRow with ID %d deleted successfully from table '%s'.\033[0m\n", row_id, table_name);
}

void save_database(Database* db, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("\033[31mError: Unable to open file '%s' for writing.\033[0m\n", filename);
        return;
    }
    
    fwrite(db, sizeof(Database), 1, file);
    fclose(file);
    
    printf("\033[32mDatabase saved to '%s' successfully.\033[0m\n", filename);
}

void load_database(Database* db, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("\033[31mError: Unable to open file '%s' for reading.\033[0m\n", filename);
        return;
    }
    
    fread(db, sizeof(Database), 1, file);
    fclose(file);
    
    printf("\033[32mDatabase loaded from '%s' successfully.\033[0m\n", filename);
}

void show_tables(Database* db) {
    printf("\033[1m\033[36mTables in the database:\033[0m\n");
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
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    printf("\033[1m\033[36mColumns in table '%s':\033[0m\n", table_name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("- %s (%s)\n", table->columns[i].name, table->columns[i].type);
    }
}

void select_from(Database* db, const char* table_name, char** columns, int num_columns) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    int* column_indices = malloc(num_columns * sizeof(int));

    if (column_indices == NULL) {
        printf("\033[31mError: Memory allocation failed.\033[0m\n");
        return;
    }
    
    for (int i = 0; i < num_columns; i++) {
        column_indices[i] = -1;
        for (int j = 0; j < table->num_columns; j++) {
            if (strcmp(table->columns[j].name, columns[i]) == 0) {
                column_indices[i] = j;
                break;
            }
        }
        if (column_indices[i] == -1) {
            printf("\033[31mError: Column '%s' not found in table '%s'.\033[0m\n", columns[i], table_name);
            free(column_indices);
            return;
        }
    }
    
    printf("\033[1m\033[36mSelected columns from table '%s':\033[0m\n", table_name);
    for (int i = 0; i < num_columns; i++) {
        printf("\033[1m%-20s\033[0m", columns[i]);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            printf("%-20s", table->rows[i].values[column_indices[j]]);
        }
        printf("\n");
    }
    
    free(column_indices);
}

void join_tables(Database* db, const char* table1, const char* table2, const char* join_column) {
    Table* t1 = NULL;
    Table* t2 = NULL;
    
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table1) == 0) {
            t1 = &db->tables[i];
        }
        if (strcmp(db->tables[i].name, table2) == 0) {
            t2 = &db->tables[i];
        }
    }
    
    if (t1 == NULL || t2 == NULL) {
        printf("\033[31mError: One or both tables not found.\033[0m\n");
        return;
    }
    
    int col1 = -1, col2 = -1;
    for (int i = 0; i < t1->num_columns; i++) {
        if (strcmp(t1->columns[i].name, join_column) == 0) {
            col1 = i;
            break;
        }
    }
    for (int i = 0; i < t2->num_columns; i++) {
        if (strcmp(t2->columns[i].name, join_column) == 0) {
            col2 = i;
            break;
        }
    }
    
    if (col1 == -1 || col2 == -1) {
        printf("\033[31mError: Join column not found in one or both tables.\033[0m\n");
        return;
    }
    
    printf("\033[1m\033[36mJoined result of tables '%s' and '%s' on column '%s':\033[0m\n", table1, table2, join_column);
    
    for (int i = 0; i < t1->num_columns; i++) {
        printf("\033[1m%-20s\033[0m", t1->columns[i].name);
    }
    for (int i = 0; i < t2->num_columns; i++) {
        if (i != col2) {
            printf("\033[1m%-20s\033[0m", t2->columns[i].name);
        }
    }
    printf("\n");
    
    for (int i = 0; i < t1->num_rows; i++) {
        for (int j = 0; j < t2->num_rows; j++) {
            if (strcmp(t1->rows[i].values[col1], t2->rows[j].values[col2]) == 0) {
                for (int k = 0; k < t1->num_columns; k++) {
                    printf("%-20s", t1->rows[i].values[k]);
                }
                for (int k = 0; k < t2->num_columns; k++) {
                    if (k != col2) {
                        printf("%-20s", t2->rows[j].values[k]);
                    }
                }
                printf("\n");
            }
        }
    }
}

void insert_into(Database* db, const char* table_name, char** column_names, char** values, int num_columns) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    if (table->num_rows >= MAX_ROWS) {
        printf("\033[31mError: Maximum number of rows reached for table '%s'.\033[0m\n", table_name);
        return;
    }
    
    int* column_indices = malloc(num_columns * sizeof(int));
    if (column_indices == NULL) {
        printf("\033[31mError: Memory allocation failed.\033[0m\n");
        return;
    }
    
    for (int i = 0; i < num_columns; i++) {
        column_indices[i] = -1;
        for (int j = 0; j < table->num_columns; j++) {
            if (strcmp(table->columns[j].name, column_names[i]) == 0) {
                column_indices[i] = j;
                break;
            }
        }
        if (column_indices[i] == -1) {
            printf("\033[31mError: Column '%s' not found in table '%s'.\033[0m\n", column_names[i], table_name);
            free(column_indices);
            return;
        }
    }
    
    Row* new_row = &table->rows[table->num_rows];
    for (int i = 0; i < table->num_columns; i++) {
        strcpy(new_row->values[i], "");  // Initialize all values to empty string
    }
    for (int i = 0; i < num_columns; i++) {
        strncpy(new_row->values[column_indices[i]], values[i], MAX_NAME_LENGTH - 1);
        new_row->values[column_indices[i]][MAX_NAME_LENGTH - 1] = '\0';
    }
    table->num_rows++;
    
    free(column_indices);
    
    printf("\033[32mRow inserted into table '%s' successfully.\033[0m\n", table_name);
}

void drop_table(Database* db, const char* table_name) {
    int table_index = -1;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table_index = i;
            break;
        }
    }
    
    if (table_index == -1) {
        printf("\033[31mError: Table '%s' not found.\033[0m\n", table_name);
        return;
    }
    
    // Shift all tables after the dropped one
    for (int i = table_index; i < db->num_tables - 1; i++) {
        db->tables[i] = db->tables[i + 1];
    }
    
    db->num_tables--;
    printf("\033[32mTable '%s' dropped successfully.\033[0m\n", table_name);
}
