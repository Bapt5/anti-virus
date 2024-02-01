#ifndef _ABR_
#define _ABR_

#include "librairies.h"

struct noeud_s
{
    unsigned long long valeur;
    struct noeud_s* gauche;
    struct noeud_s* droite;
    int hauteur;
};
typedef struct noeud_s noeud;
typedef struct noeud_s* abr;

abr creer_abr();
bool trouver_abr (abr, unsigned long long);
void ajouter_abr (abr*, unsigned long long);
void free_abr (abr);

#endif
