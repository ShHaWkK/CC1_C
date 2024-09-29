#include "../include/table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table* create_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        printf("Error: Memory allocation failed for table.\n");
        return NULL;
    }
    table->root = NULL;
    table->row_count = 0;
    return table;
}

void insert_into_table(Table* table, int id, const char* name) {
    if (search_row(id) != NULL) {
        printf("Error: ID %d already exists.\n", id);
        return;
    }
    table->root = insert_in_tree(table->root, id, name);
    table->row_count++;
    printf("Row inserted successfully: ID = %d, Name = %s\n", id, name);
}

void select_all_from_table(Table* table) {
    if (table->root == NULL) {
        printf("No rows found.\n");
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        traverse_tree(table->root);
        printf("+------+----------------------+\n");
    }
}

void select_row_from_table(Table* table, int id) {
    TreeNode* node = search_row(id);
    if (node == NULL) {
        printf("No row found with ID %d.\n", id);
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        printf("| %4d | %-20s |\n", node->id, node->name);
        printf("+------+----------------------+\n");
    }
}

void delete_from_table(Table* table, int id) {
    if (search_row(id) == NULL) {
        printf("Error: No ID %d found for deletion.\n", id);
        return;
    }
    table->root = delete_node(table->root, id);
    table->row_count--;
    printf("Row with ID %d deleted successfully.\n", id);
}

void save_table(Table* table, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }
    fwrite(&(table->row_count), sizeof(int), 1, file);
    save_tree(file, table->root);
    fclose(file);
    printf("Table saved to %s successfully.\n", filename);
}

void load_table(Table* table, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for loading.\n");
        return;
    }

    fread(&(table->row_count), sizeof(int), 1, file);
    fclose(file);

    load_tree(filename);
    printf("Table loaded from %s successfully.\n", filename);
}
