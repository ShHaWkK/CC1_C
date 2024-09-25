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

// Déclaration de la structure TreeNode
typedef struct TreeNode {
    int id;
    char name[255];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Prototypes des fonctions pour manipuler l'arbre binaire
void insert_row(int id, char* name);
void delete_row(int id);
TreeNode* search_row(int id);
void select_row();
void save_tree(FILE *file, TreeNode *node);
void load_tree(const char *filename);

// Déclaration de la racine de l'arbre binaire (externe)
extern TreeNode* root;

#endif  // BTREE_H
