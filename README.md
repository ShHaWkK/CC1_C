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

| Commande | Syntaxe | Exemple | Description |
|----------|---------|---------|-------------|
| Insérer | `insert <id> <nom>` | `insert 1 Alice` | Ajoute une nouvelle ligne |
| Sélectionner tout | `select` | `select` | Affiche toutes les lignes |
| Sélectionner par ID | `select id <id>` | `select id 1` | Affiche une ligne spécifique |
| Supprimer | `delete <id>` | `delete 1` | Supprime une ligne |
| Mettre à jour | `update <nom> where id = <id>` | `update Bob where id = 1` | Met à jour une ligne |
| Afficher la structure | `show table` | `show table` | Montre la structure de la table |
| Historique | `history` | `history` | Affiche l'historique des commandes |
| Aide | `help` | `help` | Affiche l'aide |
| Quitter | `.exit` | `.exit` | Sauvegarde et quitte le programme |

## Structure du Projet

- **src/** : Contient les fichiers source
  - `btree.c` : Implémentation de l'arbre binaire
  - `repl.c` : Interface en ligne de commande (REPL)
  - `utils.c` : Fonctions utilitaires
- **include/** : Contient les fichiers d'en-tête
  - `btree.h`
  - `utils.h`
- **tests/** : Tests unitaires et d'intégration
- **Makefile** : Pour la compilation et les tests
- `db_save.txt` : Fichier de persistance des données

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
