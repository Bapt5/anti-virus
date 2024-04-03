#ifndef _RECHERCHE_
#define _RECHERCHE_


#include "librairies.h"

#include "liste_chaine.h"
#include "jeu.h"


enum Direction_e {
    HAUT = 0,
    BAS = 1,
    GAUCHE = 2,
    DROITE =3
};
typedef enum Direction_e Direction;

/*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut les voisins
        - dir: la direction dans laquelle on veut les voisins
        - l: la liste dans laquelle on va ajouter les id des pièces voisines
        - deja_mise: un tableau de booléens de taille jeu_.nb_pieces
            indiquant si la pièce correspondante a déjà été mise dans la liste

    Retourne:
        - true si on a pu ajouter les id des pièces voisines à l, false sinon

    Ajoute à l les id des pièces voisines de la pièce id_piece dans la direction dir
*/
bool voisins_piece_dir (jeu jeu_, int id_piece, Direction dir, liste* l, bool* deja_mise);

/*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut les voisins
        - succes: un pointeur vers un booléen qui sera mis à true si tout s'est bien passé

    Retourne:
        - une liste d'entiers contenant les id des pièces voisines de la pièce id_piece
*/
liste voisins_piece (jeu jeu_, int id_piece, bool* succes);

/* 
    Paramètres:
        - jeu_: le jeu
        - n: le nombre de pièces à essayer de bouger
        - id_pieces: un tableau contenant les id des pièces à essayer de bouger
        - dir: la direction dans laquelle on veut bouger les pièces
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - ajout: la fonction pour ajouter un élément à f
        - path: le chemin menant à la grille actuelle

    Retourne:
        - true si on a pu ajouter les grilles accessibles à f, false sinon

    Ajoute à f les grilles accessibles en bougeant les pièces
        id_pieces[0], ..., id_pieces[n - 1] dans la direction dir
*/
bool position_accessible_dir (jeu jeu_, int n, int id_pieces[n], Direction dir, void* f, bool (*ajout)(void*, void*), liste path);

/*
    Paramètres: 
        - jeu_: le jeu
        - n: le nombre de pièces à essayer de bouger
        - id_pieces: un tableau contenant les id des pièces à essayer de bouger
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - ajout: la fonction pour ajouter un élément à f
        - path: le chemin menant à la grille actuelle

    Retourne: 
        - true si on a pu ajouter les grilles accessibles à f, false sinon

    Ajoute à f les grilles accessibles en bougeant les pièces id_pieces[0], ..., id_pieces[n - 1]
*/
bool position_accessible (jeu jeu_, int n, int id_pieces[n], void* f, bool (*ajout)(void*, void*), liste path);

/*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut bouger les voisins
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - ajout: la fonction pour ajouter un élément à f
        - path: le chemin menant à la grille actuelle

    Retourne:
        - true si on a pu ajouter les grilles accessibles à f, false sinon

    Ajoute à f les grilles accessibles en bougeant les voisins de la pièce id_piece
*/
bool bouge_voisins (jeu jeu_, int id_piece, void* f, bool (*ajout)(void*, void*), liste path);


#endif