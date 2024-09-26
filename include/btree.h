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

typedef struct TreeNode {
    int id;
    char name[255];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

extern TreeNode* root;

void insert_row(int id, char* name);
void select_row(void);
TreeNode* search_row(int id);
void delete_row(int id);
void save_tree(FILE* file, TreeNode* node);
void load_tree(const char* filename);
TreeNode* find_min(TreeNode* node);
TreeNode* delete_node(TreeNode* root, int id);

#endif
