#include <stdio.h>
#include <stdlib.h>
#include "../include/repl.h"

int main(int argc, char* argv[], char* envp[]) {
    (void)argc;
    (void)argv;
    (void)envp;

    repl();  // Démarrer la boucle REPL
    return 0;
}

