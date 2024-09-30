#include "../include/table.h"
#include "../include/database.h"
#include "../include/btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insert_into_table(Database* db, const char* table_name, int id, const char* name) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    
    TreeNode* existing_node = search_node(table->root, id);
    if (existing_node != NULL) {
        printf("Error: ID %d already exists in table '%s'.\n", id, table_name);
        return;
    }
    
    table->root = insert_in_tree(table->root, id, name);
    table->num_rows++;
    printf("Row inserted successfully: ID = %d, Name = %s in table '%s'\n", id, name, table_name);
}

void select_all_from_table(Database* db, const char* table_name) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    
    if (table->root == NULL) {
        printf("No rows found in table '%s'.\n", table_name);
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        inorder_traversal(table->root);
        printf("+------+----------------------+\n");
    }
}

void select_row_from_table(Database* db, const char* table_name, int id) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    
    TreeNode* node = search_node(table->root, id);
    if (node == NULL) {
        printf("No row found with ID %d in table '%s'.\n", id, table_name);
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        printf("| %4d | %-20s |\n", node->id, node->name);
        printf("+------+----------------------+\n");
    }
}

void delete_from_table(Database* db, const char* table_name, int id) {
    Table* table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    
    if (table == NULL) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    
    if (search_node(table->root, id) == NULL) {
        printf("Error: No ID %d found for deletion in table '%s'.\n", id, table_name);
        return;
    }
    
    table->root = delete_node(table->root, id);
    table->num_rows--;
    printf("Row with ID %d deleted successfully from table '%s'.\n", id, table_name);
}

void save_table(Table* table, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }
    fwrite(&(table->num_rows), sizeof(int), 1, file);
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

    fread(&(table->num_rows), sizeof(int), 1, file);
    fclose(file);

    table->root = NULL;  // Reset the root before loading
    table->root = load_tree(filename);
    printf("Table loaded from %s successfully.\n", filename);
}
