#include "resolution.h"

liste v0 (jeu jeu_, int* nb_explo) {
    liste resultat = creer_liste();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    if (g_copie == NULL) {  // Si l'allocation a échoué on libère la mémoire et on arrête la fonction
        free_liste(resultat, free_jeu);
        free_file(&f, free_liste);

        return 1;
    }

    liste path_i = ajouter_tete_liste(g_copie, resultat);

    if (path_i == NULL) {  // Si l'ajout a échoué on libère la mémoire et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_file(&f, free_liste);

        return 1;
    }

    if (!enfiler(path_i, &f)) {  // Si l'ajout a échoué on libère la mémoire et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_file(&f, free_liste);

        return 1;
    }

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        (*nb_explo)++;

        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            bool succes = true;
            resultat = inverser_liste(path, NULL, &succes);

            if (!succes) {
                free_liste(path, free_jeu);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);
                return 1;
            }

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    
                    if (!position_accessible(*jeu2, 1, (int[]){id_piece}, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        return 1;
                    }

                    if (!bouge_voisins(*jeu2, id_piece, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        return 1;
                    }
                }
            }

        }

        free_liste(path, free_jeu);
    }

    // on vide la file
    while (!est_vide_file(f)) {
        free_liste(defiler(&f), free_jeu);
    }

    return resultat;
}

liste v1 (jeu jeu_, int* nb_explo) {
    liste resultat = creer_liste();

    liste vus = creer_liste();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    if (g_copie == NULL) {  // Si l'allocation a échoué on libère la mémoire et on arrête la fonction
        free_liste(resultat, free_jeu);
        free_liste(vus, free);
        free_file(&f, free_liste);

        return 1;
    }

    liste path_i = ajouter_tete_liste(g_copie, resultat);

    if (path_i == NULL) {  // Si l'ajout a échoué on libère la mémoire et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_liste(vus, free);
        free_file(&f, free_liste);

        return 1;
    }

    if(!enfiler(path_i, &f)) {  // Si l'ajout a échoué on libère la mémoire et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_liste(vus, free);

        return 1;
    }

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (appartient_liste(&hash, vus, egalite_unsigned_long_long)) {
            free_liste(path, free_jeu);

            continue;
        }

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            bool succes = true;
            resultat = inverser_liste(path, NULL, &succes);

            if (!succes) {
                free_liste(path, free_jeu);
                free_liste(vus, free);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);

                return 1;
            }

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    if (!position_accessible(*jeu2, 1, (int[]){id_piece}, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        free_liste(vus, free);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);

                        return 1;
                    }

                    if (!bouge_voisins(*jeu2, id_piece, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        free_liste(vus, free);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);

                        return 1;
                    }
                }
            }

            // on marque comme vu cette grille
            unsigned long long* hash_p = malloc(sizeof(unsigned long long));
            
            if (hash_p == NULL) {  // Si l'allocation a échoué on libère la mémoire et on arrête la fonction
                free_liste(vus, free);
                free_liste(path, free_jeu);

                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);

                return 1;
            }

            *hash_p = hash;
            vus = ajouter_tete_liste(hash_p, vus);
            if (vus == NULL) {  // Si l'ajout a échoué on libère la mémoire et on arrête la fonction
                free_liste(vus, free);
                free_liste(path, free_jeu);

                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);

                return 1;
            }
        }

        free_liste(path, free_jeu);
    }

    free_liste(vus, free);
    // on vide la file
    while (!est_vide_file(f)) {
        free_liste(defiler(&f), free_jeu);
    }

    return resultat;
}

liste v2 (jeu jeu_, int* nb_explo) {
    liste resultat = creer_liste();

    abr vus = creer_abr();

    if (vus == NULL) {  // Si l'allocation a échoué on arrête la fonction
        return 1;
    }

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    if (g_copie == NULL) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_abr(vus);
        free_liste(resultat, free_jeu);
        free_file(&f, free_liste);

        return 1;
    }

    liste path_i = ajouter_tete_liste(g_copie, resultat);

    if (path_i == NULL) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_file(&f, free_liste);

        return 1;
    }

    if(!enfiler(path_i, &f)) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_file(&f, free_liste);

        return 1;
    }

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (trouver_abr(vus, hash)) {
            free_liste(path, free_jeu);

            continue;
        }

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            bool succes = true;
            resultat = inverser_liste(path, NULL, &succes);

            if (!succes) {
                free_liste(path, free_jeu);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                int profondeur = longueur_liste(path) - 1;

                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    if (!position_accessible(*jeu2, 1, (int[]){id_piece}, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }

                    if (!bouge_voisins(*jeu2, id_piece, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }
                }
            }

            // on marque comme vu cette grille
            if (!ajouter_abr(&vus, hash)) {
                free_liste(path, free_jeu);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }
        }

        free_liste(path, free_jeu);
    }

    free_abr(vus);
    // on vide la file
    while (!est_vide_file(f)) {
        free_liste(defiler(&f), free_jeu);
    }

    return resultat;
}

