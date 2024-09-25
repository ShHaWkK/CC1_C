/*
*  Fichier: btree.c
*  ---------------------------------------------------------------------------------
*   Ce fichier contient les fonctions de manipulation de la structure de données
*   qui est un arbre binaire
*  Les fonctions sont: 
*   - insert_row: qui permet d'insérer une ligne dans l'arbre
*   - select_row: qui permet de selectionner toutes les lignes de l'arbre
* ---------------------------------------------------------------------------------
*/

#include "../include/btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    int id;
    char name[255];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* root = NULL;  // Racine de l'arbre binaire

TreeNode* create_node(int id, char* name) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->id = id;
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* insert_in_tree(TreeNode* node, int id, char* name) {
    if (node == NULL) {
        return create_node(id, name);
    }

    if (id < node->id) {
        node->left = insert_in_tree(node->left, id, name);
    } else if (id > node->id) {
        node->right = insert_in_tree(node->right, id, name);
    }

    return node;
}

void insert_row(int id, char* name) {
    root = insert_in_tree(root, id, name);
    printf("Inserted (%d, %s)\n", id, name);
}

void traverse_tree(TreeNode* node) {
    if (node == NULL) return;

    traverse_tree(node->left);
    printf("(%d, %s)\n", node->id, node->name);
    traverse_tree(node->right);
}

void select_row() {
    if (root == NULL) {
        printf("No rows found.\n");
    } else {
        traverse_tree(root);
    }
}
