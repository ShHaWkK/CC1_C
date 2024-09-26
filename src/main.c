#include <stdio.h>
#include <stdlib.h>
#include "../include/repl.h"
#include "../include/btree.h"


int main() {
    printf("Commencer le programme...\n");
    load_tree("db_save.txt");
    printf("L'arbre est chargé avec succès.\n");
    repl();
    printf("Fin du programme.\n");
    system("pause");
    
    return 0;
}