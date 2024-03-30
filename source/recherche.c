#include "recherche.h"


void voisins_piece_dir (jeu jeu_, int id_piece, Direction dir, liste* l, bool* deja_mise) {
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
    liste l = creer_liste();
    bool* deja_mise = calloc(jeu_.nb_pieces, sizeof(bool));

    for (int dir = 0; dir < 4; dir += 1) {
        voisins_piece_dir(jeu_, id_piece, dir, &l, deja_mise);
    }
    
    free(deja_mise);

    return l;
}

void position_accessible_dir (jeu jeu_, int n, int id_pieces[n], Direction dir, void* f, void (*ajout)(void*, void*), liste path) {
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

            ajout(ajouter_tete_liste(jeu2, copie_liste(path, copie_jeu)), f);
        } else {
            break;
        }

    }

    free_jeu(jeu_copie);
}

void position_accessible (jeu jeu_, int n, int id_pieces[n], void* f, void (*ajout)(void*, void*), liste path) {
    for (int dir = 0; dir < 4; dir += 1) {
        position_accessible_dir(jeu_, n, id_pieces, dir, f, ajout, path);
    }
    
}

void bouge_voisins (jeu jeu_, int id_piece, void* f, void (*ajout)(void*, void*), liste path) {
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

            position_accessible(jeu_, k + 1, id_pieces, f, ajout, path);
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