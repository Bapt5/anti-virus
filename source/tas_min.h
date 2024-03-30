#ifndef _TAS_MIN_
#define _TAS_MIN_

#include "librairies.h"

struct tas_min_content {
    void* valeur;
    int priorite;
};
typedef struct tas_min_content tas_min_content;

struct tas_min_s {
    tas_min_content* tab;
    int taille;
    int capacite;
};
typedef struct tas_min_s tas_min;

tas_min creer_tas_min(int);
bool est_vide_tas_min(tas_min);
void inserer_tas_min(void*, int, tas_min*);
void* extraire_min_tas_min(tas_min*);
void free_tas_min(tas_min*, void (*)(void*));


#endif