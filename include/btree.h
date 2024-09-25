// path : src/include/btree.h

/*
* ---------------------------------------------------------------------------------
* Ce fichier contient les fonctions de manipulation de la structure de données
* qui est un arbre binaire
* Les fonctions sont:
* - insert_row: qui permet d'insérer une ligne dans l'arbre
* - select_row: qui permet de selectionner toutes les lignes de l'arbre
* ---------------------------------------------------------------------------------
*
*/

#ifndef BTREE_H
#define BTREE_H

// Fonctions qui sert à manipuler la base de données

void insert_row(int id, char* name);
void select_row();

#endif