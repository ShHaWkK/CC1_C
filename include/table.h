#ifndef TABLE_H
#define TABLE_H

#include "btree.h"
#include "database.h"

void create_table(Database* db, const char* table_name);
void insert_into_table(Database* db, const char* table_name, int id, const char* name);
void select_all_from_table(Database* db, const char* table_name);
void select_row_from_table(Database* db, const char* table_name, int id);
void delete_from_table(Database* db, const char* table_name, int id);

// Ces fonctions peuvent rester inchangées
void save_table(Table* table, const char* filename);
void load_table(Table* table, const char* filename);

#endif
