// path : src/include/btree.h

/*
* ---------------------------------------------------------------------------------
* Ce fichier contient les fonctions de manipulation de la structure de données
* qui est un arbre binaire
* Les fonctions sont:
* - insert_row: qui permet d'insérer une ligne dans l'arbre
* - select_row: qui permet de selectionner toutes les lignes de l'arbre
* - save_tree: qui permet de sauvegarder l'arbre dans un fichier
* - load_tree: qui permet de charger l'arbre depuis un fichier
* ---------------------------------------------------------------------------------
*
*/
#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>

#define MAX_NAME_LENGTH 255

typedef struct TreeNode {
    int id;
    char name[MAX_NAME_LENGTH];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

extern TreeNode* root;

TreeNode* create_node(int id, const char* name);
TreeNode* insert_in_tree(TreeNode* root, int id, const char* name);
TreeNode* find_min(TreeNode* node);
TreeNode* delete_node(TreeNode* root, int id);
TreeNode* search_row(int id);

void insert_row(int id, const char* name);  // Assurez-vous que cette déclaration est présente
void select_row_by_id(int id);
void traverse_tree(TreeNode* node);
void select_row(void);
void delete_row(int id);
void update_row(int id, const char* new_name);
void save_tree(FILE* file, TreeNode* node);
void load_tree(const char* filename);

#endif
