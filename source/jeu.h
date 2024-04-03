#ifndef _JEU_
#define _JEU_

#include "librairies.h"
#include "piece.h"

#include "liste_chaine.h"

struct jeu_s
{
    int taille;
    position sortie;
    int nb_pieces;
    piece** pieces;
    int piece_a_sortir; // position de la pièce à sortir dans id_pieces
    int* id_pieces;  // associe l'id de la pièce à sa position dans le tableau de pièces
};
typedef struct jeu_s jeu;

/**
    Paramètres:
        - jeu_: jeu

    Retourne:
        - grille du jeu
        - NULL si l'allocation a échoué
*/
int** construire_grille(jeu);

/**
    Paramètres:
        - jeu_: jeu
        - grille: grille du jeu

    Libère la mémoire de la grille
*/
void free_grille(jeu, int**);


/**
    Paramètres:
        - filename: nom du fichier
        Le fichier doit être de la forme:
            taille: int
            nb_pieces: int
            sortie : int int
            (id - 1) piece a sortir : int
            1e ligne: int
            2e ligne: int int int
            ...

    Retourne:
        - l'instance de jeu décrit dans le fichier
        - NULL si une allocation a échoué
*/
jeu* creer_jeu(char*);

/**
    Paramètres:
        - jeu_: jeu
        - filename: nom du fichier
    Exporte le jeu dans le fichier
*/
void export_jeu(jeu, char*);

/**
    Paramètres:
        - jeu_: jeu
    
    Affiche le jeu dans le terminal
*/
void affiche_jeu(jeu);

/**
    Paramètres:
        - jeu_: jeu
        - publisher: socket

    Envoie la taille du jeu, le nombre de pièces et les caractéristiques des pièces sur la socket
    Envoie les positions des pièces sur la socket
*/
void affiche_jeu_python(jeu, void*);

/**
    Paramètres:
        - sol: liste de jeu
        - publisher: socket

    Envoie la taille du jeu, le nombre de pièces et les caractéristiques des pièces sur la socket
    Envoie les positions des pièces sur la socket pour chaque étape de la solution
*/
void affiche_sol_python(liste, int, void*);

/**
    Paramètres:
        - jeu_: jeu

    Libère la mémoire allouée au jeu
*/
void free_jeu(jeu*);

/**
    Paramètres:
        - jeu_: jeu

    Retourne:
        - copie du jeu
        - NULL si une allocation a échoué
*/
jeu* copie_jeu(jeu*);

/**
    Paramètres:
        - jeu_: le jeu

    Retourne:
        - true si la pièce à sortir est sortie, false sinon
*/
bool est_resolu(jeu);

/**
    Paramètres:
        - jeu_: jeu
        - bool: pointeur vers un booléen qui sera mis à true si tout s'est bien passé

    Retourne:
        - true si quand on place les pièces, elles ne se chevauchent pas et ne sortent pas du jeu
        - false sinon

    Complexité: O(nombre de piece * nombre de case par piece)
    // TODO: A améliorer si possible
*/
bool est_valide_jeu(jeu, bool*);

/**
    Paramètres:
        - jeu_: le jeu
    
    Retourne:
        - un entier représentant le hash du jeu
*/ 
unsigned long long hash_jeu(jeu);

/**
    Paramètres:
        - jeu_: jeu

    Mélange les pièces du jeu
*/
void melanger_pieces_jeu (jeu*);

#endif