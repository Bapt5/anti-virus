#ifndef _CREATE_
#define _CREATE_

#include "librairies.h"
#include "jeu.h"
#include "piece.h"

#include "abr.h"
#include "file.h"
#include "liste_chaine.h"
#include "recherche.h"

#include <time.h>

/**
    Paramètres:
        - taille: la taille du jeu
        - nb_pieces: le nombre de pièces à générer

    Retourne:
        - un jeu aléatoire de taille taille avec nb_pieces pièces 
        la pièce à sortir est à la sortie du jeu
        - NULL si une allocation a échoué
        
*/
jeu* creation_jeu_random(int taille, int nb_pieces);

/**
    Paramètres:
        - j: un pointeur vers un jeu
        - nb_coups: le nombre de coups à effectuer
        - timeout: le temps maximum pour mélanger le jeu
    Postconditions:
        - bouge les pièces sur la grille de manière aléatoire
        - retourne le jeu mélangé
        - NULL si une allocation a échoué
*/
jeu* melanger_jeu(jeu* j, int nb_coups, int timeout);


#endif