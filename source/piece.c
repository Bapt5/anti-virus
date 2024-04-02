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

piece* creer_piece (int taille, position* positions_rel, bool bougeable) {
    piece* p = malloc(sizeof(piece));
    p->taille = taille;
    p->positions_rel = malloc(taille * sizeof(position));
    for (int i = 0; i < taille; i++) {
        p->positions_rel[i] = positions_rel[i];
    }

    p->bougeable = bougeable;
    return p;
}

piece* rotation_piece(piece* p, int angle) {
    piece* p_rot = malloc(sizeof(piece));
    p_rot->taille = p->taille;
    p_rot->positions_rel = malloc(p->taille * sizeof(position));
    for (int i = 0; i < p->taille; i++) {
        p_rot->positions_rel[i].i = p->positions_rel[i].i;
        p_rot->positions_rel[i].j = p->positions_rel[i].j;
    }

    for (int i = 0; i < angle; i++) {
        for (int j = 0; j < p_rot->taille; j++) {
            int tmp = p_rot->positions_rel[j].i;
            p_rot->positions_rel[j].i = -p_rot->positions_rel[j].j;
            p_rot->positions_rel[j].j = tmp;
        }
    }

    p_rot->bougeable = p->bougeable;

    return p_rot;
}

void free_piece (piece* p) {
    free(p->positions_rel);
    free(p);
}

piece* copie_piece(piece* p) {
    piece* copie = malloc(sizeof(piece));
    assert(copie != NULL);

    copie->id = p->id;
    copie->taille = p->taille;
    copie->positions_rel = malloc(sizeof(position) * p->taille);
    assert(copie->positions_rel != NULL);
    copie->pos = p->pos;
    copie->bougeable = p->bougeable;

    for (int k = 0; k < p->taille; k+=1) {
        copie->positions_rel[k].i = p->positions_rel[k].i;
        copie->positions_rel[k].j = p->positions_rel[k].j;
    }

    return copie;
}