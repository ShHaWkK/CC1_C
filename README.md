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
| `create table <table_name>` | Crée une nouvelle table | `create table users`<br>`create table products` |
| `add column <table> <column> <type>` | Ajoute une colonne à une table existante | `add column users email varchar(255)`<br>`add column products price float` |
| `insert <id> <name>` | Insère une nouvelle entrée avec ID et nom | `insert 1 John`<br>`insert 2 Alice` |
| `select [cols] from <table> [where]` | Sélectionne des données d'une table | `select * from users`<br>`select name, age from users where age > 18`<br>`select * from products where price < 100` |
| `update <table> set <col> = <val>` | Met à jour des données dans une table | `update users set age = 30 where id = 1`<br>`update products set stock = 0 where price > 1000` |
| `delete from <table> where id = <id>` | Supprime une entrée d'une table | `delete from users where id = 5`<br>`delete from products where stock = 0` |
| `show tables` | Affiche la liste des tables | `show tables` |
| `show columns <table>` | Affiche les colonnes d'une table | `show columns users`<br>`show columns products` |
| `join <table1> <table2> on <column>` | Effectue une jointure entre deux tables | `join users orders on users.id = orders.user_id`<br>`join products categories on products.category_id = categories.id` |
| `save <filename>` | Sauvegarde la base de données dans un fichier | `save mydb.dat`<br>`save mydb.dat` |
| `load <filename>` | Charge une base de données depuis un fichier | `load mydb.dat`<br>`load mydb.dat` |
| `INSERT INTO <table> (cols) VALUES` | Insère des données dans une table (syntaxe SQL) | `INSERT INTO users (name, age) VALUES ('Alice', 25)`<br>`INSERT INTO products (name, price) VALUES ('Laptop', 999.99)` |
| `DROP TABLE <table>` | Supprime une table | `DROP TABLE table_name`<br>`DROP TABLE table_name` |
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
