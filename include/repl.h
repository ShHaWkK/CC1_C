#ifndef REPL_H
#define REPL_H

#include "database.h"
#include "btree.h"

void init_repl();
void print_prompt();
void read_input(char* buffer, size_t buffer_length);
void repl();

#endif // REPL_H

