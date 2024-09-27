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
*  - store_command : Stocke les commandes dans un historique
*  - print_history : Affiche l'historique des commandes
*  - save_command_history : Sauvegarde l'historique dans un fichier
*  - load_command_history : Charge l'historique depuis un fichier
*  - my_strdup : Duplique une chaîne de caractères
* ---------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/btree.h"
#include "../include/repl.h"
#include "../include/utils.h"
#include "../include/table.h"

#define MAX_NAME_LENGTH 255
#define MAX_HISTORY 100      

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_SELECT_BY_ID,
    STATEMENT_DELETE,
    STATEMENT_UPDATE,
    STATEMENT_HELP,
    STATEMENT_HISTORY,
    STATEMENT_SHOW_TABLE
} StatementType;

typedef struct {
    StatementType type;
    int id;
    char name[MAX_NAME_LENGTH];
    int where_id;
    int has_where;
} Statement;

char* command_history[MAX_HISTORY];
int history_count = 0;

char* my_strdup(const char* str) {
    size_t len = strlen(str) + 1;
    if (len > MAX_NAME_LENGTH) {
        len = MAX_NAME_LENGTH;
    }
    char* copy = malloc(len);
    if (copy != NULL) {
        strncpy(copy, str, len - 1);
        copy[len - 1] = '\0';
    }
    return copy;
}

void store_command(const char* command) {
    if (history_count < MAX_HISTORY) {
        command_history[history_count] = my_strdup(command);
        history_count++;
    }
}

void save_command_history(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("\033[31m✗ Erreur : Impossible d'ouvrir le fichier %s\033[0m\n", filename);
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", command_history[i]);
    }
    fclose(file);
    printf("\033[32m✓ Historique sauvegardé dans %s\033[0m\n", filename);
}

void load_command_history(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        store_command(line);
    }
    fclose(file);
}

void print_prompt() {
    printf("\033[36mdb_alex > \033[0m");
}

void read_input(char* buffer, size_t buffer_length) {
    if (fgets(buffer, buffer_length, stdin) == NULL) {
        printf("\033[31m✗ Erreur lors de la lecture de l'entrée\033[0m\n");
        exit(EXIT_FAILURE);
    }
    buffer[strlen(buffer) - 1] = '\0';
}

int prepare_statement(char* buffer, Statement* statement) {
    statement->has_where = 0;

    if (strncmp(buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(buffer, "insert %d %254s", &(statement->id), statement->name);
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "select id", 9) == 0) {
        statement->type = STATEMENT_SELECT_BY_ID;
        int args_assigned = sscanf(buffer, "select id %d", &(statement->id));
        if (args_assigned != 1) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        char* where_clause = strstr(buffer, "where");
        if (where_clause) {
            statement->has_where = 1;
            int args_assigned = sscanf(where_clause, "where id = %d", &(statement->where_id));
            if (args_assigned != 1) {
                return 0;
            }
        }
        return 1;
    } else if (strncmp(buffer, "delete", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        int args_assigned = sscanf(buffer, "delete %d", &(statement->id));
        if (args_assigned < 1 || statement->id <= 0) {
            return 0;
        }
        return 1;
    } else if (strncmp(buffer, "update", 6) == 0) {
        statement->type = STATEMENT_UPDATE;
        int args_assigned = sscanf(buffer, "update %254s where id = %d", statement->name, &(statement->id));
        if (args_assigned < 2 || statement->id <= 0 || strlen(statement->name) == 0) {
            return 0;
        }
        return 1;
    } else if (strcmp(buffer, "help") == 0) {
        statement->type = STATEMENT_HELP;
        return 1;
    } else if (strcmp(buffer, "history") == 0) {
        statement->type = STATEMENT_HISTORY;
        return 1;
    } else if (strcmp(buffer, "show table") == 0) {
        statement->type = STATEMENT_SHOW_TABLE;
        return 1;
    }

    return -1;
}

void print_help() {
    printf("\n\033[36m=== Commandes Disponibles ===\033[0m\n");
    printf("\033[32minsert <id> <name>\033[0m   : Insérer une nouvelle ligne\n");
    printf("\033[32mselect\033[0m              : Afficher toutes les lignes\n");
    printf("\033[32mselect id <id>\033[0m      : Sélectionner une ligne avec un ID spécifique\n");
    printf("\033[32mselect where id = <id>\033[0m : Sélectionner une ligne avec un ID spécifique\n");
    printf("\033[32mdelete <id>\033[0m         : Supprimer une ligne\n");
    printf("\033[32mupdate <name> where id = <id>\033[0m  : Mettre à jour une ligne\n");
    printf("\033[32mshow table\033[0m          : Afficher la structure de la table\n");
    printf("\033[32mhistory\033[0m             : Afficher l'historique des commandes\n");
    printf("\033[32m.exit\033[0m               : Sauvegarder et quitter\n");
    printf("\033[32mhelp\033[0m                : Afficher cette aide\n");
}


void show_table_structure() {
    printf("\n\033[36m=== Structure de la Table ===\033[0m\n");
    printf("+------+----------------------+\n");
    printf("|  ID  | Name                 |\n");
    printf("+------+----------------------+\n");
}

void execute_statement(Statement* statement) {
    static Table* table = NULL;

    if (table == NULL) {
        table = create_table();
    }

    switch (statement->type) {
        case STATEMENT_INSERT:
            insert_into_table(table, statement->id, statement->name);
            break;
        case STATEMENT_SELECT:
            if (statement->has_where) {
                select_row_from_table(table, statement->where_id);
            } else {
                select_all_from_table(table);
            }
            break;
        case STATEMENT_SELECT_BY_ID:
            select_row_from_table(table, statement->id);
            break;
        case STATEMENT_DELETE:
            delete_from_table(table, statement->id);
            break;
        case STATEMENT_UPDATE:
            update_row(statement->id, statement->name);
            break;
        case STATEMENT_HELP:
            print_help();
            break;
        case STATEMENT_HISTORY:
            print_history();
            break;
        case STATEMENT_SHOW_TABLE:
            show_table_structure();
            break;
        default:
            printf("\033[31m✗ Commande non reconnue.\033[0m\n");
            break;
    }
}


void repl(void) {
    char buffer[1024];
    Statement statement;

    printf("-------------------------------------------------\n");
    printf("\033[32mBienvenue dans la base de données !\033[0m\nTapez 'help' pour voir les commandes.\n");
    printf("-------------------------------------------------\n");

    load_tree("db_save.txt");
    load_command_history("command_history.txt");

    while (1) {
        print_prompt();
        read_input(buffer, 1024);
        store_command(buffer);

        if (strcmp(buffer, ".exit") == 0) {
            printf("Sauvegarde de l'historique...\n");
            save_command_history("command_history.txt");

            printf("Sauvegarde de l'arbre...\n");
            FILE* file = fopen("db_save.txt", "w");
            assert(file != NULL);
            save_tree(file, root);
            fclose(file);
            printf("\033[32m✓ Arbre et historique sauvegardés avec succès.\033[0m\n");
            exit(EXIT_SUCCESS);
        }

        int result = prepare_statement(buffer, &statement);
        if (result == 1) {
            execute_statement(&statement);
        } else if (result == 0) {
            printf("\033[31m✗ Erreur de syntaxe ou données invalides\033[0m\n");
        } else {
            printf("\033[31m✗ Commande non reconnue : '%s'\033[0m\n", buffer);
        }
    }
}
