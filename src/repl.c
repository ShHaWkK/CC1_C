/*
*  Fichier: repl.c
*  Author : ShHawk
*  Date : 26/09/2024
* ---------------------------------------------------------------------------------
*  Description : Sert à lire les commandes de l'utilisateur et les exécuter.
*  Fonctions importantes :
*  - print_prompt : Affiche le prompt
*  - read_input : Lit l'entrée utilisateur
*  - prepare_statement : Prépare une commande
*  - print_help : Affiche les commandes disponibles
*  - execute_statement : Exécute une commande
*  - repl : Fonction principale pour lire les commandes
*  - store_command : Stocke les commandes dans un historique
*  - print_history : Affiche l'historique des commandes
* ---------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"
#include "../include/utils.h"

#define MAX_HISTORY 100

// Structure pour stocker une commande
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_SEARCH,
    STATEMENT_UPDATE,
    STATEMENT_HELP,
    STATEMENT_HISTORY
} StatementType;

typedef struct {
    StatementType type;
    int id;
    char name[255];
    int where_id;
    int has_where;
} Statement;

// Tableau pour l'historique des commandes
char* command_history[MAX_HISTORY];
int history_count = 0;

/*
* Fonction pour stocker chaque commande dans l'historique
*/
void store_command(const char* command) {
    if (history_count < MAX_HISTORY) {
        command_history[history_count] = (char*)malloc(strlen(command) + 1);  // Allouer de la mémoire pour la commande
        strcpy(command_history[history_count], command);  // Copier la commande
        history_count++;
    }
}

/*
* Affiche le prompt
*/
void print_prompt() {
    printf("\033[36mdb_alex > \033[0m");
}

// Fonction pour lire l'entrée utilisateur
void read_input(char* buffer, size_t buffer_length) {
    if (fgets(buffer, buffer_length, stdin) == NULL) {
        printf("\033[31m✗ Error reading input\033[0m\n");
        exit(EXIT_FAILURE);
    }
    buffer[strlen(buffer) - 1] = '\0';
}

/*
*  Fonction prepare_statement pour préparer une commande avec WHERE
*/
int prepare_statement(char* buffer, Statement* statement) {
    statement->has_where = 0;

    if (strncmp(buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(buffer, "insert %d %s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "select", 6) == 0) {
        if (strstr(buffer, "where id =")) {
            statement->type = STATEMENT_SELECT;
            int args_assigned = sscanf(buffer, "select where id = %d", &(statement->where_id));
            if (args_assigned == 1) {
                statement->has_where = 1;
                return 1;
            }
        } else {
            statement->type = STATEMENT_SELECT;
            return 1;
        }
    } else if (strncmp(buffer, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        int args_assigned = sscanf(buffer, "delete %d", &(statement->id));
        if (args_assigned < 1 || statement->id <= 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "search", 6) == 0) {
        statement->type = STATEMENT_SEARCH;
        int args_assigned = sscanf(buffer, "search %d", &(statement->id));
        if (args_assigned < 1 || statement->id <= 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "update", 6) == 0) {
        statement->type = STATEMENT_UPDATE;
        int args_assigned = sscanf(buffer, "update %d %s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strcmp(buffer, "history") == 0) {
        statement->type = STATEMENT_HISTORY;
        return 1;
    } else if (strcmp(buffer, "help") == 0) {
        statement->type = STATEMENT_HELP;
        return 1;
    }

    return -1;
}

/*
*   Fonction Help affiche les commandes disponibles.
*/
void print_help() {
    printf("\n\033[36m=== Commandes Disponibles ===\033[0m\n");
    printf("\033[32minsert <id> <name>\033[0m   : Insérer une nouvelle ligne\n");
    printf("\033[32mselect\033[0m              : Afficher toutes les lignes\n");
    printf("\033[32mselect where id = <id>\033[0m : Sélectionner une ligne avec un id spécifique\n");
    printf("\033[32msearch <id>\033[0m         : Rechercher une ligne avec un id spécifique\n");
    printf("\033[32mdelete <id>\033[0m         : Supprimer une ligne avec l'ID\n");
    printf("\033[32mupdate <id> <name>\033[0m  : Mettre à jour le nom d'une ligne avec l'ID\n");
    printf("\033[32mhistory\033[0m             : Afficher l'historique des commandes\n");
    printf("\033[32m.exit\033[0m               : Sauvegarder et quitter\n");
    printf("\033[32mhelp\033[0m                : Afficher cette aide\n");
}

// Confirmation de suppression
int confirm_delete(int id) {
    char confirmation[10];
    printf("\033[31mEtes-vous sûr de vouloir supprimer l'ID %d ? (y/n): \033[0m", id);
    fgets(confirmation, 10, stdin);
    return (confirmation[0] == 'y' || confirmation[0] == 'Y');
}

/*
* Fonction pour afficher l'historique des commandes
*/
void print_history() {
    printf("\n\033[36m=== Historique des Commandes ===\033[0m\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, command_history[i]);
    }
}

// Fonction pour exécuter une commande SQL avec des vérifications
void execute_statement(Statement* statement) {
    TreeNode* node = NULL;

    switch (statement->type) {
        case STATEMENT_INSERT:
            if (validate_insert(statement->id, statement->name)) {
                insert_row(statement->id, statement->name);
                printf("\033[32m✓ Inserted (%d, %s)\033[0m\n", statement->id, statement->name);
            }
            break;

        case STATEMENT_SELECT:
            if (statement->has_where) {
                node = search_row(statement->where_id);
                if (node) {
                    printf("\033[32m✓ Found: (%d, %s)\033[0m\n", node->id, node->name);
                } else {
                    printf("\033[31m✗ No row found with ID %d\033[0m\n", statement->where_id);
                }
            } else {
                select_row();
            }
            break;

        case STATEMENT_SEARCH:
            node = search_row(statement->id);
            if (node) {
                printf("\033[32m✓ Found: (%d, %s)\033[0m\n", node->id, node->name);
            } else {
                printf("\033[31m✗ No row found with ID %d\033[0m\n", statement->id);
            }
            break;

        case STATEMENT_DELETE:
            node = search_row(statement->id);
            if (node) {
                delete_row(statement->id);
                printf("\033[31m✓ Deleted row with ID %d\033[0m\n", statement->id);
            } else {
                printf("\033[31m✗ Error: No row found with ID %d to delete.\033[0m\n", statement->id);
            }
            break;

        case STATEMENT_UPDATE:
            node = search_row(statement->id);
            if (node) {
                strcpy(node->name, statement->name);
                printf("\033[32m✓ Updated row with ID %d to name %s\033[0m\n", node->id, node->name);
            } else {
                printf("\033[31m✗ Error: No row found with ID %d to update.\033[0m\n", statement->id);
            }
            break;

        case STATEMENT_HISTORY:
            print_history();
            break;

        case STATEMENT_HELP:
            print_help();
            break;
    }
}

// Boucle principale du REPL
void repl(void) {
    char buffer[1024];
    Statement statement;

    printf("-------------------------------------------------\n");
    printf("\033[32mBienvenue dans la base de données !\033[0m\nTapez 'help' pour voir les commandes.\n");
    printf("-------------------------------------------------\n");

    while (1) {
        print_prompt();
        read_input(buffer, 1024);
        store_command(buffer);  // Enregistrer chaque commande

        if (strcmp(buffer, ".exit") == 0) {
            printf("Sauvegarde de l'arbre dans un fichier...\n");
            FILE* file = fopen("db_save.txt", "w");
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
