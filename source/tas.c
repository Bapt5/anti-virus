#include "tas.h"

void echange(tas_content* a, tas_content* b) {
    tas_content temp = *a;
    *a = *b;
    *b = temp;
}

bool comparaison_min(int a, int b) {
    return a < b;
}

bool comparaison_max(int a, int b) {
    return a > b;
}

tas creer_tas(int capacite, bool min) {
    tas tas;
    tas.tab = malloc(capacite * sizeof(tas_content));

    if (tas.tab == NULL) {  // Si l'allocation a échoué on retourne un tas vide
        tas.taille = 0;
        tas.capacite = 0;
        return tas;
    }

    tas.taille = 0;
    tas.capacite = capacite;
    
    if (min) {
        tas.comparaison = comparaison_min;
    } else {
        tas.comparaison = comparaison_max;
    }

    return tas;
}

bool est_vide_tas(tas tas) {
    return tas.taille == 0;
}

bool inserer_tas(void* valeur, int priorite, tas* tas) {
    if (tas->taille == tas->capacite) {
        tas->capacite *= 2;
        tas->tab = realloc(tas->tab, tas->capacite * sizeof(tas_content));

        if (tas->tab == NULL) {  // Si l'allocation a échoué on retourne false
            return false;
        }
    }

    tas->tab[tas->taille].valeur = valeur;
    tas->tab[tas->taille].priorite = priorite;

    int i = tas->taille;
    while (i > 0 && tas->comparaison(tas->tab[i].priorite, tas->tab[(i - 1) / 2].priorite)) {
        // Echange père et fils
        echange(&tas->tab[i], &tas->tab[(i - 1) / 2]);

        i = (i - 1) / 2;
    }

    tas->taille++;
    return true;
}

void* extraire_tas(tas* tas) {
    if (est_vide_tas(*tas)) {
        return NULL;
    }

    void* min = tas->tab[0].valeur;
    tas->tab[0] = tas->tab[tas->taille - 1];
    tas->taille--;

    int i = 0;
    while (2 * i + 1 < tas->taille) {
        int plus_petit_fils = 2 * i + 1;
        if (2 * i + 2 < tas->taille && tas->comparaison(tas->tab[2 * i + 2].priorite, tas->tab[2 * i + 1].priorite)) {
            plus_petit_fils = 2 * i + 2;
        }

        // si le pere est plus petit que le plus petit des fils, il est plus petit que tous les fils
        if (tas->comparaison(tas->tab[i].priorite, tas->tab[plus_petit_fils].priorite)) {
            break;
        }

        echange(&tas->tab[i], &tas->tab[plus_petit_fils]);

        i = plus_petit_fils;
    }

    return min;
}

void free_tas(tas* tas, fct_free free_valeur) {
    for (int i = 0; i < tas->taille; i++) {
        if (free_valeur != NULL) {
            free_valeur(tas->tab[i].valeur);
        }
    }

    free(tas->tab);
    tas->taille = 0;
    tas->capacite = 0;
}