#include "resolution.h"

enum Direction {
    HAUT,
    BAS,
    GAUCHE,
    DROITE
};

liste voisins_piece (jeu jeu_, int id_piece) {
    /*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut les voisins

    Retourne:
        - une liste d'entiers contenant les id des pièces voisines de la pièce id_piece
    */

    assert(id_piece >= 1 && id_piece <= jeu_.nb_pieces);

    int** grille = construire_grille(jeu_);

    liste l = creer_liste();
    
    piece* p = jeu_.pieces[id_piece - 1];

    for (int k = 0; k < p->taille; k += 1) {
        int i = p->pos.i + p->positions_rel[k].i;
        int j = p->pos.j + p->positions_rel[k].j;

        position pos_bas = {.i = i + 1, .j = j};
        if (est_valide(pos_bas, jeu_.taille)) {
            int val = grille[pos_bas.i][pos_bas.j];
            if (val != 0 && val != p->id && !appartient_liste(&val, l, egalite_int)) {
                int* val_p = malloc(sizeof(int));
                *val_p = val;

                l = ajouter_tete_liste(val_p, l);
            }
        }

        position pos_haut = {.i = i - 1, .j = j};
        if (est_valide(pos_haut, jeu_.taille)) {
            int val = grille[pos_haut.i][pos_haut.j];
            if (val != 0 && val != p->id && !appartient_liste(&val, l, egalite_int)) {
                int* val_p = malloc(sizeof(int));
                *val_p = val;

                l = ajouter_tete_liste(val_p, l);
            }
        }

        position pos_droite = {.i = i, .j = j + 1};
        if (est_valide(pos_droite, jeu_.taille)) {
            int val = grille[pos_droite.i][pos_droite.j];
            if (val != 0 && val != p->id && !appartient_liste(&val, l, egalite_int)) {
                int* val_p = malloc(sizeof(int));
                *val_p = val;

                l = ajouter_tete_liste(val_p, l);
            }
        }

        position pos_gauche = {.i = i, .j = j - 1};
        if (est_valide(pos_gauche, jeu_.taille)) {
            int val = grille[pos_gauche.i][pos_gauche.j];
            if (val != 0 && val != p->id && !appartient_liste(&val, l, egalite_int)) {
                int* val_p = malloc(sizeof(int));
                *val_p = val;

                l = ajouter_tete_liste(val_p, l);
            }
        }
    }

    free_grille(jeu_, grille);

    return l;
}

