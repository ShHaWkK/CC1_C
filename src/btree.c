#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"

#define MAX_NAME_LENGTH 255  // Limite de taille pour le nom

TreeNode* root = NULL;

/*
* Fonction create_node permet de créer un nouveau nœud avec validation de l'entrée
*/
TreeNode* create_node(int id, const char* name) {
    assert(id > 0 && name != NULL);

    // Vérifier la longueur du nom pour éviter les dépassements de tampon
    if (strlen(name) >= MAX_NAME_LENGTH) {
        printf("\033[31mError: Name is too long (maximum %d characters allowed).\033[0m\n", MAX_NAME_LENGTH - 1);
        return NULL;
    }

    // Allouer dynamiquement un nouveau nœud
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("\033[31mError: Memory allocation failed.\033[0m\n");
        return NULL;
    }

    node->id = id;

    // Utiliser strncpy pour copier en toute sécurité la chaîne dans le champ 'name'
    strncpy(node->name, name, MAX_NAME_LENGTH - 1);
    node->name[MAX_NAME_LENGTH - 1] = '\0';  // Assurez-vous que la chaîne est terminée par un caractère nul

    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
* Fonction insert_in_tree permet d'insérer un nœud dans l'arbre
*/
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

/*
* Fonction insert_row permet d'insérer une ligne dans l'arbre avec vérification
*/
void insert_row(int id, const char* name) {
    assert(id > 0 && name != NULL);

    // Vérifier si la chaîne de caractères dépasse la taille maximale
    if (strlen(name) >= MAX_NAME_LENGTH) {
        printf("\033[31mError: Name is too long (maximum %d characters allowed).\033[0m\n", MAX_NAME_LENGTH - 1);
        return;
    }

    root = insert_in_tree(root, id, name);
}

/*
* Fonction select_row_by_id permet d'afficher une ligne par ID
*/
void select_row_by_id(int id) {
    TreeNode* node = search_row(id);
    if (node == NULL) {
        printf("No row found with ID %d.\n", id);
    } else {
        printf("+------+----------------------+\n");
        printf("|  ID  | Name                 |\n");
        printf("+------+----------------------+\n");
        printf("| %4d | %-20s |\n", node->id, node->name);
        printf("+------+----------------------+\n");
    }
}

/*
* Fonction traverse_tree permet de parcourir l'arbre en ordre croissant
*/
void traverse_tree(TreeNode* node) {
    if (node == NULL) return;
    traverse_tree(node->left);
    printf("| %4d | %-20s |\n", node->id, node->name);
    traverse_tree(node->right);
}

/*
* Fonction select_row permet d'afficher toutes les lignes de l'arbre
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
* Fonction search_row permet de rechercher un nœud dans l'arbre
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
        strncpy(root->name, temp->name, MAX_NAME_LENGTH - 1);
        root->name[MAX_NAME_LENGTH - 1] = '\0';  // Assurez-vous que la chaîne est terminée par un caractère nul
        root->right = delete_node(root->right, temp->id);
    }
    return root;
}

/*
* Fonction delete_row permet de supprimer une ligne par ID
*/
void delete_row(int id) {
    assert(id > 0);
    root = delete_node(root, id);
    printf("\033[32m✓ Deleted row with ID %d\033[0m\n", id);
}

/*
* Fonction update_row permet de mettre à jour une ligne par ID
*/
void update_row(int id, const char* new_name) {
    TreeNode* node = search_row(id);
    if (node == NULL) {
        printf("No row found with ID %d.\n", id);
    } else {
        strncpy(node->name, new_name, MAX_NAME_LENGTH - 1);
        node->name[MAX_NAME_LENGTH - 1] = '\0';  // Assurez-vous que la chaîne est terminée par un caractère nul
        printf("\033[32m✓ Updated row with ID %d to name %s\033[0m\n", id, new_name);
    }
}

/*
* Fonction save_tree permet de sauvegarder l'arbre dans un fichier
*/
void save_tree(FILE *file, TreeNode *node) {
    if (node == NULL) return;
    fprintf(file, "%d %s\n", node->id, node->name);
    save_tree(file, node->left);
    save_tree(file, node->right);
}

/*
* Fonction load_tree permet de charger l'arbre depuis un fichier
*/
void load_tree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return;

    int id;
    char name[MAX_NAME_LENGTH];

    while (fscanf(file, "%d %s", &id, name) != EOF) {
        insert_row(id, name);
    }

    fclose(file);
}
