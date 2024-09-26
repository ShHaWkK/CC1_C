#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"

TreeNode* root = NULL;

/*
*   Fonction create_node permet de créer un nouveau nœud
*/
TreeNode* create_node(int id, char* name) {
    assert(id > 0 && name != NULL);
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->id = id;
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
*   Fonction insert_in_tree permet d'insérer un nœud dans l'arbre  
*/
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

/*
*  Fonction insert_row permet d'insérer une ligne dans l'arbre 
*/
void insert_row(int id, char* name) {
    assert(id > 0 && name != NULL);  
    root = insert_in_tree(root, id, name);
}

// Fonction pour parcourir l'arbre en ordre croissant
void traverse_tree(TreeNode* node) {
    if (node == NULL) return;
    traverse_tree(node->left);
    printf("| %4d | %-20s |\n", node->id, node->name);
    traverse_tree(node->right);
}

/*
* fonction select_row selectionne les lignes. 
*/
void select_row() {
    if (root == NULL) {
        printf("No rows found.\n");
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        traverse_tree(root);
        printf("+------+----------------------+\n");
    }
}

/*
*  Fonction search_row permet de rechercher un nœud dans l'arbre 
*/
TreeNode* search_row(int id) {
    TreeNode* current = root;
    while (current != NULL) {
        if (id == current->id) {
            return current;
        } else if (id < current->id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

/*
 * Fonction find_min permet de trouver le nœud le plus petit dans un sous-arbre
 *  Pour supprimer un nœud avec deux enfants  
 */
TreeNode* find_min(TreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*
* Fonction delete_node permet de supprimer un nœud dans l'arbre
*/

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
        strcpy(root->name, temp->name);
        root->right = delete_node(root->right, temp->id);
    }
    return root;
}

void delete_row(int id) {
    assert(id > 0);
    root = delete_node(root, id);
    printf("\033[32m✓ Deleted row with ID %d\033[0m\n", id);
}

/*
*   Fonction Save_tree permet de sauvegarder l'arbre dans un fichier
*/
void save_tree(FILE *file, TreeNode *node) {
    if (node == NULL) return;
    fprintf(file, "%d %s\n", node->id, node->name);
    save_tree(file, node->left);
    save_tree(file, node->right);
}

/*
* Fonction Load_tree permet de charger l'arbre depuis un fichier
*/
void load_tree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return;

    int id;
    char name[255];

    while (fscanf(file, "%d %s", &id, name) != EOF) {
        insert_row(id, name);
    }

    fclose(file);
}
