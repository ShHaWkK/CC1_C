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
#include <stdbool.h>
#include "../include/btree.h"

// Enumération pour les résultats des commandes
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
    STATEMENT_SELECT
} StatementType;

// Structure pour représenter une commande (insert/select)
typedef struct {
    StatementType type;
    int id;
    char name[255];
} Statement;

// Structure pour gérer l'entrée utilisateur dans le REPL
typedef struct {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

// Fonction pour créer et initialiser un buffer d'entrée
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = (char*)malloc(1024 * sizeof(char));  // Taille du buffer à 1024 octets
    input_buffer->buffer_length = 1024;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Fonction pour afficher l'invite de commande
void print_prompt() {
    printf("db > ");
}

// Fonction pour lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    if (fgets(input_buffer->buffer, input_buffer->buffer_length, stdin) == NULL) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = strlen(input_buffer->buffer) - 1;  // Retirer la nouvelle ligne
    input_buffer->buffer[input_buffer->input_length] = '\0';  // Terminer la chaîne de caractères
}

// Fonction pour fermer le buffer d'entrée et libérer la mémoire
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Fonction pour gérer les commandes méta (comme .exit)
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        // Sauvegarde de l'arbre binaire dans un fichier
        FILE *file = fopen("db_save.txt", "w");
        if (file == NULL) {
            printf("Error opening file for saving data.\n");
            return META_COMMAND_UNRECOGNIZED_COMMAND;
        }

        save_tree(file, root);  // Sauvegarder les données de l'arbre dans le fichier
        fclose(file);

        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

// Fonction pour préparer une commande à partir de l'entrée utilisateur
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        // Extraire l'ID et le nom de la commande
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s", &(statement->id), statement->name);
        if (args_assigned < 2) {
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

// Fonction pour exécuter une commande SQL (insert ou select)
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
        // Insérer dans l'arbre binaire
            insert_row(statement->id, statement->name);
            break;
        case (STATEMENT_SELECT):
            select_row();
            break;
    }
}

// Boucle principale du REPL (Read-Eval-Print Loop)
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    
    while (true) {
        print_prompt();  // Afficher l'invite de commande
        read_input(input_buffer);  // Lire l'entrée utilisateur

        // Vérifier si l'utilisateur a entré une commande méta
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        // Préparer la commande SQL
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        // Exécuter la commande préparée
        execute_statement(&statement);
        printf("Executed.\n");
    }
}
