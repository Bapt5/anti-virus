#include "jeu.h"

int taille_ligne(jeu jeu_, int ligne) {
    /*
    Paramètres:
        - jeu_: jeu
        - ligne: numéro de la ligne

    Retourne: 
        - taille de la ligne
    */

    assert(ligne >= 0 && ligne < jeu_.taille);

    if (ligne < jeu_.taille/2 + 1) {
        return 2*ligne + 1;
    } else {
        return 2*(jeu_.taille - 1 - ligne) + 1;
    }
}

bool est_vide_case (jeu jeu_, int** grille, position p) {
    /*
    Paramètres:
        - jeu_: jeu
        - grille: grille du jeu
        - p: position

    Retourne:
        - true si la case est vide
        - false sinon
    */

    if (!est_valide(p, jeu_.taille)) {
        return false;
    }

    return grille[p.i][p.j] == 0;
}

int** construire_grille(jeu jeu_) {
    int** grille = malloc((jeu_.taille) * sizeof(int*));
    assert(grille != NULL);

    // on créé la grille
    for (int i = 0; i < jeu_.taille; i += 1){
        int l_ligne = taille_ligne(jeu_, i);

        grille[i] = malloc(jeu_.taille * sizeof(int));
        assert(grille[i] != NULL);

        for (int j = 0; j < jeu_.taille; j += 1) {
            if (j < (jeu_.taille - l_ligne)/2 || j >= (jeu_.taille + l_ligne)/2) {
                grille[i][j] = -1;
            } else {
                grille[i][j] = 0;
            }
        }
    }

    // on place les pièces
    for (int k = 0; k < jeu_.nb_pieces; k += 1) {
        piece* p = jeu_.pieces[k];

        for (int i = 0; i < p->taille; i += 1) {
            int i_ = p->pos.i + p->positions_rel[i].i;
            int j_ = p->pos.j + p->positions_rel[i].j;
            
            assert(est_vide_case(jeu_, grille, (position){.i = i_, .j = j_}));
            grille[i_][j_] = p->id;
        }
    }

    return grille;
}

void free_grille(jeu jeu_, int** grille) {
    for (int i = 0; i < jeu_.taille; i += 1) {
        free(grille[i]);
    }

    free(grille);
}

bool est_valide_jeu(jeu jeu_) {
    int* grille = calloc(jeu_.taille*jeu_.taille, sizeof(int));
    assert(grille != NULL);

    // on place les pièces
    for (int k = 0; k < jeu_.nb_pieces; k += 1) {
        piece* p = jeu_.pieces[k];

        for (int i = 0; i < p->taille; i += 1) {
            int i_ = p->pos.i + p->positions_rel[i].i;
            int j_ = p->pos.j + p->positions_rel[i].j;

            position pos_case = {.i = i_, .j = j_};

            if (!est_valide(pos_case, jeu_.taille) || grille[i_*jeu_.taille + j_] != 0) {
                free(grille);
                return false;
            }

            grille[i_*jeu_.taille + j_] = p->id;
        }
    }

    free(grille);

    return true;
}

