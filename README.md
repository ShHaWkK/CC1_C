# CC1_C

### Author 
- ShHaWkK (alias Alexandre UZAN)

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



### Outils
### Bonus : valgrind 
Tout d'abord, veuillez installer l'outil : 
pour valgrind : 

```
sudo apt-get install valgrind

```

Puis vous pouvez mettre les commandes suivantes pour Valgrind : 

Si vous avez déjà compilé votre code avec `make`, commencez par nettoyer les fichiers précédents :
ˋˋˋbash
make clean
ˋˋˋ
ensuite la commande suivante pour lancer le Valgrind : 
```
make valgrind
```
> [!IMPORTANT]
> valgrind sert à quoi ? 
> il va permettre d’analyser le programme et de détecter d’éventuelles erreurs de mémoire.

## Explications

### fichier repl.c : 
> [!NOTE]
> - ```prepare_statement``` : Cette fonction **analyse** l'entrée de l'utilisateur pour déterminer le type de commande et extraire les paramètres.
>   - Elle utilise une série de `if` et `else if` pour comparer le début de la chaîne d'entrée avec des commandes.
>   - Pour chaque type de commande, elle utilise `sscanf` pour extraire les paramètres de la chaîne d'entrée.
>   - Pour les commandes comme "INSERT INTO", elle effectue une analyse **manuelle de la chaîne pour extraire les noms de colonnes et les valeurs**.
>   - Elle alloue dynamiquement de la mémoire pour les noms de colonnes dans le cas de commandes comme "SELECT" ou "INSERT INTO".

> [!IMPORTANT]
> La fonction `prepare_statement` est très importante parce qu'elle détermine comment les commandes de l'utilisateur sont interprétées. 

> [!NOTE]
> - ```execute_statement``` : Cette fonction est le cœur de l'exécution des commandes dans notre base de données.
>   
>   - Elle utilise une structure `switch` pour identifier le type de commande à exécuter. Je trouve que c'est la méthode "facile" à gérer : ajouter une nouvelle commande ne nécessite que l'ajout d'un nouveau cas dans le switch.
>   
>   - Pour chaque type de commande, elle appelle une fonction spécifique de la base de données :
>     
>     - `STATEMENT_CREATE_TABLE` : Crée une nouvelle table dans la base de données.<br>
>       Exemple : `create table users` créerait une nouvelle table nommée "users".
>     
>     - `STATEMENT_ADD_COLUMN` : Ajoute une nouvelle colonne à une table existante.<br>
>       Exemple : `add column users age INT` ajouterait une colonne "age" de type entier à la table "users".
>     
>     - `STATEMENT_INSERT` : Insère une nouvelle ligne de données dans une table.
>       Cette fonction prépare les données sous forme de tableau avant de les insérer.
>     
>     - `STATEMENT_SELECT` et `STATEMENT_SELECT_WHERE` : Récupère des données de la base.
>       La différence entre les deux est la présence d'une condition WHERE pour filtrer les résultats.
>     
>     - `STATEMENT_UPDATE` : Modifie des données existantes dans une table.<br>
>       Exemple : `update users set age = 30 where id = 1` mettrait à jour l'âge de l'utilisateur avec l'ID 1.
>     
>     - `STATEMENT_DELETE` : Supprime des lignes d'une table.<br>
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
>   - Elle lit  le fichier et reconstruit les structures de données de la base de données avec les informations sauvegardées.
>   
>   - La gestion des erreurs est cruciale :
>     Si une commande n'est pas reconnue, un message d'erreur est affiché.
>     Cela évite les crashs du programme et informe l'utilisateur du problème.
> [!NOTE]
> - ```repl``` : Cette fonction est la **boucle principale** du programme, implémentant le modèle Read-Eval-Print Loop.
>   - Elle initialise d'abord la base de données et l'historique des commandes.
>   - Ensuite, elle entre dans une boucle infinie où elle :
>     Affiche un prompt
>     Lit l'entrée de l'utilisateur
>     Stocke la commande dans l'historique
>     Prépare la déclaration
>     Exécute la déclaration si elle est valide
>     Libère la mémoire allouée pour la déclaration
>   - Si la préparation de la déclaration échoue, elle affiche un message d'erreur.

> [!IMPORTANT]
> La boucle REPL est le cœur du programme. Elle gère toute l'interaction avec l'utilisateur et assure que chaque commande est traitée correctement. La gestion des erreurs et la libération de la mémoire sont cruciales ici pour maintenir la stabilité du programme sur le long terme.