liste position_accessible (jeu jeu_, int n, int id_pieces[n], Direction dir) {
    /* 
    Paramètres:
        - jeu_: le jeu
        - n: le nombre de pièces à essayer de bouger
        - id_pieces: un tableau contenant les id des pièces à essayer de bouger

    Retourne:
        - une liste de jeu* contenant les grilles accessibles en bougeant les pièces
          id_pieces[0], ..., id_pieces[n - 1]
    */

    for (int i = 0; i < n; i += 1) {
        assert(id_pieces[i] >= 1 && id_pieces[i] <= jeu_.nb_pieces);
        assert(jeu_.pieces[id_pieces[i] - 1]->bougeable);
    }

    liste l = creer_liste();

    // position* positions_init = malloc(n * sizeof(position));

    // for (int k = 0; k < n; k += 1) {
    //     positions_init[k] = jeu_.pieces[id_pieces[k] - 1]->pos;
    // }

    jeu* jeu_copie = copie_jeu(&jeu_);

    for (int i = 1; i < jeu_.taille; i += 1) {
        for (int k = 0; k < n; k += 1) {
            if (dir == HAUT) {
                jeu_copie->pieces[id_pieces[k] - 1]->pos.i -= i;
            } else if (dir == BAS) {
                jeu_copie->pieces[id_pieces[k] - 1]->pos.i += i;
            } else if (dir == GAUCHE) {
                jeu_copie->pieces[id_pieces[k] - 1]->pos.j -= i;
            } else if (dir == DROITE) {
                jeu_copie->pieces[id_pieces[k] - 1]->pos.j += i;
            }
        }

        if (est_valide_jeu(*jeu_copie)) {
            jeu* jeu2 = copie_jeu(jeu_copie);

            l = ajouter_tete_liste(jeu2, l);
        } else {
            break;
        }

    }

    // on regarde si on peut aller en bas
    // for (int i = 1; i < jeu_.taille; i += 1) {
    //     for (int k = 0; k < n; k += 1) {
    //         jeu_.pieces[id_pieces[k] - 1]->pos.i = positions_init[k].i + i;
    //         jeu_.pieces[id_pieces[k] - 1]->pos.j = positions_init[k].j;
    //     }

    //     bool valide = true;
    //     for (int k = 0; k < n; k += 1) {
    //         if (!est_valide_jeu(jeu_)) {
    //             valide = false;
    //             break;
    //         }
    //     }

    //     if (valide) {
    //         jeu* jeu2 = copie_jeu(&jeu_);

    //         l = ajouter_tete_liste(jeu2, l);
    //     } else {
    //         break;
    //     }
    // }

    // // on regarde si on peut aller en haut
    // for (int i = 1; i < jeu_.taille; i += 1) {
    //     for (int k = 0; k < n; k += 1) {
    //         jeu_.pieces[id_pieces[k] - 1]->pos.i = positions_init[k].i - i;
    //         jeu_.pieces[id_pieces[k] - 1]->pos.j = positions_init[k].j;
    //     }

    //     bool valide = true;
    //     for (int k = 0; k < n; k += 1) {
    //         if (!est_valide_jeu(jeu_)) {
    //             valide = false;
    //             break;
    //         }
    //     }

    //     if (valide) {
    //         jeu* jeu2 = copie_jeu(&jeu_);

    //         l = ajouter_tete_liste(jeu2, l);
    //     } else {
    //         break;
    //     }
    // }

    // // on regarde si on peut aller à droite
    // for (int j = 1; j < jeu_.taille; j += 1) {
    //     for (int k = 0; k < n; k += 1) {
    //         jeu_.pieces[id_pieces[k] - 1]->pos.i = positions_init[k].i;
    //         jeu_.pieces[id_pieces[k] - 1]->pos.j = positions_init[k].j + j;
    //     }

    //     bool valide = true;
    //     for (int k = 0; k < n; k += 1) {
    //         if (!est_valide_jeu(jeu_)) {
    //             valide = false;
    //             break;
    //         }
    //     }

    //     if (valide) {
    //         jeu* jeu2 = copie_jeu(&jeu_);

    //         l = ajouter_tete_liste(jeu2, l);
    //     } else {
    //         break;
    //     }
    // }

    // // on regarde si on peut aller à gauche
    // for (int j = 1; j < jeu_.taille; j += 1) {
    //     for (int k = 0; k < n; k += 1) {
    //         jeu_.pieces[id_pieces[k] - 1]->pos.i = positions_init[k].i;
    //         jeu_.pieces[id_pieces[k] - 1]->pos.j = positions_init[k].j - j;
    //     }

    //     bool valide = true;
    //     for (int k = 0; k < n; k += 1) {
    //         if (!est_valide_jeu(jeu_)) {
    //             valide = false;
    //             break;
    //         }
    //     }

    //     if (valide) {
    //         jeu* jeu2 = copie_jeu(&jeu_);

    //         l = ajouter_tete_liste(jeu2, l);
    //     } else {
    //         break;
    //     }
    // }

    // for (int k = 0; k < n; k += 1) {
    //     jeu_.pieces[id_pieces[k] - 1]->pos = positions_init[k];
    // }

    // free(positions_init);

    free_jeu(jeu_copie);

    return l;
}

void bouge_voisins (jeu* jeu_, int id_piece, file* f, liste path) {
    /*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut bouger les voisins
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - path: le chemin menant à la grille actuelle

    Ajoute à f les grilles accessibles en bougeant les voisins de la pièce id_piece
    */

    liste voisins_l = voisins_piece(*jeu_, id_piece);
    int nb_voisins_bougeables = 0;
    for (liste l_i = voisins_l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
        if (jeu_->pieces[*(int*)tete_liste(l_i) - 1]->bougeable) {
            nb_voisins_bougeables += 1;
        }
    }

    if (nb_voisins_bougeables == 0) {
        free_liste(voisins_l, free);
        return;
    }

    // on transforme la liste en tableau
    int* voisins = malloc(nb_voisins_bougeables * sizeof(int));
    int i = 0;
    for (liste l_i = voisins_l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
        int* id_voisin = tete_liste(l_i);
        if (jeu_->pieces[*id_voisin - 1]->bougeable) {
            voisins[i] = *id_voisin;
            i += 1;
        }
    }
    free_liste(voisins_l, free);

    for (int k = 1; k < nb_voisins_bougeables; k += 1) {
        // on essaye de bouger en même temps tous les ensembles de k pièces
        int* choix_voisins = malloc(k * sizeof(int));
        
        for (int i = 0; i < k; i += 1) {
            choix_voisins[i] = i;
        }

        while (choix_voisins[0] <= nb_voisins_bougeables - k) {
            // On bouge les k pièces
            int* id_pieces = malloc((k + 1) * sizeof(int));

            for (int i = 0; i < k; i += 1) {
                id_pieces[i] = voisins[choix_voisins[i]];
            }
            id_pieces[k] = id_piece;

            liste l = position_accessible(*jeu_, k + 1, id_pieces);
            free(id_pieces);

            for (liste l_i = l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
                enfiler(ajouter_tete_liste(tete_liste(l_i), copie_liste(path, copie_jeu)), f);
            }

            free_liste(l, NULL);

            // On passe au sous-ensemble suivant
            int i = k - 1;
            while (i >= 0 && choix_voisins[i] == nb_voisins_bougeables - k + i) {
                i -= 1;
            }

            if (i < 0) {
                break;
            }

            choix_voisins[i] += 1;
            for (int j = i + 1; j < k; j++) {
                choix_voisins[j] = choix_voisins[i] + j - i;
            }
        }
        free(choix_voisins);
    }
    free(voisins);
}