liste v3 (jeu jeu_, int* nb_explo) {
    // initialisation random
    srand(time(NULL));

    liste resultat = creer_liste();

    abr vus = creer_abr();

    if (vus == NULL) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_liste(resultat, free_jeu);

        return 1;
    }

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    if (g_copie == NULL) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_abr(vus);
        free_liste(resultat, free_jeu);
        free_file(&f, free_liste);

        return 1;
    }

    liste path_i = ajouter_tete_liste(g_copie, resultat);

    if (path_i == NULL) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_file(&f, free_liste);

        return 1;
    }

    if(!enfiler(path_i, &f)) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_file(&f, free_liste);

        return 1;
    }

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (trouver_abr(vus, hash)) {
            free_liste(path, free_jeu);

            continue;
        }

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            bool succes = true;
            resultat = inverser_liste(path, NULL, &succes);

            if (!succes) {
                free_liste(path, free_jeu);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }

            free_liste(path, NULL);
            break;
        } else {
            // on mélange l'ordre des pièces
            melanger_pieces_jeu(jeu2);

            for (int i = 0; i < jeu2->nb_pieces; i += 1) {
                piece* p = jeu2->pieces[i];
                int id_piece = p->id;

                if (p->bougeable) {
                    if (!position_accessible(*jeu2, 1, (int[]){id_piece}, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }

                    if (!bouge_voisins(*jeu2, id_piece, &f, enfiler, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_file(f)) {
                            free_liste(defiler(&f), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }
                }
            }

            // on marque comme vu cette grille
            if (!ajouter_abr(&vus, hash)) {
                free_liste(path, free_jeu);
                while (!est_vide_file(f)) {
                    free_liste(defiler(&f), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }
        }

        free_liste(path, free_jeu);
    }

    free_abr(vus);
    // on vide la file
    while (!est_vide_file(f)) {
        free_liste(defiler(&f), free_jeu);
    }

    return resultat;

}

int distance (position p1, position p2) {
    /*
    Paramètres:
        - p1: une position
        - p2: une position
    
    Retourne:
        - la distance entre p1 et p2
    */

    return (p1.i - p2.i) * (p1.i - p2.i) + (p1.j - p2.j) * (p1.j - p2.j);
}

int heuristique1 (jeu jeu_) {
    piece* piece_a_sortir = jeu_.pieces[jeu_.id_pieces[jeu_.piece_a_sortir]];

    int d = distance(piece_a_sortir->pos, jeu_.sortie);

    return d;
}

int heuristique2 (jeu jeu_) {
    piece* piece_a_sortir = jeu_.pieces[jeu_.id_pieces[jeu_.piece_a_sortir]];

    liste l = creer_liste();
    bool* deja_mise = calloc(jeu_.nb_pieces, sizeof(bool));

    bool succes = true;  // boolen pour savoir si l'ajout des voisins a réussi

    if (piece_a_sortir->pos.i > jeu_.sortie.i) {
        succes = succes && voisins_piece_dir(jeu_, piece_a_sortir->id, HAUT, &l, deja_mise);
    } else if (piece_a_sortir->pos.i < jeu_.sortie.i) {
        succes = succes && voisins_piece_dir(jeu_, piece_a_sortir->id, BAS, &l, deja_mise);
    }
    
    if (piece_a_sortir->pos.j > jeu_.sortie.j) {
        succes = succes && voisins_piece_dir(jeu_, piece_a_sortir->id, GAUCHE, &l, deja_mise);
    } else if (piece_a_sortir->pos.j < jeu_.sortie.j) {
        succes = succes && voisins_piece_dir(jeu_, piece_a_sortir->id, DROITE, &l, deja_mise);
    }

    int nb_bloquants = longueur_liste(l);

    free_liste(l, free);
    free(deja_mise);

    if (!succes) {
        return -1;
    }

    return nb_bloquants;    
}

int heuristique3 (jeu jeu_) {
    piece* piece_a_sortir = jeu_.pieces[jeu_.id_pieces[jeu_.piece_a_sortir]];

    int d_sortie = distance(piece_a_sortir->pos, jeu_.sortie);

    int prio = 0;

    for (int i = 0; i < jeu_.nb_pieces; i += 1) {
        piece* p = jeu_.pieces[i];

        if (distance(p->pos, jeu_.sortie) < d_sortie) {
            prio += 1;
        }
    }

    return prio;
}

bool insere_heuristique_fil (liste path, tas_min* tas, int (*heuristique)(jeu)) {
    /*
    Paramètres:
        - path: le chemin menant à la grille
        - tas: le tas dans lequel on va ajouter la grille
        - heuristique: la fonction d'heuristique à utiliser
    Ajoute à tas la grille à la fin de path avec une priorité égale à l'heuristique
    */   

    jeu* jeu_ = tete_liste(path);

    int prio = heuristique(*jeu_);

    if (prio == -1) {
        return false;
    }

    if (!inserer_tas_min(path, prio, tas)) {
        return false;
    }
    
    return true;
}


liste v_heuristique (jeu jeu_, int* nb_explo, int (*heuristique)(jeu)) {
    /*
    Paramètres:
        - jeu_: le jeu
        - nb_explo: un pointeur vers un entier dans lequel on va stocker le nombre d'explorations
        - heuristique: la fonction d'heuristique à utiliser
    
    Retourne:
        - une liste de jeu* contenant les états du jeu menant à la solution
    */

    liste resultat = creer_liste();

    abr vus = creer_abr();

    if (vus == NULL) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_liste(resultat, free_jeu);

        return 1;
    }

    int capacite_tas = jeu_.nb_pieces * jeu_.nb_pieces * jeu_.taille * jeu_.taille;
    tas_min tas = creer_tas_min(capacite_tas);

    if (tas.capacite == 0) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_abr(vus);
        free_liste(resultat, free_jeu);

        return 1;
    }

    jeu* g_copie = copie_jeu(&jeu_);

    if (g_copie == NULL) {  // Si l'allocation a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_abr(vus);
        free_liste(resultat, free_jeu);
        free_tas_min(&tas, free_liste);

        return 1;
    }

    liste path_i = ajouter_tete_liste(g_copie, resultat);

    if (path_i == NULL) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_tas_min(&tas, free_liste);

        return 1;
    }

    if(!inserer_tas_min(path_i, 0, &tas)) {  // Si l'ajout a échoué on libère la mémoire déjà allouée et on arrête la fonction
        free_jeu(g_copie);
        free_liste(resultat, free_jeu);
        free_abr(vus);
        free_tas_min(&tas, free_liste);

        return 1;
    }

    while (!est_vide_tas_min(tas)) {
        liste path = extraire_min_tas_min(&tas);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (trouver_abr(vus, hash)) {
            free_liste(path, free_jeu);

            continue;
        }

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            bool succes = true;
            resultat = inverser_liste(path, NULL, &succes);

            if (!succes) {
                free_liste(path, free_jeu);
                while (!est_vide_tas_min(tas)) {
                    free_liste(extraire_min_tas_min(&tas), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    bool partial (liste path, tas_min* tas) {  // WARNING: fonction locale (possible erreur)
                        return insere_heuristique_fil(path, tas, heuristique);
                    }

                    if (!position_accessible(*jeu2, 1, (int[]){id_piece}, &tas, partial, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_tas_min(tas)) {
                            free_liste(extraire_min_tas_min(&tas), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }

                    if (!bouge_voisins(*jeu2, id_piece, &tas, partial, path)) {
                        free_liste(path, free_jeu);
                        while (!est_vide_tas_min(tas)) {
                            free_liste(extraire_min_tas_min(&tas), free_jeu);
                        }

                        free_liste(resultat, free_jeu);
                        free_abr(vus);

                        return 1;
                    }
                }
            }

            // on marque comme vu cette grille
            if (!ajouter_abr(&vus, hash)) {
                free_liste(path, free_jeu);
                while (!est_vide_tas_min(tas)) {
                    free_liste(extraire_min_tas_min(&tas), free_jeu);
                }

                free_liste(resultat, free_jeu);
                free_abr(vus);

                return 1;
            }
        }

        free_liste(path, free_jeu);
    }

    free_abr(vus);

    // on vide le tas
    while (!est_vide_tas_min(tas)) {
        free_liste(extraire_min_tas_min(&tas), free_jeu);
    }
    free_tas_min(&tas, free_liste);

    return resultat;
}

liste v4 (jeu jeu_, int* nb_explo) {
    return v_heuristique(jeu_, nb_explo, heuristique1);
}

liste v5 (jeu jeu_, int* nb_explo) {
    return v_heuristique(jeu_, nb_explo, heuristique2);
}

liste v6 (jeu jeu_, int* nb_explo) {
    return v_heuristique(jeu_, nb_explo, heuristique3);
}


// TODO: bouge voisins perte de temps dans certains cas