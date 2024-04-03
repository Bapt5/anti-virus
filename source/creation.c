#include "creation.h"

jeu* creation_jeu_random(int taille, int nb_pieces) {
    // pièces du jeu
    int nb_pieces_jeu = 7;
    piece** pieces = malloc(nb_pieces_jeu * sizeof(piece*));

    if (pieces == NULL){  // Si l'allocation a échoué on retourne NULL
        return NULL;
    }

    pieces[0] = creer_piece(2, 
                    (position[]){
                        {.i = 0, .j = 0}, 
                        {.i = 0, .j = 1}}, 
                    true);
    pieces[1] = creer_piece(3, 
                    (position[]){
                        {.i = 0, .j = 0}, 
                        {.i = 0, .j = 1},
                        {.i = 1, .j = 0}},
                    true);
    pieces[2] = creer_piece(2,
                    (position[]){
                        {.i = 0, .j = 0},
                        {.i = 1, .j = 1}},
                    true);
    pieces[3] = creer_piece(3,
                    (position[]){
                        {.i = 0, .j = 0},
                        {.i = 1, .j = 1},
                        {.i = 2, .j = 2}},
                    true);
    pieces[4] = creer_piece(3,
                    (position[]){
                        {.i = 0, .j = 0},
                        {.i = 1, .j = -1},
                        {.i = 1, .j = 1}},
                    true);
    pieces[5] = creer_piece(3,
                    (position[]){
                        {.i = 0, .j = 0},
                        {.i = 1, .j = 0},
                        {.i = 2, .j = -1}},
                    true);
    pieces[6] = creer_piece(1,
                    (position[]){
                        {.i = 0, .j = 0}},
                    false);

    if (pieces[0] == NULL || pieces[1] == NULL || pieces[2] == NULL || pieces[3] == NULL || pieces[4] == NULL || pieces[5] == NULL || pieces[6] == NULL){
        for (int i = 0; i < nb_pieces_jeu; i++){
            free_piece(pieces[i]);
        }
        free(pieces);

        return NULL;
    }

    jeu* j = malloc(sizeof(jeu));

    if (j == NULL){  // Si l'allocation a échoué on libère la mémoire déjà allouée et on retourne NULL
        for (int i = 0; i < nb_pieces_jeu; i++){
            free_piece(pieces[i]);
        }
        free(pieces);

        return NULL;
    }

    j->taille = taille;
    j->sortie = (position){.i = 0, .j = taille/2};
    j->nb_pieces = 1;
    j->pieces = malloc(nb_pieces * sizeof(piece*));

    if (j->pieces == NULL){  // Si l'allocation a échoué on libère la mémoire déjà allouée et on retourne NULL
        for (int i = 0; i < nb_pieces_jeu; i++){
            free_piece(pieces[i]);
        }
        free(pieces);
        free(j);

        return NULL;
    }

    j->id_pieces = malloc(nb_pieces * sizeof(int));

    if (j->id_pieces == NULL){  // Si l'allocation a échoué on libère la mémoire déjà allouée et on retourne NULL
        for (int i = 0; i < nb_pieces_jeu; i++){
            free_piece(pieces[i]);
        }
        free(pieces);
        free(j->pieces);
        free(j);

        return NULL;
    }

    // initialisation de la piece sortir
    j->pieces[0] = creer_piece(2, 
                    (position[]){
                        {.i = 0, .j = 0}, 
                        {.i = 1, .j = 0}}, 
                    true);
    j->pieces[0]->pos.i = j->sortie.i;
    j->pieces[0]->pos.j = j->sortie.j;
    j->pieces[0]->id = 1;
    j->id_pieces[0] = 0;
    j->piece_a_sortir = 0;

    for (int i = 1; i < nb_pieces; i++){
        j->id_pieces[i] = i;
        // j->pieces[i] = creation_piece_random(taille);
        j->pieces[i] = rotation_piece(pieces[rand()%nb_pieces_jeu], rand()%4);

        if (j->pieces[i] == NULL){  // Si l'allocation a échoué on libère la mémoire déjà allouée et on retourne NULL
            for (int i = 0; i < nb_pieces_jeu; i++){
                free_piece(pieces[i]);
            }
            free(pieces);
            for (int i = 0; i < j->nb_pieces; i++){
                free_piece(j->pieces[i]);
            }
            free(j->pieces);
            free(j->id_pieces);
            free(j);

            return NULL;
        }

        j->pieces[i]->id = i + 1;
        j->pieces[i]->pos.i = rand()%taille;
        j->pieces[i]->pos.j = rand()%taille;
        j->nb_pieces++;

        // on verifie que le jeu est toujours valide
        bool succes = true;
        if (!est_valide_jeu(*j, &succes)){
            free_piece(j->pieces[i]);
            j->nb_pieces--;
            i--;
        }

        if (!succes){  // Si l'allocation a échoué on libère la mémoire déjà allouée et on retourne NULL
            for (int i = 0; i < nb_pieces_jeu; i++){
                free_piece(pieces[i]);
            }
            free(pieces);
            for (int i = 0; i < j->nb_pieces; i++){
                free_piece(j->pieces[i]);
            }
            free(j->pieces);
            free(j->id_pieces);
            free(j);

            return NULL;
        }
    }

    for (int i = 0; i < nb_pieces_jeu; i++){
        free_piece(pieces[i]);
    }
    free(pieces);

    return j;
}

