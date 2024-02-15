#include "resolution.h"

enum Direction_e {
    HAUT = 0,
    BAS = 1,
    GAUCHE = 2,
    DROITE =3
};
typedef enum Direction_e Direction;


void voisins_piece_dir (jeu jeu_, int id_piece, Direction dir, liste* l, bool* deja_mise) {
    /*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut les voisins
        - dir: la direction dans laquelle on veut les voisins
        - l: la liste dans laquelle on va ajouter les id des pièces voisines
        - deja_mise: un tableau de booléens de taille jeu_.nb_pieces
            indiquant si la pièce correspondante a déjà été mise dans la liste

    Ajoute à l les id des pièces voisines de la pièce id_piece dans la direction dir
    */

    assert(id_piece >= 1 && id_piece <= jeu_.nb_pieces);

    int** grille = construire_grille(jeu_);

    piece* p = jeu_.pieces[jeu_.id_pieces[id_piece - 1]];

    for (int k = 0; k < p->taille; k += 1) {
        int i = p->pos.i + p->positions_rel[k].i;
        int j = p->pos.j + p->positions_rel[k].j;

        if (dir == HAUT) {
            i -= 1;
        } else if (dir == BAS) {
            i += 1;
        } else if (dir == GAUCHE) {
            j -= 1;
        } else if (dir == DROITE) {
            j += 1;
        }

        if (est_valide((position){.i = i, .j = j}, jeu_.taille)) {
            int id = grille[i][j];
            if (id != 0 && id != p->id && !deja_mise[id - 1]) {
                int* id_p = malloc(sizeof(int));
                *id_p = id;

                *l = ajouter_tete_liste(id_p, *l);
                deja_mise[id - 1] = true;
            }
        }
    }

    free_grille(jeu_, grille);
}

liste voisins_piece (jeu jeu_, int id_piece) {
    /*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut les voisins

    Retourne:
        - une liste d'entiers contenant les id des pièces voisines de la pièce id_piece
    */

    liste l = creer_liste();
    bool* deja_mise = calloc(jeu_.nb_pieces, sizeof(bool));

    for (int dir = 0; dir < 4; dir += 1) {
        voisins_piece_dir(jeu_, id_piece, dir, &l, deja_mise);
    }
    
    free(deja_mise);

    return l;
}

void position_accessible_dir (jeu jeu_, int n, int id_pieces[n], Direction dir, file* f, liste path) {
    /* 
    Paramètres:
        - jeu_: le jeu
        - n: le nombre de pièces à essayer de bouger
        - id_pieces: un tableau contenant les id des pièces à essayer de bouger
        - dir: la direction dans laquelle on veut bouger les pièces
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - path: le chemin menant à la grille actuelle

    Ajoute à f les grilles accessibles en bougeant les pièces
        id_pieces[0], ..., id_pieces[n - 1] dans la direction dir
    */

    for (int i = 0; i < n; i += 1) {
        assert(id_pieces[i] >= 1 && id_pieces[i] <= jeu_.nb_pieces);
        assert(jeu_.pieces[jeu_.id_pieces[id_pieces[i] - 1]]->bougeable);
    }

    jeu* jeu_copie = copie_jeu(&jeu_);

    for (int i = 1; i < jeu_.taille; i += 1) {
        for (int k = 0; k < n; k += 1) {
            if (dir == HAUT) {
                jeu_copie->pieces[jeu_copie->id_pieces[id_pieces[k] - 1]]->pos.i -= 1;
            } else if (dir == BAS) {
                jeu_copie->pieces[jeu_copie->id_pieces[id_pieces[k] - 1]]->pos.i += 1;
            } else if (dir == GAUCHE) {
                jeu_copie->pieces[jeu_copie->id_pieces[id_pieces[k] - 1]]->pos.j -= 1;
            } else if (dir == DROITE) {
                jeu_copie->pieces[jeu_copie->id_pieces[id_pieces[k] - 1]]->pos.j += 1;
            }
        }

        if (est_valide_jeu(*jeu_copie)) {
            jeu* jeu2 = copie_jeu(jeu_copie);

            enfiler(ajouter_tete_liste(jeu2, copie_liste(path, copie_jeu)), f);
        } else {
            break;
        }

    }

    free_jeu(jeu_copie);
}

