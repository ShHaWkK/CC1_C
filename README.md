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

### fichier repl.c : 
> [!NOTE]
> - ```prepare_statement``` : Cette fonction **analyse** l'entrée de l'utilisateur pour déterminer le type de commande et extraire les paramètres pertinents.
>   - Elle utilise une série de `if` et `else if` pour comparer le début de la chaîne d'entrée avec des commandes connues.
>   - Pour chaque type de commande, elle utilise `sscanf` pour extraire les paramètres de la chaîne d'entrée.
>   - Pour les commandes comme "INSERT INTO", elle effectue une analyse manuelle de la chaîne pour extraire les noms de colonnes et les valeurs.
>   - Elle alloue dynamiquement de la mémoire pour les noms de colonnes dans le cas de commandes comme "SELECT" ou "INSERT INTO".

> [!IMPORTANT]
> La fonction `prepare_statement` est très importante parce qu'elle détermine comment les commandes de l'utilisateur sont interprétées. Une erreur ici pourrait conduire à une mauvaise exécution des commandes ou à des comportements inattendus du programme.

> [!NOTE]
> - ```execute_statement``` : Cette fonction est le cœur de l'exécution des commandes dans notre système de base de données.
>   
>   - Elle utilise une structure `switch` pour identifier le type de commande à exécuter. Cette approche permet une extensibilité facile : ajouter une nouvelle commande ne nécessite que l'ajout d'un nouveau cas dans le switch.
>   
>   - Pour chaque type de commande, elle appelle une fonction spécifique de la base de données :
>     
>     - `STATEMENT_CREATE_TABLE` : Crée une nouvelle table dans la base de données.
>       Exemple : `create table users` créerait une nouvelle table nommée "users".
>     
>     - `STATEMENT_ADD_COLUMN` : Ajoute une nouvelle colonne à une table existante.
>       Exemple : `add column users age INT` ajouterait une colonne "age" de type entier à la table "users".
>     
>     - `STATEMENT_INSERT` : Insère une nouvelle ligne de données dans une table.
>       Cette fonction prépare les données sous forme de tableau avant de les insérer.
>     
>     - `STATEMENT_SELECT` et `STATEMENT_SELECT_WHERE` : Récupère des données de la base.
>       La différence entre les deux est la présence d'une condition WHERE pour filtrer les résultats.
>     
>     - `STATEMENT_UPDATE` : Modifie des données existantes dans une table.
>       Exemple : `update users set age = 30 where id = 1` mettrait à jour l'âge de l'utilisateur avec l'ID 1.
>     
>     - `STATEMENT_DELETE` : Supprime des lignes d'une table.
>       Exemple : `delete from users where id = 1` supprimerait l'utilisateur avec l'ID 1.
>     
>     - `STATEMENT_JOIN` : Combine des données de deux tables différentes.
>       C'est une opération plus complexe qui permet de relier des informations de plusieurs tables.
>     
>     - `STATEMENT_SAVE` et `STATEMENT_LOAD` : Gèrent la persistance de la base de données.
>       Ces commandes permettent de sauvegarder l'état actuel de la base ou de restaurer un état précédent.
>     - `save_database` : Cette fonction **persiste** l'état actuel de la base de données dans un fichier.
>   - Elle est appelée lorsque l'utilisateur entre une commande "save [filename]".
>   - La fonction prend en paramètres la structure de la base de données et le nom du fichier où sauvegarder.
>   - Elle utilise probablement des opérations d'écriture de fichier pour stocker les structures de données et leur contenu.

> [!NOTE]
> - `load_database` : Cette fonction **restaure** l'état de la base de données à partir d'un fichier sauvegardé.
>   - Elle est appelée lorsque l'utilisateur entre une commande "load [filename]".
>   - La fonction prend en paramètres la structure de la base de données et le nom du fichier à charger.
>   - Elle lit probablement le fichier et reconstruit les structures de données de la base de données avec les informations sauvegardées.
>   
>   - La commande EXIT est particulière :
>     Elle assure que toutes les données sont sauvegardées avant de quitter le programme.
>     Cela inclut l'historique des commandes et l'état actuel de la base de données.
>   
>   - La gestion des erreurs est cruciale :
>     Si une commande n'est pas reconnue, un message d'erreur est affiché.
>     Cela évite les crashs du programme et informe l'utilisateur du problème.
> [!NOTE]
> - ```repl``` : Cette fonction est la **boucle principale** du programme, implémentant le modèle Read-Eval-Print Loop.
>   - Elle initialise d'abord la base de données et l'historique des commandes.
>   - Ensuite, elle entre dans une boucle infinie où elle :
>     1. Affiche un prompt
>     2. Lit l'entrée de l'utilisateur
>     3. Stocke la commande dans l'historique
>     4. Prépare la déclaration
>     5. Exécute la déclaration si elle est valide
>     6. Libère la mémoire allouée pour la déclaration
>   - Si la préparation de la déclaration échoue, elle affiche un message d'erreur.

> [!IMPORTANT]
> La boucle REPL est le cœur du programme. Elle gère toute l'interaction avec l'utilisateur et assure que chaque commande est traitée correctement. La gestion des erreurs et la libération de la mémoire sont cruciales ici pour maintenir la stabilité du programme sur le long terme.


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


## Fichier database.c : Fonctions intéressantes

> [!NOTE]
> - ```create_table``` : Cette fonction **initialise** une nouvelle table dans la base de données avec un nom donné. Elle **ajoute** la table à la liste des tables de la base de données.
> 
> - ```add_column``` : Cette fonction **ajoute** une nouvelle colonne à une table existante. Elle **spécifie** le nom et le type de données de la colonne.
> 
> - ```insert_row``` : Cette fonction **insère** une nouvelle ligne de données dans une table spécifiée. Elle **copie** chaque valeur fournie dans la nouvelle ligne.
> 
> - ```select_where``` : Cette fonction **filtre et affiche** les lignes d'une table qui correspondent à une condition spécifique sur une colonne donnée.

> [!IMPORTANT]
> Dans toutes ces fonctions, il est crucial de **vérifier les limites** (nombre maximum de tables, colonnes, lignes) pour éviter les débordements. De plus, les noms et valeurs sont généralement **tronqués** s'ils dépassent la longueur maximale autorisée.

> [!NOTE]
> - ```save_database``` : Cette fonction **persiste** la structure et les données de la base de données dans un fichier binaire sur le disque.
> 
> - ```load_database``` : Cette fonction **reconstruit** la base de données à partir d'un fichier sauvegardé, chargeant toutes les structures de tables, les informations sur les colonnes et les données des lignes.

> [!IMPORTANT]
> Le format de fichier utilisé est **spécifique à cette implémentation**. Il n'est pas nécessairement compatible avec d'autres systèmes de bases de données. Assurez-vous que les **permissions de fichier** sont correctes pour l'écriture et la lecture.

>[!Note]
**Sécurité**
- Validation des entrées pour prévenir les **débordements de tampon**
- Utilisation de **strncpy** pour des **copies de chaînes sécurisées**
