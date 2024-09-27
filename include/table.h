#ifndef TABLE_H
#define TABLE_H

#include "btree.h"

typedef struct Table {
    TreeNode* root;  // Arbre binaire des lignes
    int row_count;   // Nombre total de lignes dans la table
} Table;

// Fonction pour créer une table
Table* create_table();

// Fonction pour insérer une ligne dans la table
void insert_into_table(Table* table, int id, const char* name);

// Fonction pour afficher toutes les lignes
void select_all_from_table(Table* table);

// Fonction pour rechercher une ligne par ID
void select_row_from_table(Table* table, int id);

// Fonction pour supprimer une ligne par ID
void delete_from_table(Table* table, int id);

// Persistance des données
void save_table(Table* table, const char* filename);
void load_table(Table* table, const char* filename);

#endif
