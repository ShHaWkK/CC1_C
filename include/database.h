// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 255
#define MAX_COLUMNS 10
#define MAX_ROWS 100
#define MAX_TABLES 10

typedef struct TreeNode {
    int id;
    char name[MAX_NAME_LENGTH];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char type[20];
} Column;

typedef struct {
    char values[MAX_COLUMNS][MAX_NAME_LENGTH];
} Row;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Column columns[MAX_COLUMNS];
    Row rows[MAX_ROWS];
    int num_columns;
    int num_rows;
    TreeNode* root; 
} Table;


typedef struct {
    Table tables[MAX_TABLES];
    int num_tables;
} Database;

void init_database(Database* db);
void create_table(Database* db, const char* table_name);
void add_column(Database* db, const char* table_name, const char* column_name, const char* column_type);
void insert_row(Database* db, const char* table_name, char* values[]);
void select_all(Database* db, const char* table_name);
void select_where(Database* db, const char* table_name, const char* column_name, const char* value);
void update_row(Database* db, const char* table_name, int row_id, const char* column_name, const char* new_value);
void delete_row(Database* db, const char* table_name, int row_id);
void save_database(Database* db, const char* filename);
void load_database(Database* db, const char* filename);
void show_tables(Database* db);
void show_columns(Database* db, const char* table_name);
void select_from(Database* db, const char* table_name, char** columns, int num_columns);
void join_tables(Database* db, const char* table1, const char* table2, const char* join_column);
void insert_into(Database* db, const char* table_name, char** column_names, char** values, int num_columns);
void drop_table(Database* db, const char* table_name);

#endif
