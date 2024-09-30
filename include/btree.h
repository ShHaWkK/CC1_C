// btree.h
#ifndef BTREE_H
#define BTREE_H

#include "database.h"

extern TreeNode* g_root;

// Fonctions de base de l'arbre binaire
TreeNode* create_node(int id, const char* name);
TreeNode* insert_in_tree(TreeNode* node, int id, const char* name);
TreeNode* find_min(TreeNode* node);
TreeNode* delete_node(TreeNode* root, int id);
TreeNode* search_node(TreeNode* root, int id);
void traverse_tree(TreeNode* node);

// Fonctions de sauvegarde et chargement
void save_tree(FILE *file, TreeNode *node);
TreeNode* load_tree(const char *filename);

// Fonction d'aide pour l'affichage
void inorder_traversal(TreeNode* root);

#endif // BTREE_H
