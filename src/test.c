#include <stdio.h>
#include "../include/btree.h"

void run_tests() {
    printf("Running tests...\n");

    // Insertion de données
    insert_row(1, "Alice");
    insert_row(2, "Bob");
    insert_row(3, "Charlie");
    insert_row(4, "David");
    insert_row(5, "Aymeric");
    select_row();

    // Recherche de données
    TreeNode* node = search_row(2);
    if (node) {
        printf("Search test passed: Found (%d, %s)\n", node->id, node->name);
    } else {
        printf("Search test failed\n");
    }

    // Suppression de données
    delete_row(2);
    select_row();

    // Vérification de la suppression
    node = search_row(2);
    if (node == NULL) {
        printf("Delete test passed: Row with ID 2 not found\n");
    } else {
        printf("Delete test failed\n");
    }
}

int main() {
    run_tests();
    return 0;
}