liste v1 (jeu jeu_) {
    liste resultat = creer_liste();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    enfiler(ajouter_tete_liste(g_copie, resultat), &f);

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[id_piece - 1]->bougeable) {
                    liste l = position_accessible(*jeu2, 1, (int[]){id_piece});

                    for (liste l_i = l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
                        enfiler(ajouter_tete_liste(tete_liste(l_i), copie_liste(path, copie_jeu)), &f);
                    }

                    free_liste(l, NULL);

                    bouge_voisins(jeu2, id_piece, &f, path);
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

unsigned long long hash_jeu(jeu jeu_) {
    /*
    Paramètres:
        - jeu_: le jeu
    
    Retourne:
        - un entier représentant le hash du jeu
    */ 

    unsigned long long hashValue = 0;
    unsigned long long exp = 1;

    unsigned long long base = (unsigned long long)jeu_.taille;

    for (int i = 0; i < jeu_.nb_pieces; i++) {
        piece* p = jeu_.pieces[jeu_.id_pieces[i]];

        hashValue += exp * (unsigned long long)p->pos.i;
        exp *= base;

        hashValue += exp * (unsigned long long)p->pos.j;
        exp *= base;
    }

    return hashValue;
}

liste v2 (jeu jeu_) {
    liste resultat = creer_liste();

    liste vus = creer_liste();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    enfiler(ajouter_tete_liste(g_copie, resultat), &f);

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (appartient_liste(&hash, vus, egalite_unsigned_long_long)) {
            free_liste(path, free_jeu);

            continue;
        }

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[id_piece - 1]->bougeable) {
                    liste l = position_accessible(*jeu2, 1, (int[]){id_piece});

                    for (liste l_i = l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
                        enfiler(ajouter_tete_liste(tete_liste(l_i), copie_liste(path, copie_jeu)), &f);
                    }

                    free_liste(l, NULL);

                    bouge_voisins(jeu2, id_piece, &f, path);
                }
            }

            // on marque comme vu cette grille
            unsigned long long* hash_p = malloc(sizeof(unsigned long long));
            *hash_p = hash;
            vus = ajouter_tete_liste(hash_p, vus);
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

liste v3 (jeu jeu_) {
    liste resultat = creer_liste();

    abr vus = creer_abr();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    enfiler(ajouter_tete_liste(g_copie, resultat), &f);

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (trouver_abr(vus, hash)) {
            free_liste(path, free_jeu);

            continue;
        }

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[id_piece - 1]->bougeable) {
                    liste l = position_accessible(*jeu2, 1, (int[]){id_piece});

                    for (liste l_i = l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
                        enfiler(ajouter_tete_liste(tete_liste(l_i), copie_liste(path, copie_jeu)), &f);
                    }

                    free_liste(l, NULL);

                    bouge_voisins(jeu2, id_piece, &f, path);
                }
            }

            // on marque comme vu cette grille
            ajouter_abr(&vus, hash);
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

liste v4 (jeu jeu_) {
    // initialisation random
    srand(time(NULL));

    liste resultat = creer_liste();

    abr vus = creer_abr();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    enfiler(ajouter_tete_liste(g_copie, resultat), &f);

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        unsigned long long hash = hash_jeu(*jeu2);

        if (trouver_abr(vus, hash)) {
            free_liste(path, free_jeu);

            continue;
        }

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            // on mélange l'ordre des pièces
            melanger_pieces_jeu(jeu2);

            for (int i = 0; i < jeu2->nb_pieces; i += 1) {
                int id_piece = i + 1;

                if (jeu2->pieces[id_piece - 1]->bougeable) {
                    liste l = position_accessible(*jeu2, 1, (int[]){id_piece});

                    for (liste l_i = l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
                        enfiler(ajouter_tete_liste(tete_liste(l_i), copie_liste(path, copie_jeu)), &f);
                    }

                    free_liste(l, NULL);

                    bouge_voisins(jeu2, id_piece, &f, path);
                }
            }

            // on marque comme vu cette grille
            ajouter_abr(&vus, hash);
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