#include "recherche.h"


bool voisins_piece_dir (jeu jeu_, int id_piece, Direction dir, liste* l, bool* deja_mise) {
    assert(id_piece >= 1 && id_piece <= jeu_.nb_pieces);

    int** grille = construire_grille(jeu_);

    if (grille == NULL) {  // Si la construction de la grille a échoué on renvoie false
        return false;
    }

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
                
                if (id_p == NULL) {
                    free_grille(jeu_, grille);
                    return false;
                }

                *id_p = id;

                *l = ajouter_tete_liste(id_p, *l);

                if (*l == NULL) {  // Si l'allocation a échoué on renvoie false
                    free_grille(jeu_, grille);
                    return false;
                }

                deja_mise[id - 1] = true;
            }
        }
    }

    free_grille(jeu_, grille);

    return true;
}

liste voisins_piece (jeu jeu_, int id_piece, bool* succes) {
    *succes = true;

    liste l = creer_liste();
    bool* deja_mise = calloc(jeu_.nb_pieces, sizeof(bool));

    if (deja_mise == NULL) {
        *succes = false;
        return l;
    }

    for (int dir = 0; dir < 4; dir += 1) {
        succes = succes && voisins_piece_dir(jeu_, id_piece, dir, &l, deja_mise);
    }
    
    free(deja_mise);

    return l;
}

bool position_accessible_dir (jeu jeu_, int n, int id_pieces[n], Direction dir, void* f, bool (*ajout)(void*, void*), liste path) {
    for (int i = 0; i < n; i += 1) {
        assert(id_pieces[i] >= 1 && id_pieces[i] <= jeu_.nb_pieces);
        assert(jeu_.pieces[jeu_.id_pieces[id_pieces[i] - 1]]->bougeable);
    }

    jeu* jeu_test = copie_jeu(&jeu_);

    if (jeu_test == NULL) {  // Si la copie a échoué on renvoie false
        return false;
    }

    for (int k = 0; k < n; k += 1) {
        if (dir == HAUT) {
            jeu_test->pieces[jeu_test->id_pieces[id_pieces[k] - 1]]->pos.i -= 1;
        } else if (dir == BAS) {
            jeu_test->pieces[jeu_test->id_pieces[id_pieces[k] - 1]]->pos.i += 1;
        } else if (dir == GAUCHE) {
            jeu_test->pieces[jeu_test->id_pieces[id_pieces[k] - 1]]->pos.j -= 1;
        } else if (dir == DROITE) {
            jeu_test->pieces[jeu_test->id_pieces[id_pieces[k] - 1]]->pos.j += 1;
        }
    }

    bool succes = true;
    if (est_valide_jeu(*jeu_test, &succes) && succes) {
        bool succes_copie = true;
        liste path_copie = copie_liste(path, copie_jeu, &succes_copie);

        if (!succes_copie) {  // Si la copie a échoué on renvoie false
            free_jeu(jeu_test);
            return false;
        }

        liste new_path = ajouter_tete_liste(jeu_test, path_copie);

        if (new_path == NULL) {  // Si l'allocation a échoué on renvoie false
            free_jeu(jeu_test);
            return false;
        }

        if (!ajout(new_path, f)) {  // Si l'ajout a échoué on renvoie false
            free_jeu(jeu_test);

            return false;
        }
    } else if (!succes) {  // Si l'allocation a échoué on renvoie false
        free_jeu(jeu_test);
        return false;
    } else {
        free_jeu(jeu_test);
    }

    return true;
}

bool position_accessible (jeu jeu_, int n, int id_pieces[n], void* f, bool (*ajout)(void*, void*), liste path) {
    for (int dir = 0; dir < 4; dir += 1) {
        if (!position_accessible_dir(jeu_, n, id_pieces, dir, f, ajout, path)) {
            return false;
        }
    } 
    return true;
}

bool bouge_voisins (jeu jeu_, int id_piece, void* f, bool (*ajout)(void*, void*), liste path) {
    bool succes = true;
    liste voisins_l = voisins_piece(jeu_, id_piece, &succes);

    if (!succes) {
        return false;
    }

    int nb_voisins_bougeables = 0;
    for (liste l_i = voisins_l; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
        if (jeu_.pieces[jeu_.id_pieces[*(int*)tete_liste(l_i) - 1]]->bougeable) {
            nb_voisins_bougeables += 1;
        }
    }

    if (nb_voisins_bougeables == 0) {
        free_liste(voisins_l, free);
        return true;
    }

    // on transforme la liste en tableau
    int* voisins = malloc(nb_voisins_bougeables * sizeof(int));
    
    if (voisins == NULL) {
        free_liste(voisins_l, free);
        return false;
    }

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
        
        if (choix_voisins == NULL) {
            free(voisins);
            return false;
        }
        
        for (int i = 0; i < k; i += 1) {
            choix_voisins[i] = i;
        }

        while (choix_voisins[0] <= nb_voisins_bougeables - k) {
            // On bouge les k pièces
            int* id_pieces = malloc((k + 1) * sizeof(int));
            
            if (id_pieces == NULL) {
                free(choix_voisins);
                free(voisins);
                return false;
            }

            for (int i = 0; i < k; i += 1) {
                id_pieces[i] = voisins[choix_voisins[i]];
            }
            id_pieces[k] = id_piece;

            if(!position_accessible(jeu_, k + 1, id_pieces, f, ajout, path)) {
                free(choix_voisins);
                free(voisins);
                free(id_pieces);
                return false;
            }
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
    return true;
}