jeu* creer_jeu(char* filename) {
    FILE* fichier = fopen(filename, "r");
    assert(fichier != NULL);

    int taille;
    fscanf(fichier, "%d ", &taille);
    assert(taille%2 == 1);

    // 2 sum_k=0^taille/2 (2k+1) - taille
    int nb_cases = 2*(taille/2) * (taille/2 + 1) + 1;

    int nb_pieces;
    fscanf(fichier, "%d ", &nb_pieces);

    position sortie;
    fscanf(fichier, "%d %d ", &sortie.i, &sortie.j);
    assert(est_valide(sortie, taille));

    int piece_a_sortir;
    fscanf(fichier, "%d ", &piece_a_sortir);
    assert(piece_a_sortir >= 0 && piece_a_sortir < nb_pieces);

    // 1e position: position de la pièce de base
    // positions suivantes: position relative des autres cases
    position** positions_pieces = malloc(nb_pieces * sizeof(position*));
    for (int i = 0; i < nb_pieces; i += 1) {
        positions_pieces[i] = malloc(nb_cases * sizeof(position));
    }
    
    // nombre de cases occupées par chaque pièce
    int* nb_cases_piece = malloc(nb_pieces * sizeof(int));
    for (int i = 0; i < nb_pieces; i += 1) {
        nb_cases_piece[i] = 0;
    }

    // on lit les positions des pièces
    for (int i = 0; i < taille; i += 1) {
        int l_ligne = taille_ligne((jeu){.taille = taille}, i);

        for (int j = 0; j < l_ligne; j += 1) {
            int val;
            fscanf(fichier, "%d ", &val);

            if (val > 0) {
                int id_piece = val - 1;
                if (nb_cases_piece[id_piece] == 0) {
                    positions_pieces[id_piece][0] = (position){.i = i, .j = j + (taille - l_ligne)/2};
                } else {
                    // on donne la position relative
                    position pos_rel;
                    pos_rel.i = i - positions_pieces[id_piece][0].i;
                    pos_rel.j = j + (taille - l_ligne)/2 - positions_pieces[id_piece][0].j;

                    positions_pieces[id_piece][nb_cases_piece[id_piece]] = pos_rel;
                }

                nb_cases_piece[id_piece] += 1;
            }
        }

        fscanf(fichier, "\n");
    }

    // on créé les pièces
    piece** pieces = malloc(nb_pieces * sizeof(piece));
    int* id_pieces = malloc(nb_pieces * sizeof(int));
    for (int i = 0; i < nb_pieces; i += 1) {
        position pos = positions_pieces[i][0];
        position* positions_rel = malloc(nb_cases_piece[i] * sizeof(position));
        positions_rel[0] = (position){.i = 0, .j = 0};
        for (int j = 1; j < nb_cases_piece[i]; j += 1) {
            positions_rel[j] = positions_pieces[i][j];
        }

        pieces[i] = malloc(sizeof(piece));
        assert(pieces[i] != NULL);

        pieces[i]->id = i + 1;
        pieces[i]->taille = nb_cases_piece[i];
        pieces[i]->positions_rel = positions_rel;
        pieces[i]->pos = pos;
        if (nb_cases_piece[i] > 1) {
            pieces[i]->bougeable = true;
        } else {
            pieces[i]->bougeable = false;
        }

        id_pieces[i] = i;
    }

    free(nb_cases_piece);
    for (int i = 0; i < nb_pieces; i += 1) {
        free(positions_pieces[i]);
    }
    free(positions_pieces);

    fclose(fichier);

    jeu* jeu_ = malloc(sizeof(jeu));
    assert(jeu_ != NULL);

    jeu_->taille = taille;
    jeu_->sortie = sortie;
    jeu_->nb_pieces = nb_pieces;
    jeu_->pieces = pieces;
    jeu_->piece_a_sortir = piece_a_sortir;
    jeu_->id_pieces = id_pieces;
    
    return jeu_;
}

void affiche_jeu(jeu jeu_) {
    int** grille = construire_grille(jeu_);

    for (int i = 0; i < jeu_.taille; i += 1) {

        for (int j = 0; j < jeu_.taille; j += 1) {
            if (grille[i][j] == -1) {
                printf("  ");
            } else {
                printf("%d ", grille[i][j]);
            }
        }

        printf("\n");  
    }

    free_grille(jeu_, grille);
}

void envoyer_positions_pieces(jeu jeu_, void* publisher) {
    /*
    Paramètres:
        - jeu_: jeu
        - publisher: socket

    Envoie les positions des pièces sur la socket
    */

    for (int i = 0; i < jeu_.nb_pieces; i += 1) {
        char message[100];

        // id, i, j
        sprintf(message, "position_piece %d %d %d", jeu_.pieces[i]->id, jeu_.pieces[i]->pos.i, jeu_.pieces[i]->pos.j);

        // printf("%s\n", message);
        zmq_send(publisher, message, strlen(message), 0);
    }
}

void formate_msg_piece (jeu jeu_, int indice_piece, char* message) {
    /*
    Paramètres:
        - jeu_: jeu
        - message: message à envoyer
        - publisher: socket

    Ajoute les positions relatives de la pièce au message
    */

    // piece id taille 
    sprintf(message + strlen(message), "piece %d %d", jeu_.pieces[indice_piece]->id, jeu_.pieces[indice_piece]->taille);
    
    // positions relatives
    for (int k = 0; k < jeu_.pieces[indice_piece]->taille; k += 1) {
        sprintf(message + strlen(message), " %d %d", jeu_.pieces[indice_piece]->positions_rel[k].i, jeu_.pieces[indice_piece]->positions_rel[k].j);
    }

    sprintf(message + strlen(message), "\n");
}

