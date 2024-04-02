#ifndef _PIECE_
#define _PIECE_

#include "librairies.h"

struct position_s
{
    int i;
    int j;
};
typedef struct position_s position;

/**
    Paramètres:
        p: position à tester
        taille: taille du jeu

    Retourne:
        true si la position est bien dans la grille, false sinon
*/
bool est_valide (position, int);

struct piece_s
{
    int id;
    int taille;
    position* positions_rel;
    position pos;
    bool bougeable;
};
typedef struct piece_s piece;

/**
    Paramètres:
        taille: taille de la pièce
        positions_rel: tableau des positions relatives de la pièce
        bougeable: true si la pièce est bougeable, false sinon

    Retourne:
        - une pièce de taille taille avec les positions relatives positions_rel
        et bougeable si bougeable est true
        - la position n'est pas initialisée
*/
piece* creer_piece (int, position*, bool);

/**
    Paramètres:
        p: pointeur vers la pièce à tourner
        angle: angle de rotation (90° par angle)

    Retourne:
        - une pièce tournée de angle*90° par rapport à la pièce p
*/
piece* rotation_piece (piece*, int);

/**
    Paramètres:
        p: pointeur vers la pièce à libérer

    Libère la mémoire allouée à la pièce
*/
void free_piece (piece*);

/**
    Paramètres:
        p: pointeur vers la pièce à copier

    Retourne:
        - un pointeur vers une copie de la pièce p
*/
piece* copie_piece (piece*);


#endif
