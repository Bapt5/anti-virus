#include "abr.h"

int max (int a, int b) {
    if (a < b) {
        return b;
    }
    return a;
}

abr creer_abr() {
    /* Creer une feuille d'un ABR */
    
    abr a = malloc(sizeof(noeud));
    a->droite = NULL;
    a->gauche = NULL;
    a->hauteur = -1;

    return a;
}

void rotation_gauche(abr* a_p) {
    /* Fait une rotation gauche de l'arbre */
    abr a = *a_p;
    assert(a->droite != NULL);

    abr new_racine = a->droite;
    abr fils_gauche = new_racine->gauche;

    new_racine->gauche = a;
    a->droite = fils_gauche;

    // on met à jour les hauteurs
    a->hauteur = max(a->gauche->hauteur, a->droite->hauteur) + 1;
    new_racine->hauteur = max(new_racine->gauche->hauteur, new_racine->droite->hauteur) + 1;

    *a_p = new_racine;
}

void rotation_droite(abr* a_p) {
    /* Fait une rotation droite de l'arbre */
    abr a = *a_p;
    assert(a->gauche != NULL);

    abr new_racine = a->gauche;
    abr fils_droite = new_racine->droite;

    new_racine->droite = a;
    a->gauche = fils_droite;

    // on met à jour les hauteurs
    a->hauteur = max(a->gauche->hauteur, a->droite->hauteur) + 1;
    new_racine->hauteur = max(new_racine->gauche->hauteur, new_racine->droite->hauteur) + 1;

    *a_p = new_racine;
}

bool trouver_abr (abr a, unsigned long long val) {
    if (a->droite == NULL && a->gauche == NULL) {
        return false;

    } else if (a->valeur == val) {
        return true;

    } else if (val < a->valeur) {
        return trouver_abr(a->gauche, val);

    } else {
        return trouver_abr(a->droite, val);
    }
}

void ajouter_abr (abr* a_p, unsigned long long val) {
    abr a = *a_p;

    if (a->droite == NULL && a->gauche == NULL) {
        a->valeur = val;
        a->droite = creer_abr();
        a->gauche = creer_abr();
        a->hauteur = 0;

    } else if (val < a->valeur) {
        ajouter_abr(&(a->gauche), val);   

    } else if (a->valeur < val) {
        ajouter_abr(&(a->droite), val);

    }

    // on rééquilibre l'arbre
    if (a->droite->hauteur - a->gauche->hauteur > 1) {
        rotation_gauche(a_p);
    } else if (a->gauche->hauteur - a->droite->hauteur > 1) {
        rotation_droite(a_p);
    }
    a->hauteur = max(a->gauche->hauteur, a->droite->hauteur) + 1;
}

// equilibrage : https://stephane.glondu.net/projets/tipe/transparents.pdf

void free_abr (abr a) {
    if (a->droite != NULL) {
        free_abr(a->droite);
    }
    if (a->gauche != NULL) {
        free_abr(a->gauche);
    }
    
    free(a);
}