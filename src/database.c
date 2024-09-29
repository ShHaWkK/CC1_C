#include "../include/database.h"

void init_database(Database* db) {
    db->num_tables = 0;
}

void create_table(Database* db, const char* table_name) {
    if (db->num_tables >= MAX_TABLES) {
        printf("Maximum number of tables reached.\n");
        return;
    }
    
    Table* new_table = &db->tables[db->num_tables];
    strncpy(new_table->name, table_name, MAX_NAME_LENGTH - 1);
    new_table->name[MAX_NAME_LENGTH - 1] = '\0';
    new_table->num_columns = 0;
    new_table->num_rows = 0;
    db->num_tables++;
    
    printf("Table '%s' created successfully.\n", table_name);
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    if (table->num_columns >= MAX_COLUMNS) {
        printf("Maximum number of columns reached for table '%s'.\n", table_name);
        return;
    }
    
    Column* new_column = &table->columns[table->num_columns];
    strncpy(new_column->name, column_name, MAX_NAME_LENGTH - 1);
    new_column->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(new_column->type, column_type, 19);
    new_column->type[19] = '\0';
    table->num_columns++;
    
    printf("Column '%s' added to table '%s' successfully.\n", column_name, table_name);
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    if (table->num_rows >= MAX_ROWS) {
        printf("Maximum number of rows reached for table '%s'.\n", table_name);
        return;
    }
    
    Row* new_row = &table->rows[table->num_rows];
    for (int i = 0; i < table->num_columns; i++) {
        strncpy(new_row->values[i], values[i], MAX_NAME_LENGTH - 1);
        new_row->values[i][MAX_NAME_LENGTH - 1] = '\0';
    }
    table->num_rows++;
    
    printf("Row inserted into table '%s' successfully.\n", table_name);
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    printf("Contents of table '%s':\n", table_name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("%-15s", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        for (int j = 0; j < table->num_columns; j++) {
            printf("%-15s", table->rows[i].values[j]);
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
        printf("Table '%s' not found.\n", table_name);
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
        printf("Column '%s' not found in table '%s'.\n", column_name, table_name);
        return;
    }
    
    printf("Matching rows in table '%s':\n", table_name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("%-15s", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        if (strcmp(table->rows[i].values[column_index], value) == 0) {
            for (int j = 0; j < table->num_columns; j++) {
                printf("%-15s", table->rows[i].values[j]);
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    if (row_id < 0 || row_id >= table->num_rows) {
        printf("Invalid row ID.\n");
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
        printf("Column '%s' not found in table '%s'.\n", column_name, table_name);
        return;
    }
    
    strncpy(table->rows[row_id].values[column_index], new_value, MAX_NAME_LENGTH - 1);
    table->rows[row_id].values[column_index][MAX_NAME_LENGTH - 1] = '\0';
    printf("Row updated successfully.\n");
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    if (row_id < 0 || row_id >= table->num_rows) {
        printf("Invalid row ID.\n");
        return;
    }
    
    for (int i = row_id; i < table->num_rows - 1; i++) {
        memcpy(&table->rows[i], &table->rows[i + 1], sizeof(Row));
    }
    table->num_rows--;
    
    printf("Row deleted successfully.\n");
}

void save_database(Database* db, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    
    fwrite(db, sizeof(Database), 1, file);
    fclose(file);
    
    printf("Database saved to file '%s' successfully.\n", filename);
}

void load_database(Database* db, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
    
    fread(db, sizeof(Database), 1, file);
    fclose(file);
    
    printf("Database loaded from file '%s' successfully.\n", filename);
}

void show_tables(Database* db) {
    printf("Tables in the database:\n");
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    printf("Columns in table '%s':\n", table_name);
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
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    
    int* column_indices = malloc(num_columns * sizeof(int));
    for (int i = 0; i < num_columns; i++) {
        column_indices[i] = -1;
        for (int j = 0; j < table->num_columns; j++) {
            if (strcmp(table->columns[j].name, columns[i]) == 0) {
                column_indices[i] = j;
                break;
            }
        }
        if (column_indices[i] == -1) {
            printf("Column '%s' not found in table '%s'.\n", columns[i], table_name);
            free(column_indices);
            return;
        }
    }
    
    for (int i = 0; i < num_columns; i++) {
        printf("%-15s", columns[i]);
    }
    printf("\n");
    
    for (int i = 0; i < table->num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            printf("%-15s", table->rows[i].values[column_indices[j]]);
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
        printf("One or both tables not found.\n");
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
        printf("Join column not found in one or both tables.\n");
        return;
    }
    
    for (int i = 0; i < t1->num_columns; i++) {
        printf("%-15s", t1->columns[i].name);
    }
    for (int i = 0; i < t2->num_columns; i++) {
        if (i != col2) {
            printf("%-15s", t2->columns[i].name);
        }
    }
    printf("\n");
    
    for (int i = 0; i < t1->num_rows; i++) {
        for (int j = 0; j < t2->num_rows; j++) {
            if (strcmp(t1->rows[i].values[col1], t2->rows[j].values[col2]) == 0) {
                for (int k = 0; k < t1->num_columns; k++) {
                    printf("%-15s", t1->rows[i].values[k]);
                }
                for (int k = 0; k < t2->num_columns; k++) {
                    if (k != col2) {
                        printf("%-15s", t2->rows[j].values[k]);
                    }
                }
                printf("\n");
            }
        }
    }
}