# CC1_C

## Exam condition

### Conditions of exam
– How to build and maintain code (documentation, build, attention point)
– Compiler flags (define which ones you need and tell me why you
chose them) and which compiler’s version
– The architecture will be looked at – Do not do a single huge .c file,
you’ll lose a lot of points
– What is to be done to build project
– Commit are looked at – do not push at the end, work continuously


Look at : 'https:://github.com/Mbahal/class_db'


Do the makefile
- Start doing structures for table and rows (for rows you can define
one struct and use it, and not use user input)
- Implement the select statement
- Implement the Insert statement


Provide some tests (assert for now)
- Keep track on where you are in the DB should you need it
- Store everything in a binary tree and develop the major things you’ll need (balancing/splitting/searching/inserting/deleting and
other if you have ideas)
- Do some disk persistence


Bonus – Code audit and “quality”

- One of the best way to find bugs (whether security or not) is to
use dynamic security tools thus
- Look at valgrind/fuzzers (honggfuzz or AFL++) and sanitizers and
integrate them in your code when testing and report on some bug they
found and how you patched them. You can see electric fence as well
even though I have mixed feelings on it.

## Important 
The most important things for me are
– Working binary tree, and some sql commands (insert/delete/select)
– Disk persistence is quite easy and should not be too hard

rendu : 17/11 23h59


## Structure du Projet 

Ce projet implémente une base de données simple utilisant une structure d'arbre binaire pour stocker des lignes avec des commandes de type SQL. 

Il permet d'effectuer des opérations de base telles que l'insertion, la sélection, la suppression et la recherche de données. 

De plus, il offre une **persistance des données sur disque**  pour garantir que les informations sont conservées.

 - **Commandes de base** comme `insert`, `select`, `delete` et `search` disponibles dans un environnement REPL
 - **Sortie colorée** : J'ai mis des couleurs pour avoir une interface utilisateur pour **améliorer l'expérience utilisateur** (par exemple, vert pour les insertions réussies, rouge pour les erreurs).

## Commandes disponibles dans le REPL

| Commande                       | Syntaxe                | Exemple            | Description                                          |
|--------------------------------|-----------------------|--------------------|------------------------------------------------------|
| Insérer une ligne              | `insert <id> <nom>`   | `insert 1 Alice`   | Ajoute une nouvelle ligne avec un ID et un nom      |
| Sélectionner toutes les lignes  | `select`              | `select`           | Affiche toutes les lignes de la base de données     |
| Rechercher une ligne par ID    | `search <id>`         | `search 1`         | Recherche et affiche une ligne spécifique par son ID |
| Supprimer une ligne par ID     | `delete <id>`         | `delete 1`         | Supprime une ligne spécifique par son ID             |
| Quitter et sauvegarder         | `.exit`               | `.exit`            | Sauvegarde l'arbre dans `db_save.txt` et quitte le programme |
