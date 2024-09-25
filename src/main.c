#include <stdio.h>
#include <stdlib.h>
#include "../include/repl.h"
#include "../include/btree.h"


int main() {
    printf("Starting program...\n");
    load_tree("db_save.txt");
    printf("Tree loaded.\n");
    repl();
    printf("Program finished.\n");
    system("pause");
    
    return 0;
}