#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"

TreeNode* root = NULL;  // Définition de la racine de l'arbre binaire

// Fonction pour créer un nouveau nœud
TreeNode* create_node(int id, char* name) {
    assert(id > 0 && name != NULL);  // Vérification des entrées
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->id = id;
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Fonction pour insérer un nœud dans l'arbre binaire
TreeNode* insert_in_tree(TreeNode* node, int id, char* name) {
    if (node == NULL) {
        return create_node(id, name);  // Créer un nouveau nœud si nécessaire
    }
    if (id < node->id) {
        node->left = insert_in_tree(node->left, id, name);  // Insérer à gauche
    } else if (id > node->id) {
        node->right = insert_in_tree(node->right, id, name);  // Insérer à droite
    }
    return node;
}

// Interface pour insérer une ligne dans l'arbre
void insert_row(int id, char* name) {
    assert(id > 0 && name != NULL);  // Vérification des paramètres
    root = insert_in_tree(root, id, name);
}

// Fonction pour parcourir l'arbre en ordre croissant
void traverse_tree(TreeNode* node) {
    if (node == NULL) return;
    traverse_tree(node->left);
    printf("(%d, %s)\n", node->id, node->name);
    traverse_tree(node->right);
}

// Interface pour afficher les lignes
void select_row() {
    if (root == NULL) {
        printf("No rows found.\n");
    } else {
        traverse_tree(root);
    }
}

// Fonction pour rechercher un nœud par ID
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
    return NULL;  // Si le nœud n'est pas trouvé
}

// Fonction pour supprimer un nœud (à implémenter selon les besoins)
void delete_row(int id) {
    assert(id > 0);  // Vérification que l'ID est valide
    printf("Deleted row with ID %d\n", id);  // Suppression simulée
}

// Fonction pour sauvegarder l'arbre binaire dans un fichier
void save_tree(FILE *file, TreeNode *node) {
    if (node == NULL) return;
    fprintf(file, "%d %s\n", node->id, node->name);  // Sauvegarder chaque nœud
    save_tree(file, node->left);
    save_tree(file, node->right);
}

// Fonction pour charger l'arbre depuis un fichier
void load_tree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return;

    int id;
    char name[255];

    while (fscanf(file, "%d %s", &id, name) != EOF) {
        insert_row(id, name);  // Insérer les données dans l'arbre
    }

    fclose(file);
}
