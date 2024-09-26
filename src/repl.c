/*
*  Fichier: btree.c
*  Author : ShHawk
*  Date : 25/09/2024
*  ---------------------------------------------------------------------------------
*   Ce fichier contient les fonctions de manipulation de la structure de données
*   qui est un arbre binaire
*  Les fonctions sont: 
*   - insert_row: qui permet d'insérer une ligne dans l'arbre
*   - select_row: qui permet de selectionner toutes les lignes de l'arbre
*   - save_tree: qui permet de sauvegarder l'arbre dans un fichier
*   - load_tree: qui permet de charger l'arbre depuis un fichier

* ---------------------------------------------------------------------------------
*/#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"
#include "../include/repl.h"
#include "../include/utils.h"

// Déclaration des types de commandes
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_SEARCH,
    STATEMENT_UPDATE,
    STATEMENT_HELP
} StatementType;

// Structure pour stocker une commande
typedef struct {
    StatementType type;
    int id;
    char name[255];
} Statement;

// Fonction pour afficher l'invite de commande avec des couleurs
void print_prompt() {
    printf("\033[36mdb > \033[0m");  // Texte en cyan pour l'invite
}


// Fonction pour lire l'entrée utilisateur
void read_input(char* buffer, size_t buffer_length) {
    if (fgets(buffer, buffer_length, stdin) == NULL) {
        printf("\033[31m✗ Error reading input\033[0m\n");
        exit(EXIT_FAILURE);
    }
    buffer[strlen(buffer) - 1] = '\0';  // Retirer la nouvelle ligne
}

// Fonction pour préparer une commande à partir de l'entrée utilisateur
int prepare_statement(char* buffer, Statement* statement) {
    if (strncmp(buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(buffer, "insert %d %s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;  // Erreur de syntaxe ou données invalides
        }
        return 1;
    } else if (strcmp(buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return 1;
    } else if (strncmp(buffer, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        int args_assigned = sscanf(buffer, "delete %d", &(statement->id));
        if (args_assigned < 1 || statement->id <= 0) {
            return 0;  // Erreur de syntaxe ou ID invalide
        }
        return 1;
    } else if (strncmp(buffer, "search", 6) == 0) {
        statement->type = STATEMENT_SEARCH;
        int args_assigned = sscanf(buffer, "search %d", &(statement->id));
        if (args_assigned < 1 || statement->id <= 0) {
            return 0;  // Erreur de syntaxe ou ID invalide
        }
        return 1;
    } else if (strncmp(buffer, "update", 6) == 0) {
        statement->type = STATEMENT_UPDATE;
        int args_assigned = sscanf(buffer, "update %d %s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strcmp(buffer, "help") == 0) {
        statement->type = STATEMENT_HELP;
        return 1;
    }
    return -1;  // Commande non reconnue
}

// Affiche les commandes disponibles
void print_help() {
    printf("\n\033[36m=== Commandes Disponibles ===\033[0m\n");
    printf("\033[32minsert <id> <name>\033[0m   : Insérer une nouvelle ligne\n");
    printf("\033[32mselect\033[0m              : Afficher toutes les lignes\n");
    printf("\033[32mdelete <id>\033[0m         : Supprimer une ligne avec l'ID\n");
    printf("\033[32msearch <id>\033[0m         : Rechercher une ligne avec l'ID\n");
    printf("\033[32mupdate <id> <name>\033[0m  : Mettre à jour le nom d'une ligne avec l'ID\n");
    printf("\033[32m.exit\033[0m               : Sauvegarder et quitter\n");
    printf("\033[32mhelp\033[0m                : Afficher cette aide\n");
}

// Confirmation de suppression
int confirm_delete(int id) {
    char confirmation[10];
    printf("\033[31mÊtes-vous sûr de vouloir supprimer l'ID %d ? (y/n): \033[0m", id);
    fgets(confirmation, 10, stdin);
    return (confirmation[0] == 'y' || confirmation[0] == 'Y');
}

// Fonction pour exécuter une commande SQL avec des vérifications
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            if (validate_insert(statement->id, statement->name)) {
                insert_row(statement->id, statement->name);
                printf("\033[32m✓ Inserted (%d, %s)\033[0m\n", statement->id, statement->name);  // Texte en vert
            }
            break;

        case (STATEMENT_SELECT):
            printf("\033[34m");              
            select_row();
            printf("\033[0m");
            break;

        case (STATEMENT_DELETE):
            if (confirm_delete(statement->id)) {
                TreeNode* node = search_row(statement->id);
                if (node == NULL) {
                    printf("\033[31m✗ Error: No row found with ID %d to delete.\033[0m\n", statement->id);
                } else {
                    delete_row(statement->id);
                    printf("\033[31m✓ Deleted row with ID %d\033[0m\n", statement->id); 
                }
            } else {
                printf("\033[33mSuppression annulée.\033[0m\n");
            }
            break;

        case (STATEMENT_SEARCH):
            {
                TreeNode* node = search_row(statement->id);
                if (node != NULL) {
                    printf("\033[32m✓ Found: (%d, %s)\033[0m\n", node->id, node->name);  
                } else {
                    printf("\033[31m✗ No row found with ID %d\033[0m\n", statement->id);  
                }
            }
            break;

        case (STATEMENT_UPDATE):
            {
                TreeNode* node = search_row(statement->id);
                if (node != NULL) {
                    strcpy(node->name, statement->name);
                    printf("\033[32m✓ Updated (%d, %s)\033[0m\n", node->id, node->name);
                } else {
                    printf("\033[31m✗ Error: No row found with ID %d to update.\033[0m\n", statement->id);
                }
            }
            break;

        case (STATEMENT_HELP):
            print_help();
            break;
    }
}

// REPL
void repl(void) {
    char buffer[1024];
    Statement statement;
    

    printf("-------------------------------------------------\n"); 
    printf("\033[32mBienvenue dans la base de données !\033[0m\nTapez 'help' pour voir les commandes.\n"); 
    printf("-------------------------------------------------\n");
    
    while (1) {
        print_prompt();
        read_input(buffer, 1024);

        if (strcmp(buffer, ".exit") == 0) {
            printf("Sauvegarde de l'arbre dans un fichier...\n");
            FILE *file = fopen("db_save.txt", "w");
            assert(file != NULL);
            save_tree(file, root);
            fclose(file);
            printf("\033[32m✓ Arbre sauvegardé avec succès.\033[0m\n");
            exit(EXIT_SUCCESS);
        }

        int result = prepare_statement(buffer, &statement);
        if (result == 1) {
            execute_statement(&statement);
        } else if (result == 0) {
            printf("\033[31m✗ Erreur de syntaxe ou données invalides. Impossible de traiter la commande.\033[0m\n");
        } else {
            printf("\033[31m✗ Commande non reconnue : '%s'\033[0m\n", buffer);
        }
    }
}