liste file_to_liste (file f) {
    liste l = creer_liste();
    while (!est_vide_file(f)) {
        liste deplacement_l = defiler(&f);
        jeu* deplacement = tete_liste(deplacement_l);
        free_liste(deplacement_l, NULL);

        l = ajouter_tete_liste(deplacement, l);
        assert(l != NULL);
    }
    return l;
}

jeu* melanger_jeu(jeu* jeu_, int nb_coups, int timeout) {
    time_t start = time(NULL);
    int fail = 0;
    
    jeu* j = copie_jeu(jeu_);

    abr vus = creer_abr();

    if (j == NULL || vus == NULL) {
        return NULL;
    }

    unsigned long long hash = hash_jeu(*j);
    if(!ajouter_abr(&vus, hash)){
        free_jeu(j);
        free_abr(vus);

        return NULL;
    }

    for (int i = 0; i < nb_coups; i++) {
        if (time(NULL) - start > timeout) {
            printf("Timeout\n");
            break;
        }
        if (fail > jeu_->nb_pieces * jeu_->taille * jeu_->taille) {
            printf("Fail\n");
            break;
        }

        int id_piece = rand()%j->nb_pieces + 1;

        if (!j->pieces[jeu_->id_pieces[id_piece - 1]]->bougeable) {
            i--;
            fail++;
            continue;
        }

        file f = creer_file();
        if(!position_accessible(*j, 1, (int[]){id_piece}, &f, enfiler, creer_liste())){
            free_file(&f, free_liste);
            i--;
            fail++;
            continue;
        }

        liste deplacements_possibles = file_to_liste(f);

        int nb_deplacements = longueur_liste(deplacements_possibles);

        if (nb_deplacements == 0){
            free_liste(deplacements_possibles, free_grille);
            i--;
            fail++;
            continue;
        }

        int idc_deplacement = rand()%nb_deplacements;
        jeu* deplacement = i_ieme_element_liste(deplacements_possibles, idc_deplacement);
        unsigned long long hash_deplacement = hash_jeu(*deplacement);

        if (trouver_abr(vus, hash_deplacement)) {
            free_liste(deplacements_possibles, free_jeu);
            i--;
            fail++;
            continue;
        }
        
        free_jeu(j);
        j = copie_jeu(deplacement);

        if (j == NULL) {
            free_liste(deplacements_possibles, free_jeu);
            free_abr(vus);

            return NULL;
        }

        fail = 0;

        if (!ajouter_abr(&vus, hash_deplacement)) {
            free_liste(deplacements_possibles, free_jeu);
            free_abr(vus);

            return NULL;
        }
        free_liste(deplacements_possibles, free_jeu);
    }

    free_abr(vus);

    return j;
}