void envoyer_jeu_pieces(jeu jeu_, void* publisher) {
    /*
    Paramètres:
        - jeu_: jeu
        - publisher: socket

    Envoie la taille du jeu, le nombre de pièces et les caractéristiques des pièces sur la socket
    */

    // on créé le message
    // 1ere ligne: grille taille nombre de pièces (20 caractères)
    // 2e ligne: piece id taille pos1 pos2 ... ((20 + taille) caractères)
    // 3e ligne: piece id taille pos1 pos2 ... ((20 + taille) caractères)
    // ...
    char* message = malloc(jeu_.taille * jeu_.nb_pieces * (20 + jeu_.taille * jeu_.taille) * sizeof(char)); 

    // grille taille nombre de pièces
    sprintf(message, "grille %d %d\n", jeu_.taille, jeu_.nb_pieces);

    // on ajoute la pièce à sortir en premier
    formate_msg_piece(jeu_, jeu_.piece_a_sortir, message);

    // on ajoute les pièces au message
    for (int i = 0; i < jeu_.nb_pieces; i += 1) {
        if (i != jeu_.piece_a_sortir) {
            formate_msg_piece(jeu_, i, message);
        }
    }

    // printf("%s\n", message);
    zmq_send(publisher, message, strlen(message), 0);

    free(message);
}

void affiche_jeu_python(jeu jeu_, void* publisher) {
    // on envoie le jeu et les pieces
    envoyer_jeu_pieces(jeu_, publisher);
    
    // on envoie les positions des pièces
    envoyer_positions_pieces(jeu_, publisher);

    // on envoie un RESET
    // printf("RESET\n");
    zmq_send(publisher, "RESET", strlen("RESET"), 0);
}

void affiche_sol_python(liste sol, int pause, void* publisher) {

    envoyer_jeu_pieces(*(jeu*)tete_liste(sol), publisher);
    sleep(1);

    for (liste l_i = sol; !est_vide_liste(l_i); l_i = queue_liste(l_i)) {
        // on envoie un NEXT
        // printf("NEXT\n");
        zmq_send(publisher, "NEXT", strlen("NEXT"), 0);

        envoyer_positions_pieces(*(jeu*)tete_liste(l_i), publisher);
        sleep(pause);
    }

    // on envoie un RESET
    // printf("RESET\n");
    zmq_send(publisher, "RESET", strlen("RESET"), 0);
}

void free_jeu(jeu* jeu_) {
    for (int k = 0; k < jeu_->nb_pieces; k += 1){
        free_piece(jeu_->pieces[k]);
    }

    free(jeu_->pieces);
    free(jeu_->id_pieces);

    free(jeu_);
}

jeu* copie_jeu (jeu* jeu_) {
    jeu* jeu2 = malloc(sizeof(jeu));
    assert(jeu2 != NULL);

    jeu2->taille = jeu_->taille;
    jeu2->sortie = jeu_->sortie;

    // on copie les pieces
    jeu2->nb_pieces = jeu_->nb_pieces;
    jeu2->pieces = malloc(jeu2->nb_pieces * sizeof(piece));
    assert(jeu2->pieces != NULL);

    for (int k = 0; k < jeu2->nb_pieces; k += 1) {
        jeu2->pieces[k] = copie_piece(jeu_->pieces[k]);
    }

    jeu2->piece_a_sortir = jeu_->piece_a_sortir;

    // on copie les id des pieces
    jeu2->id_pieces = malloc(jeu2->nb_pieces * sizeof(int));
    assert(jeu2->id_pieces != NULL);
    
    for (int k = 0; k < jeu2->nb_pieces; k += 1) {
        jeu2->id_pieces[k] = jeu_->id_pieces[k];
    }

    return jeu2;
}

bool est_resolu (jeu jeu_) {
    piece* piece_a_sortir = jeu_.pieces[jeu_.piece_a_sortir];
    return piece_a_sortir != NULL && piece_a_sortir->pos.i == jeu_.sortie.i && piece_a_sortir->pos.j == jeu_.sortie.j;
}

unsigned long long hash_jeu(jeu jeu_) {
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

void melanger_pieces_jeu (jeu* jeu_) {
    int n = jeu_->nb_pieces;

    for (int i = 0; i < n; i += 1) {
        int j = rand() % n;

        piece* tmp = jeu_->pieces[i];

        jeu_->pieces[i] = jeu_->pieces[j];
        jeu_->pieces[j] = tmp;

        // on met à jour l'indice de la pièce à sortir
        if (i == jeu_->piece_a_sortir) {
            jeu_->piece_a_sortir = j;
        } else if (j == jeu_->piece_a_sortir) {
            jeu_->piece_a_sortir = i;
        }

        // on met à jour les indices correspondant aux id des pièces
        jeu_->id_pieces[jeu_->pieces[i]->id - 1] = i;
        jeu_->id_pieces[jeu_->pieces[j]->id - 1] = j;
    }

}