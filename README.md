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


## Description du Projet

Ce projet implémente un système de gestion de base de données avancé utilisant une structure d'arbre binaire pour un stockage efficace des données. Il offre une interface en ligne de commande (REPL) robuste avec des fonctionnalités étendues, une persistance des données, et des mécanismes de sécurité.

## Structure du Projet

- **src/**
  - `main.c` : Point d'entrée du programme
  - `btree.c` : Implémentation de l'arbre binaire
  - `repl.c` : Interface en ligne de commande (REPL)
  - `database.c` : Opérations avancées de base de données
  - `utils.c` : Fonctions utilitaires et de sécurité
  - `table.c` : Gestion avancée des tables
- **include/**
  - Fichiers d'en-tête correspondants (.h)
- **Makefile** : Compilation, tests, et outils d'analyse
- `db_save.txt` : Persistance des données
- `command_history.txt` : Historique des commandes

## Commandes disponibles dans le REPL

| Commande | Description | Exemples |
|----------|-------------|----------|
| `create table <table_name>` | Crée une nouvelle table | `create table users`<br>`create table orders` |
| `add column <table> <column> <type>` | Ajoute une colonne à une table existante | `add column users id int`<br>`add column users name string`<br> `add column users age int`<br>`add column orders id int ` <br>`add column orders user_id int `<br> `add column orders product string` |
| `insert <id> <name>` | Insère une nouvelle entrée avec ID et nom | `insert 1 John`<br>`insert 2 Alice` |
| `select [cols] from <table> [where]` | Sélectionne des données d'une table | `select * from users`<br>`select * from users where id = 2`<br> `select name, age from users` |
| `update <table> set <col> = <val>` | Met à jour des données dans une table | `update users set age = 31 where id = 1`|
| `delete from <table> where id = <id>` | Supprime une entrée d'une table | `delete from users where id = 3` |
| `show tables` | Affiche la liste des tables | `show tables` |
| `show columns <table>` | Affiche les colonnes d'une table | `show columns users`<br>`show columns orders` |
| `join <table1> <table2> on <column>` | Effectue une jointure entre deux tables | `join users orders on id `|
| `save <filename>` | Sauvegarde la base de données dans un fichier | `save mydb.txt` |
| `load <filename>` | Charge une base de données depuis un fichier | `load mydb.txt` |
| `INSERT INTO <table> (cols) VALUES` | Insère des données dans une table (syntaxe SQL) | `INSERT INTO users (id, name, age) VALUES (1, John, 30)`<br>`INSERT INTO users (id, name, age) VALUES (2, Alice, 25)`<br>`INSERT INTO users (id, name, age) VALUES (3, Bob, 35)`<br> `INSERT INTO orders (id, user_id, product) VALUES (1, 1, laptop) ` <br> `INSERT INTO orders (id, user_id, product) VALUES (2, 2, phone) `   |
| `DROP TABLE <table>` | Supprime une table | `DROP TABLE table_name`<br>`DROP TABLE orders` |
| `help` | Affiche l'aide | `help` |
| `history` | Affiche l'historique des commandes | `history` |
| `.exit` | Quitte le programme | `.exit` |


### Compilation
```bash
make
```
```bash
./class_db.exe
```

## Explications

### Fichier btree.c : 

### Fonction pour delete : 
> [!NOTE]
> - ```find_min``` : Cette fonction **cherche** le nœud avec la plus petite valeur dans un sous-arbre en se déplaçant vers la gauche jusqu'à ce qu'il n'y ait plus de nœuds à gauche.
> - ```delete_node```: Cette fonction **supprime** un nœud d'un arbre binaire de recherche. 
> Elle gère trois cas : si le nœud a aucun enfant, un enfant, ou deux enfants. Si le nœud a deux enfants, elle remplace le nœud à supprimer par le plus petit nœud de son sous-arbre droit  et supprime ce successeur.

Pourquoi cette idée ?

> [!IMPORTANT]
> Quand on supprime un nœud avec deux enfants, **on ne peut pas simplement le retirer sans casser la structure de l'arbre**. 
> La solution est de **remplacer** ce nœud par son "successeur logique" - le plus petit élément plus grand que lui.

### Fonction pour insert 
> [!Note]
> La fonction d'insertion gère l'ajout d'un nouveau nœud tout en maintenant l'équilibre de l'arbre.


> [!IMPORTANT]
>Le projet utilise un arbre binaire de recherche pour stocker les données
>La taille maximale du nom est limitée à 255 caractères
>Les données sont sauvegardées automatiquement à la sortie du programme
>L'historique des commandes est également sauvegardé

>[!Note]
**Sécurité**
- Validation des entrées pour prévenir les **débordements de tampon**
- Utilisation de **strncpy** pour des **copies de chaînes sécurisées**
