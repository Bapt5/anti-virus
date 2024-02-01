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
