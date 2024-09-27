#ifndef UTILS_H
#define UTILS_H

#include "btree.h"

int validate_insert(int id, char* name);
void print_history();

extern char* command_history[];
extern int history_count;

#endif
