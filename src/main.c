#include <stdio.h>
#include <stdlib.h>
#include "../include/repl.h"
#include "../include/btree.h"

int main() {
    printf("------Commencer le programme...------\n");
    load_tree("db_save.txt");
    printf("L'arbre est chargé avec succès.\n");

    // Lancer la boucle REPL
    repl();

    // Sauvegarde de l'arbre à la fin du programme
    FILE *file = fopen("db_save.txt", "w");
    if (file != NULL) {
        save_tree(file, root);
        fclose(file);
        printf("Arbre sauvegardé dans db_save.txt.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'arbre.\n");
    }
    
    printf("--- Fin du programme ---\n");
    system("pause");  // Utilisez cette ligne uniquement sur Windows
    return 0;
}
