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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"
#include "../include/repl.h"

// Déclaration des types de commandes
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_SEARCH
} StatementType;

// Structure pour stocker une commande
typedef struct {
    StatementType type;
    int id;
    char name[255];
} Statement;

// Fonction pour afficher l'invite de commande avec des couleurs
void print_prompt() {
    printf("\033[33mdb > \033[0m");  // Texte en jaune pour l'invite
}

// Fonction pour lire l'entrée utilisateur
void read_input(char* buffer, size_t buffer_length) {
    if (fgets(buffer, buffer_length, stdin) == NULL) {
        printf("\033[31mError reading input\033[0m\n");
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
    }
    return -1;  // Commande non reconnue
}

// Vérifications avant insertion
int validate_insert(int id, char* name) {
    if (id <= 0) {
        printf("\033[31mError: ID must be a positive integer.\033[0m\n");
        return 0;
    }
    if (strlen(name) == 0) {
        printf("\033[31mError: Name cannot be empty.\033[0m\n");
        return 0;
    }
    TreeNode* existing_node = search_row(id);
    if (existing_node != NULL) {
        printf("\033[31mError: A row with ID %d already exists.\033[0m\n", id);
        return 0;
    }
    return 1;  // Tout est valide
}

// Fonction pour exécuter une commande SQL avec des vérifications
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            if (validate_insert(statement->id, statement->name)) {
                insert_row(statement->id, statement->name);
                printf("\033[32mInserted (%d, %s)\033[0m\n", statement->id, statement->name);  // Texte en vert
            }
            break;

        case (STATEMENT_SELECT):
            printf("\033[34m");  // Texte en bleu pour la sélection
            select_row();
            printf("\033[0m");   // Réinitialiser la couleur
            break;

        case (STATEMENT_DELETE):
            {
                TreeNode* node = search_row(statement->id);
                if (node == NULL) {
                    printf("\033[31mError: No row found with ID %d to delete.\033[0m\n", statement->id);
                } else {
                    delete_row(statement->id);
                    printf("\033[31mDeleted row with ID %d\033[0m\n", statement->id);  // Texte en rouge
                }
            }
            break;

        case (STATEMENT_SEARCH):
            {
                TreeNode* node = search_row(statement->id);
                if (node != NULL) {
                    printf("\033[32mFound: (%d, %s)\033[0m\n", node->id, node->name);  // Texte en vert
                } else {
                    printf("\033[31mNo row found with ID %d\033[0m\n", statement->id);  // Texte en rouge
                }
            }
            break;
    }
}

// Boucle principale du REPL
void repl(void) {
    char buffer[1024];
    Statement statement;
    
    while (1) {
        print_prompt();
        read_input(buffer, 1024);

        if (strcmp(buffer, ".exit") == 0) {
            printf("Saving tree to file...\n");
            FILE *file = fopen("db_save.txt", "w");
            assert(file != NULL);
            save_tree(file, root);
            fclose(file);
            printf("Tree saved successfully.\n");
            exit(EXIT_SUCCESS);
        }

        int result = prepare_statement(buffer, &statement);
        if (result == 1) {
            execute_statement(&statement);
        } else if (result == 0) {
            printf("\033[31mSyntax error or invalid data. Could not parse statement.\033[0m\n");
        } else {
            printf("\033[31mUnrecognized command '%s'\033[0m\n", buffer);
        }
    }
}
