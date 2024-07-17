# TIPE - Antivirus

## Règles

1. Choisissez un défi et placez toutes les pièces du jeu comme indiqué sur le plateau de jeu.
2. Déplacez les pièces de jeu pour pouvoir manœuvrer le virus (la pièce de jeu rouge) vers la sortie : 
    - Les pièces de jeu ne peuvent glisser qu'en diagonale, dans 4 directions. Vous ne devez pas les déplacer horizontalement ou verticalement ni les faire pivoter.
    - Parfois, vous devez déplacer des pièces ensemble en groupe.
    - Vous êtes autorisé à pousser une partie d'une pièce de jeu à travers l' ouverture, pour libérer de l'espace pour déplacer d'autres pièces de jeu pendant votre défi.
    - Vous pouvez déplacer toutes les pièces du jeu sauf les petites blanches. Vous devriez les considérer comme faisant partie du plateau de jeu pendant les défis.
3. Vous avez trouvé la solution lorsque vous pouvez forcer le virus rouge à passer complètement par l'ouverture et à sortir du plateau de jeu. Célébrez votre réussite en relevant un nouveau défi !

## Modélisation

Une pièce est représenté par :
- une position de référence arbitraire
- le nombre de cases qu'elle occupe
- la liste des positions relatives des cases occupés
- un bouléen donnant la possibilité de déplacé la pièce

La structure de jeu est représenté par :
- la largeur de la grille
- l'emplacement de la sortie
- le nombre de pièces
- la liste des pièces

## Résolution naïve

Parcours en largeur de l'arbre des positions. On construit étape par étape cet arbre des positions

### Méthode

1. On commence par la position initiale
2. On génère les positions suivantes possibles
3. On ajoute ces positions à la file des positions à explorer
4. On recommence avec la première position de la file

### Hash d'une grile

Pour éviter d'explorer plusieurs fois la même grille, on utilise un hash **unique** de la grille. On stocke les grilles déjà explorées dans une structure `vus`.

On note 
- $l$ la largeur de la grille
- $n$ le nombre de pièces
- $\forall k \in \llbracket 1, n \rrbracket,(i_k, j_k)$ les coordonnées de la pièce $k$ dans la grille  (avec $i_k$ la ligne et $j_k$ la colonne)

On définit le hash d'une grille comme suit (en base $l$) :

$\overline{j_n i_n j_{n-1} i_{n-1} \ldots j_1 i_1}^l$

Cette entier est stocké en base 10 en mémoire

### Structure de données utilisée pour `vus`

#### Liste

- Complexité insertion : $O(1)$
- Complexité recherche : $O(n)$

#### Arbre binaire de recherche : AVL

- Complexité insertion : $O(\log n)$
- Complexité recherche : $O(\log n)$

### Résultats

## Las Vegas

### Méthode

On choisit aléatoirement l'ordre dans lequel on bouge les pièces pour une configuration donnée

### Résultats

## Heuristiques

### Méthodes

Certaines idées d'heuristiques pour le problème du jeu Anti-virus peuvent inclure des mesures de la distance entre l'état actuel et l'état final. Voici quelques suggestions d'heuristiques que vous pourriez explorer :

1. **Distance de la pièce rouge à la sortie :** Mesurez la distance horizontale entre la position actuelle de la pièce rouge et la sortie. Cette heuristique pourrait être admissible car il est peu probable que la pièce rouge se déplace latéralement sans se rapprocher de la sortie.

2. **Nombre de pièces bloquant la pièce rouge :** Comptez le nombre de pièces qui bloquent directement le chemin de la pièce rouge vers la sortie. Une heuristique plus élevée pourrait indiquer un état moins prometteur.

3. **Nombre de pièce sur le chemin direct de la pièce rouge :** Comptez le nombre de pièces qui se trouvent sur le chemin direct entre la pièce rouge et la sortie. Cette heuristique est utile pour prioriser les mouvements qui dégagent le chemin de la pièce rouge.

### Exploitation des résultats

- Tester les heuristiques sur de nombreuses instances
    - générer des instances aléatoires
- Comparer les performances des heuristiques
- Classer les heuristiques en fonction de leur efficacité (statistiques sur les instances testées)

## Génération de grilles

https://medium.com/swlh/programming-puzzle-rush-hour-traffic-jam-3ee513e6c4ab

### Limites

- Les grilles générés necessite parfois beaucoup de mémoire pour être résolus (OOM)

## IA classification : choix d'heuristique

k plus proches voisins + arbre k dimensionnel