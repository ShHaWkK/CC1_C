#include <stdio.h>
#include <stdlib.h>
#include "../include/repl.h"
#include "../include/btree.h"
#include "../include/database.h"

int main() {
    printf("------Commencer le programme...------\n");
    load_tree("db_save.txt");
    printf("L'arbre est chargé avec succès.\n");

    repl();

    FILE *file = fopen("db_save.txt", "w");
    if (file != NULL) {
        save_tree(file, g_root);
        fclose(file);
        printf("Arbre sauvegardé dans db_save.txt.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'arbre.\n");
    }
    
    printf("--- Fin du programme ---\n");
    return 0;
}
