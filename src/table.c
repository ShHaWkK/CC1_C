#include <stdio.h>
#include <stdlib.h>
#include "../include/btree.h"
#include "../include/table.h"

/* Crée une nouvelle table */
Table* create_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        printf("Erreur d'allocation mémoire pour la table.\n");
        return NULL;
    }
    table->root = NULL;
    table->row_count = 0;
    return table;
}

/* Insère une ligne dans la table */
void insert_into_table(Table* table, int id, const char* name) {
    if (search_row(id) != NULL) {
        printf("Erreur : L'ID %d existe déjà.\n", id);
        return;
    }
    table->root = insert_in_tree(table->root, id, name);
    table->row_count++;
    printf("Ligne insérée avec succès : ID = %d, Nom = %s\n", id, name);
}

/* Sélectionne toutes les lignes */
void select_all_from_table(Table* table) {
    if (table->root == NULL) {
        printf("Aucune ligne trouvée.\n");
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        traverse_tree(table->root);
        printf("+------+----------------------+\n");
    }
}

/* Sélectionne une ligne par ID */
void select_row_from_table(__attribute__((unused)) Table* table, int id) {
    TreeNode* node = search_row(id);
    if (node == NULL) {
        printf("Aucune ligne trouvée avec l'ID %d.\n", id);
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        printf("| %4d | %-20s |\n", node->id, node->name);
        printf("+------+----------------------+\n");
    }
}

/* Supprime une ligne par ID */
void delete_from_table(Table* table, int id) {
    if (search_row(id) == NULL) {
        printf("Erreur : Aucun ID %d trouvé pour la suppression.\n", id);
        return;
    }
    table->root = delete_node(table->root, id);
    table->row_count--;
    printf("Ligne avec ID %d supprimée avec succès.\n", id);
}

/* Sauvegarde la table dans un fichier binaire */
void save_table(Table* table, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }
    fwrite(&(table->row_count), sizeof(int), 1, file);  // Sauvegarder le nombre de lignes
    save_tree(file, table->root);  // Sauvegarder l'arbre binaire
    fclose(file);
    printf("Table sauvegardée dans %s.\n", filename);
}

/* Charge la table depuis un fichier binaire */
void load_table(Table* table, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour le chargement.\n");
        return;
    }

    fread(&(table->row_count), sizeof(int), 1, file);  // Charger le nombre de lignes
    fclose(file);  // Fermer le fichier après lecture du nombre de lignes

    load_tree(filename);  // Passer le nom du fichier pour charger l'arbre binaire
    printf("Table chargée depuis %s.\n", filename);
}
