#include "piece.h"

bool est_valide(position p, int taille) {
    // on calcule la taille de la ligne à laquelle appartient la position
    int taille_ligne;
    if (p.i < taille/2 + 1) {
        taille_ligne = 2*p.i + 1;
    } else {
        taille_ligne = 2*(taille - 1 - p.i) + 1;
    }

    if (p.j < (taille - taille_ligne)/2) {
        return false;
    } else if (p.j >= (taille + taille_ligne)/2) {
        return false;
    } else {
        return true;
    }
}

void free_piece (piece* p) {
    free(p->positions_rel);
    free(p);
}

piece* copie_piece(piece* p) {
    piece* copie = malloc(sizeof(piece));

    copie->id = p->id;
    copie->taille = p->taille;
    copie->positions_rel = malloc(sizeof(position) * p->taille);
    copie->pos = p->pos;
    copie->bougeable = p->bougeable;

    for (int k = 0; k < p->taille; k+=1) {
        copie->positions_rel[k].i = p->positions_rel[k].i;
        copie->positions_rel[k].j = p->positions_rel[k].j;
    }

    return copie;
}