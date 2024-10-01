/*
*   Author:     ShHawk
*   Date:       26/09/2024
*   Fichier :  btree.c
*- ------------------------------------------------------------------------------------------------------------------
*   Explications des fonctions rapidement : 
*       - create_node : Crée un nouveau noeud avec un id et un nom donné
*       - insert_in_tree : Insère un noeud dans l'arbre
*       - find_min : Trouve le noeud le plus petit
*       - delete_node : Supprime un noeud
*       - search_node : Cherche un noeud
*       - traverse_tree : Parcours l'arbre
*       - save_tree : Sauvegarde l'arbre dans un fichier
*       - load_tree : Charge l'arbre depuis un fichier
*       - inorder_traversal : Parcours l'arbre
*---------------------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"
#include "../include/database.h"

TreeNode* g_root = NULL;

TreeNode* create_node(int id, const char* name) {
    assert(id > 0 && name != NULL);

    if (strlen(name) >= MAX_NAME_LENGTH) {
        printf("Error: Name is too long (maximum %d characters allowed).\n", MAX_NAME_LENGTH - 1);
        return NULL;
    }

    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    node->id = id;
    strncpy(node->name, name, MAX_NAME_LENGTH - 1);
    node->name[MAX_NAME_LENGTH - 1] = '\0'; 
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* insert_in_tree(TreeNode* node, int id, const char* name) {
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

TreeNode* find_min(TreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

TreeNode* delete_node(TreeNode* root, int id) {
    if (root == NULL) return root;

    if (id < root->id) {
        root->left = delete_node(root->left, id);
    } else if (id > root->id) {
        root->right = delete_node(root->right, id);
    } else {
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        TreeNode* temp = find_min(root->right);
        root->id = temp->id;
        strncpy(root->name, temp->name, MAX_NAME_LENGTH - 1);
        root->name[MAX_NAME_LENGTH - 1] = '\0';  
        root->right = delete_node(root->right, temp->id);
    }
    return root;
}

TreeNode* search_node(TreeNode* root, int id) {
    if (root == NULL || root->id == id) {
        return root;
    }
    if (id < root->id) {
        return search_node(root->left, id);
    }
    return search_node(root->right, id);
}

void traverse_tree(TreeNode* node) {
    if (node == NULL) return;
    traverse_tree(node->left);
    printf("| %4d | %-20s |\n", node->id, node->name);
    traverse_tree(node->right);
}

void save_tree(FILE *file, TreeNode *node) {
    if (node == NULL) return;
    fprintf(file, "%d %s\n", node->id, node->name);
    save_tree(file, node->left);
    save_tree(file, node->right);
}

TreeNode* load_tree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    TreeNode* root = NULL;
    int id;
    char name[MAX_NAME_LENGTH];

    while (fscanf(file, "%d %s", &id, name) != EOF) {
        root = insert_in_tree(root, id, name);
    }

    fclose(file);
    return root;
}

void inorder_traversal(TreeNode* root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        printf("| %4d | %-20s |\n", root->id, root->name);
        inorder_traversal(root->right);
    }
}
