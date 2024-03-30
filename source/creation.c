#include "creation.h"

piece* creation_piece_random(int taille_jeu){
    /*
    Paramètres:
        - taille_jeu: la taille du jeu
    Retourne:
        - une pièce de taille entre 2 et taille_jeu positionné aléatoirement sur la grille
    */

    piece* p = malloc(sizeof(piece));
    p->taille = rand()%(taille_jeu - 1) + 2;

    p->positions_rel = malloc(p->taille * sizeof(position));

    p->positions_rel[0].i = 0;
    p->positions_rel[0].j = 0;

    for (int i = 1; i < p->taille; i++){
        int position_racroche = rand()%i;

        p->positions_rel[i].i = p->positions_rel[position_racroche].i + rand()%3 - 1;
        p->positions_rel[i].j = p->positions_rel[position_racroche].j + rand()%3 - 1;
    }

    if (p->taille == 1){
        p->bougeable = false;
    } else {
        p->bougeable = true;
    }

    p->pos.i = rand()%taille_jeu;
    p->pos.j = rand()%taille_jeu;

    return p;
}

piece* create_piece_sortir (jeu* j) {
    /*
    Paramètres:
        - j: un pointeur vers un jeu
    Retourne:
        - une pièce de taille 2 qui est à la sortie du jeu
    */
    piece* p = malloc(sizeof(piece));
    p->taille = 2;
    p->positions_rel = malloc(p->taille * sizeof(position));

    p->positions_rel[0].i = 0;
    p->positions_rel[0].j = 0;
    p->positions_rel[1].i = 1;
    p->positions_rel[1].j = 0;
    // probleme piece sortir

    p->bougeable = true;

    p->pos.i = j->sortie.i;
    p->pos.j = j->sortie.j;

    return p;
}

jeu* creation_jeu_random(int taille, int nb_pieces) {
    jeu* j = malloc(sizeof(jeu));
    j->taille = taille;
    j->sortie = (position){.i = 0, .j = taille/2};
    j->nb_pieces = 1;
    j->pieces = malloc(nb_pieces * sizeof(piece*));
    j->id_pieces = malloc(nb_pieces * sizeof(int));

    // initialisation de la piece sortir
    j->pieces[0] = create_piece_sortir(j);
    j->pieces[0]->id = 1;
    j->id_pieces[0] = 0;
    j->piece_a_sortir = 0;

    for (int i = 1; i < nb_pieces; i++){
        j->id_pieces[i] = i;
        j->pieces[i] = creation_piece_random(taille);
        j->pieces[i]->id = i + 1;
        j->nb_pieces++;

        // on verifie que le jeu est toujours valide
        if (!est_valide_jeu(*j)){
            free_piece(j->pieces[i]);
            j->nb_pieces--;
            i--;
        }
    }

    return j;
}

liste file_to_liste (file f) {
    liste l = creer_liste();
    while (!est_vide_file(f)) {
        liste deplacement_l = defiler(&f);
        jeu* deplacement = tete_liste(deplacement_l);
        free_liste(deplacement_l, NULL);

        l = ajouter_tete_liste(deplacement, l);
    }
    return l;
}

jeu* melanger_jeu(jeu* jeu_, int nb_coups) {
    
    jeu* j = copie_jeu(jeu_);
    abr vus = creer_abr();

    unsigned long long hash = hash_jeu(*j);
    ajouter_abr(&vus, hash);

    for (int i = 0; i < nb_coups; i++){
        int id_piece = rand()%j->nb_pieces + 1;

        file f = creer_file();
        position_accessible(*j, 1, (int[]){id_piece}, &f, enfiler, creer_liste());

        liste deplacements_possibles = file_to_liste(f);

        int nb_deplacements = longueur_liste(deplacements_possibles);

        if (nb_deplacements == 0){
            free_liste(deplacements_possibles, free_grille);
            // i--;
            continue;
        }

        int idc_deplacement = rand()%nb_deplacements;
        jeu* deplacement = i_ieme_element_liste(deplacements_possibles, idc_deplacement);
        unsigned long long hash_deplacement = hash_jeu(*deplacement);

        if (trouver_abr(vus, hash_deplacement)) {
            free_liste(deplacements_possibles, free_jeu);
            // i--;
            continue;
        }
        
        free_jeu(j);
        j = copie_jeu(deplacement);

        ajouter_abr(&vus, hash_deplacement);
        free_liste(deplacements_possibles, free_jeu);
    }

    free_abr(vus);

    return j;
}