/*
*  ---------------------------------------------------------------------------------
*  Fichier: utils.c
*  Author : ShHawk
*  Date : 25/09/2024
*  Description : Sert à valider les entrées utilisateur
*  ---------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include "../include/btree.h"
#include "../include/utils.h"

int validate_insert(int id, char* name) {
    if (id <= 0) {
        printf("\033[31mError: ID must be a positive integer.\033[0m\n");
        return 0;
    }
    if (strlen(name) == 0) {
        printf("\033[31mError: Name cannot be empty.\033[0m\n");
        return 0;
    }
    // Vérification de l'ID
    TreeNode* existing_node = search_row(id);
    if (existing_node != NULL) {
        printf("\033[31mError:  %d already exists.\033[0m\n", id);
        return 0;
    }
    return 1; 
}

// Tableau pour l'historique des commandes
extern char* command_history[];
extern int history_count;

void print_history() {
    printf("\n\033[36m=== Historique des Commandes ===\033[0m\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, command_history[i]);
    }
}
