#ifndef UTILS_H
#define UTILS_H

#include "database.h"

#define MAX_HISTORY 100

char* my_strdup(const char* str);
void store_command(const char* command);
void save_command_history(const char* filename);
void load_command_history(const char* filename);
void print_history();
void print_help();
void show_tables(Database* db);

#endif
