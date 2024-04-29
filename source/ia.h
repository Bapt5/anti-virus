#ifndef IA
#define IA

#include "librairies.h"

#include "liste_chaine.h"
#include "tas.h"

#include "similarite.h"

#include "jeu.h"
#include "resolution.h"


struct donnee_s
{
    coordonnee_t coordonnee;
    int heuristique;
};
typedef struct donnee_s donnee;

struct kd_trees_s
{
    int profondeur;
    donnee donnee;
    struct kd_trees_s* gauche;
    struct kd_trees_s* droit;
};
typedef struct kd_trees_s kd_trees;

struct kd_trees_context_s
{
    kd_trees* arbre;
    int dimension;
    fct_coordonnee fct_coor;
    int nb_resolutions;
    fct_resolution* resolutions;
};
typedef struct kd_trees_context_s kd_trees_context;


/*
    * Crée un arbre kd à partir d'une dimension et d'une donnée.
    * 
    * @param donnee: la donnée à stocker dans l'arbre.
    * @param profondeur: la profondeur de l'arbre.
    * 
    * @return: l'arbre kd créé.
*/
kd_trees* create_kd_trees(donnee, int);

/*
    * Entraine l'algorithme des k plus proches voisins sur un ensemble de jeux
    * 
    * @param jeux: le tableau de jeux sur lequel on entraine l'algorithme.
    * @param nb_jeux: le nombre de jeux dans le tableau.
    * @param fct_coor: une fonction qui retourne les coordonnées d'un jeu.
    * @param resolutions: un tableau de fonctions de résolution.
    * @param nb_resolutions: le nombre de fonctions de résolution.
    * 
    * @return: l'arbre kd correspondant à l'entrainement.
*/
kd_trees_context* train_knn(jeu[], int, fct_coordonnee, fct_resolution[], int);

/*
    * Libère la mémoire allouée pour un contexte d'arbre kd.
    *
    * @param context: le contexte d'arbre kd à libérer.
*/
void free_kd_trees_context(kd_trees_context*);

/*
    * Exporte un arbre kd dans un fichier.
    *
    * @param context: le contexte d'arbre kd à exporter.
    * @param nom_fichier: le nom du fichier dans lequel on exporte l'arbre.
    * 
*/
void export_kd_trees_context(kd_trees_context*, char*);

/*
    * Importe un arbre kd depuis un fichier.
    * 
    * @param nom_fichier: le nom du fichier depuis lequel on importe l'arbre.
    * @param fct_coor: une fonction qui retourne les coordonnées d'un jeu.
    * @param resolutions: un tableau de fonctions de résolution.
    * @param nb_resolutions: le nombre de fonctions de résolution.
    * 
    * @return: le contexte d'arbre kd importé.
*/
kd_trees_context* import_kd_trees_context(char*, fct_coordonnee, fct_resolution[], int);

/*
    * Recherche les k plus proches voisins d'un jeu dans un arbre kd.
    * 
    * @param arbre: l'arbre kd dans lequel on recherche.
    * @param jeu: le jeu pour lequel on recherche les k plus proches voisins.
    * @param k: le nombre de voisins à rechercher.
    * 
    * @return: l'entier correspondant à l'heuristique la plus présente parmi les k plus proches voisins.
*/
fct_resolution knn(kd_trees_context*, jeu, int);

/*
    * Affiche une matrice de confusion.
    * 
    * @param jeux: le tableau de jeux sur lequel on crée la matrice de confusion.
    * @param nb_jeux: le nombre de jeux dans le tableau.
    * @param context: le contexte d'arbre kd.
    * @param k: le nombre de voisins à rechercher.

*/
void matrice_confusion (jeu*, int, kd_trees_context*, int);


#endif