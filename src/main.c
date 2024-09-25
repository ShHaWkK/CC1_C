#include <stdio.h>
#include <stdlib.h>
#include "repl.c"
#include <stdbool.h>
#include <string.h>


int main(int argc, char* argv[], char* envp[]){
  
  repl(); 
  // Start the REPL (Read Eval Print Loop)
  return 0;
  
}