### Fichier btree.c : 

### Fonction pour delete : 
> [!NOTE]
> - ```find_min``` : Cette fonction **cherche** le nœud avec la plus petite valeur dans un sous-arbre en se déplaçant vers la gauche jusqu'à ce qu'il n'y ait plus de nœuds à gauche.
> - ```delete_node```: Cette fonction **supprime** un nœud d'un arbre binaire de recherche. 
> Elle gère trois cas : si le nœud a aucun enfant, un enfant, ou deux enfants. Si le nœud a deux enfants, elle remplace le nœud à supprimer par le plus petit nœud de son sous-arbre droit  et supprime ce successeur.
- Un peu plus Explicatif concernant la fonction delete_node :  
> [!IMPORTANT]
>-  **3 cas de suppression :**
>    - Pas d'enfants : On supprime simplement le nœud en renvoyant NULL.
>    - Un enfant : On renvoie le sous-arbre qui contient cet enfant.
>    - Deux enfants : On ne peut pas simplement supprimer le nœud car cela casserait la structure de l'arbre. On le remplace donc par son successeur logique, qui est le nœud ayant la plus petite valeur dans son sous-arbre droit (trouvé avec `find_min`).

### Fonction `insert_in_tree`
> [!Note]
> La fonction d'insertion gère l'ajout d'un nouveau nœud tout en maintenant l'équilibre de l'arbre.
 En comparant l'ID, on détermine si le nouveau nœud doit être placé à gauche ou à droite. j'ai utilisé une fonction récursive simplifie "l'algorithme", car chaque appel traite une sous-partie de l'arbre jusqu'à ce qu'on trouve la position pour insérer le nouveau nœud.


> [!IMPORTANT]
>Le projet utilise un arbre binaire de recherche pour stocker les données
>La taille maximale du nom est limitée à 255 caractères
>Les données sont sauvegardées automatiquement à la sortie du programme
>L'historique des commandes est également sauvegardé


### Fonction `create_node`
Pourquoi cette idée ?
>[!IMPORTANT]
> **Pourquoi utiliser assert ?** : 
> - Nous utilisons assert pour **garantir que l'ID est positif et que le nom n'est pas nul.**
>    Contrôle de la taille du nom : Limiter la taille du nom avec MAX_NAME_LENGTH évite les débordements de tampon (buffer overflow), qui sont des vulnérabilités.
>    Allocation dynamique : Nous allouons de la mémoire pour un nouveau nœud. Si l'allocation échoue, cela renvoie NULL et signalons l'erreur à l'utilisateur.

### Fonction `find_min`

Cette fonction cherche le nœud avec la plus petite valeur dans un sous-arbre. En descendant toujours vers la gauche (dans un arbre binaire de recherche), on arrive au nœud le plus petit.
C'est utile lors de la suppression de nœuds ayant deux enfants (voir plus bas), car on doit remplacer le nœud par son successeur logique.

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

## Problèmes Rencontrés avec AFL++ et Valgrind

### Incompatibilité entre ASan et Valgrind

J'ai découvert une incompatibilité entre Address Sanitizer (ASan) et Valgrind.  il était nécessaire de compiler le programme sans les options ASan, car Valgrind possède son propre système pour détecter les erreurs de mémoire, ce qui entre en conflit avec ASan. 
Cette incompatibilité s'explique par le fait que les deux outils instrumentent les allocations mémoire de manière différente, ce qui peut conduire à des conflits.

### Solution pour AFL++

Pour AFL++, j'ai rencontré des problèmes liés à la taille de la mémoire allouée pour la bitmap de couverture. 
La solution a été d'augmenter la taille de AFL_MAP_SIZE. Par exemple :
```bash
AFL_MAP_SIZE=1000000 make run_fuzzer
```
Cette augmentation de la taille de la map permet à AFL++ de stocker plus d'informations sur la couverture du code, ce qui peut être nécessaire pour des programmes plus complexes ou avec beaucoup de chemins d'exécution.

J'ai passé beaucoup de temps à comprendre que  Valgrind et AFL++ ne sont pas conçus pour être utilisés ensemble, mais plutôt de manière indépendante

Au final j'ai juste utilisé **Valgrind**
![image](https://github.com/user-attachments/assets/3091b649-052b-4d68-ab82-19f85d8da6c5)

