#include <assert.h>
#include "../include/table.h"

void test_insert() {
    Table* table = create_table();
    insert_into_table(table, 1, "Alice");
    insert_into_table(table, 2, "Bob");
    
    assert(table->row_count == 2);  // Test si l'insertion fonctionne correctement
    printf("Test d'insertion réussi.\n");
}

void test_select() {
    Table* table = create_table();
    insert_into_table(table, 1, "Alice");

    // Test de sélection
    printf("Sélection des lignes :\n");
    select_all_from_table(table);
}

void test_delete() {
    Table* table = create_table();
    insert_into_table(table, 1, "Alice");
    insert_into_table(table, 2, "Bob");

    delete_from_table(table, 1);
    
    assert(table->row_count == 1);
    printf("Test de suppression réussi.\n");
}

int main() {
    test_insert();
    test_select();
    test_delete();
    
    printf("Tous les tests sont passés.\n");
    return 0;
}