void position_accessible (jeu jeu_, int n, int id_pieces[n], file* f, liste path) {
    /*
    Paramètres: 
        - jeu_: le jeu
        - n: le nombre de pièces à essayer de bouger
        - id_pieces: un tableau contenant les id des pièces à essayer de bouger
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - path: le chemin menant à la grille actuelle
    Ajoute à f les grilles accessibles en bougeant les pièces id_pieces[0], ..., id_pieces[n - 1]
    */
    
    for (int dir = 0; dir < 4; dir += 1) {
        position_accessible_dir(jeu_, n, id_pieces, dir, f, path);
    }
    
}

void bouge_voisins (jeu jeu_, int id_piece, file* f, liste path) {
    /*
    Paramètres:
        - jeu_: le jeu
        - id_piece: l'id de la pièce dont on veut bouger les voisins
        - f: la file dans laquelle on va ajouter les grilles accessibles
        - path: le chemin menant à la grille actuelle

    Ajoute à f les grilles accessibles en bougeant les voisins de la pièce id_piece
    */

    liste voisins_l = voisins_piece(jeu_, id_piece);
    int nb_voisins_bougeables = 0;
    for (liste l_i = voisins_l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
        if (jeu_.pieces[jeu_.id_pieces[*(int*)tete_liste(l_i) - 1]]->bougeable) {
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
        if (jeu_.pieces[jeu_.id_pieces[*id_voisin - 1]]->bougeable) {
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

            position_accessible(jeu_, k + 1, id_pieces, f, path);
            free(id_pieces);

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

liste v1 (jeu jeu_, int* nb_explo) {
    liste resultat = creer_liste();

    file f = creer_file();

    jeu* g_copie = copie_jeu(&jeu_);

    enfiler(ajouter_tete_liste(g_copie, resultat), &f);

    while (!est_vide_file(f)) {
        liste path = defiler(&f);
        jeu* jeu2 = tete_liste(path);

        (*nb_explo)++;

        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    position_accessible(*jeu2, 1, (int[]){id_piece}, &f, path);

                    bouge_voisins(*jeu2, id_piece, &f, path);
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

liste v2 (jeu jeu_, int* nb_explo) {
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

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    position_accessible(*jeu2, 1, (int[]){id_piece}, &f, path);

                    bouge_voisins(*jeu2, id_piece, &f, path);
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

liste v3 (jeu jeu_, int* nb_explo) {
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

        (*nb_explo)++;

        // printf("%llu\n", hash);
        // affiche_jeu(*jeu2);

        if (est_resolu(*jeu2)) {
            resultat = inverser_liste(path, NULL);

            free_liste(path, NULL);
            break;
        } else {
            for (int id_piece = 1; id_piece <= jeu2->nb_pieces; id_piece += 1) {
                int profondeur = longueur_liste(path) - 1;

                printf("%d;%d\n", f.taille, profondeur); //TODO: à supprimer

                if (jeu2->pieces[jeu2->id_pieces[id_piece - 1]]->bougeable) {
                    position_accessible(*jeu2, 1, (int[]){id_piece}, &f, path);

                    bouge_voisins(*jeu2, id_piece, &f, path);
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

liste v4 (jeu jeu_, int* nb_explo) {
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

        (*nb_explo)++;

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
                piece* p = jeu2->pieces[i];
                int id_piece = p->id;

                if (p->bougeable) {
                    position_accessible(*jeu2, 1, (int[]){id_piece}, &f, path);

                    bouge_voisins(*jeu2, id_piece, &f, path);
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

// TODO: bouge voisins perte de temps dans certains cas