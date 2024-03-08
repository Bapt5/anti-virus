#include "tas_min.h"

void echange(tas_min_content* a, tas_min_content* b) {
    tas_min_content temp = *a;
    *a = *b;
    *b = temp;
}

tas_min creer_tas_min(int capacite) {
    tas_min tas;
    tas.tab = malloc(capacite * sizeof(tas_min_content));
    tas.taille = 0;
    tas.capacite = capacite;

    return tas;
}

bool est_vide_tas_min(tas_min tas) {
    return tas.taille == 0;
}

void inserer_tas_min(void* valeur, int priorite, tas_min* tas) {
    if (tas->taille == tas->capacite) {
        tas->capacite *= 2;
        tas->tab = realloc(tas->tab, tas->capacite * sizeof(tas_min_content));
    }

    tas->tab[tas->taille].valeur = valeur;
    tas->tab[tas->taille].priorite = priorite;

    int i = tas->taille;
    while (i > 0 && tas->tab[i].priorite < tas->tab[(i - 1) / 2].priorite) {
        // Echange père et fils
        echange(&tas->tab[i], &tas->tab[(i - 1) / 2]);

        i = (i - 1) / 2;
    }

    tas->taille++;
}

void* extraire_min_tas_min(tas_min* tas) {
    if (est_vide_tas_min(*tas)) {
        return NULL;
    }

    void* min = tas->tab[0].valeur;
    tas->tab[0] = tas->tab[tas->taille - 1];
    tas->taille--;

    int i = 0;
    while (2 * i + 1 < tas->taille) {
        int plus_petit_fils = 2 * i + 1;
        if (2 * i + 2 < tas->taille && tas->tab[2 * i + 2].priorite < tas->tab[2 * i + 1].priorite) {
            plus_petit_fils = 2 * i + 2;
        }

        // si le pere est plus petit que le plus petit des fils, il est plus petit que tous les fils
        if (tas->tab[i].priorite < tas->tab[plus_petit_fils].priorite) {
            break;
        }

        echange(&tas->tab[i], &tas->tab[plus_petit_fils]);

        i = plus_petit_fils;
    }

    return min;
}

void free_tas_min(tas_min* tas, void (*free_valeur)(void*)) {
    for (int i = 0; i < tas->taille; i++) {
        free_valeur(tas->tab[i].valeur);
    }

    free(tas->tab);
    tas->taille = 0;
    tas->capacite = 0;
}