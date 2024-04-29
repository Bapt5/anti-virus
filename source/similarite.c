#include "similarite.h"

void free_coordonnee(coordonnee_t coordonnee) {
    free(coordonnee.c);
}

int distance_euclidienne(coordonnee_t coordonnee1, coordonnee_t coordonnee2) {
    assert(coordonnee1.dimension == coordonnee2.dimension);

    double distance = 0;

    for (int i = 0; i < coordonnee1.dimension; i++) {
        distance += (coordonnee1.c[i] - coordonnee2.c[i]) * (coordonnee1.c[i] - coordonnee2.c[i]);
    }

    return distance;
}

coordonnee_t coordonnee1(jeu j) {
    int dimension = 5;

    int* donnee = malloc(dimension * sizeof(int));

    donnee[0] = j.nb_pieces;
    donnee[1] = j.taille;
    donnee[2] = heuristique1(j);
    donnee[3] = heuristique2(j);
    donnee[4] = heuristique3(j);

    coordonnee_t c = {dimension, donnee};

    return c;
}