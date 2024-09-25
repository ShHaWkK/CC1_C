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
#include "../include/btree.h"
#include "../include/repl.h"

// Enumération pour le résultat des commandes
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// Enumération pour les résultats de la préparation des commandes
typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR
} PrepareResult;

// Enumération pour les types de commandes SQL
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,  
    STATEMENT_SEARCH   
} StatementType;

// Pour représenter la commande
typedef struct {
    StatementType type;
    int id;
    char name[255];
} Statement;

// gérer l'entrée utilisateur dans le REPL
typedef struct {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

// Fonction pour afficher l'invite de commande avec des couleurs
void print_prompt() {
    printf("\033[33mdb > \033[0m");
}

// Fonction pour créer et initialiser un buffer d'entrée
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = (char*)malloc(1024 * sizeof(char)); 
    input_buffer->buffer_length = 1024;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Fonction pour lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    if (fgets(input_buffer->buffer, input_buffer->buffer_length, stdin) == NULL) {
        printf("\033[31mError reading input\033[0m\n");  
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = strlen(input_buffer->buffer) - 1;  
    input_buffer->buffer[input_buffer->input_length] = '\0';  
}

// Fonction pour fermer le buffer d'entrée et libérer la mémoire
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Fonction pour préparer une commande à partir de l'entrée utilisateur
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s", &(statement->id), statement->name);
        if (args_assigned < 2) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }

    // Reconnaître la commande delete
    if (strncmp(input_buffer->buffer, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        int args_assigned = sscanf(input_buffer->buffer, "delete %d", &(statement->id));
        if (args_assigned < 1) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }

    // Reconnaître la commande search
    if (strncmp(input_buffer->buffer, "search", 6) == 0) {
        statement->type = STATEMENT_SEARCH;
        int args_assigned = sscanf(input_buffer->buffer, "search %d", &(statement->id));
        if (args_assigned < 1) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }

    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Fonction pour exécuter une commande SQL avec des couleurs
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            insert_row(statement->id, statement->name);
            printf("\033[32mInserted (%d, %s)\033[0m\n", statement->id, statement->name); 
            break;

        case (STATEMENT_SELECT):
            printf("\033[34m");  
            select_row();
            printf("\033[0m");   
            break;

        case (STATEMENT_DELETE):
            delete_row(statement->id);
            printf("\033[31mDeleted row with ID %d\033[0m\n", statement->id);  
            break;

        case (STATEMENT_SEARCH):
            {
                TreeNode* node = search_row(statement->id);
                if (node != NULL) {
                    printf("\033[32mFound: (%d, %s)\033[0m\n", node->id, node->name);  
                } else {
                    printf("\033[31mNo row found with ID %d\033[0m\n", statement->id);  
                }
            }
            break;
    }
}

// Boucle principale du REPL avec des couleurs
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    
    while (1) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            if (strcmp(input_buffer->buffer, ".exit") == 0) {
                FILE *file = fopen("db_save.txt", "w");
                if (file != NULL) {
                    save_tree(file, root);
                    fclose(file);
                }
                close_input_buffer(input_buffer);
                exit(EXIT_SUCCESS);
            } else {
                printf("\033[31mUnrecognized command '%s'\033[0m\n", input_buffer->buffer);  
            }
        } else {
            Statement statement;
            switch (prepare_statement(input_buffer, &statement)) {
                case PREPARE_SUCCESS:
                    execute_statement(&statement);
                    break;
                case PREPARE_SYNTAX_ERROR:
                    printf("\033[31mSyntax error. Could not parse statement.\033[0m\n"); 
                    break;
                case PREPARE_UNRECOGNIZED_STATEMENT:
                    printf("\033[31mUnrecognized keyword at start of '%s'.\033[0m\n", input_buffer->buffer);
                    break;
            }
        }
    }
}
