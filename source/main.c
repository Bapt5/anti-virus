#include "librairies.h"
#include "jeu.h"
#include "liste_chaine.h"

#include "resolution.h"
#include "creation.h"

#include <time.h>

liste get_solution(liste(*f)(jeu, int*), jeu* j) {
    int nb_explo = 0;

    clock_t debut = clock();
    liste resultat = f(*j, &nb_explo);
    clock_t fin = clock();

    double temps_execution = (double)(fin - debut) / CLOCKS_PER_SEC;

    printf("Nombre d'explorations %d\n", nb_explo);

    if (est_vide_liste(resultat)) {
        printf("Temps d'exécution %f s\n", temps_execution);
        printf("Pas de solution\n");
    } else {
        printf("Solution trouvée en %f s\n", temps_execution);
        printf("Solution en %d coups\n", longueur_liste(resultat) - 1);

    }

    return resultat;
}

void test_fonction(char* nom_fonction, char* nom_jeu, liste(*f)(jeu, int*), jeu* j, bool affiche_solution, int pause, void* publisher) {
    printf("Test %s sur %s: \n", nom_fonction, nom_jeu);

    if (affiche_solution) {
        printf("Affichage de l'instance de jeu\n");
        affiche_jeu_python(*j, publisher);
        sleep(pause);
    }

    liste resultat = get_solution(f, j);

    // AFFICHAGE DE LA SOLUTION
    if (affiche_solution && !est_vide_liste(resultat)) {
        while (true) {
            printf("Afficher la solution ? (Y/n) ");
            char c;
            scanf(" %c", &c);

            if (c == 'Y') {
                affiche_sol_python(resultat, pause, publisher);
            }else {
                break;
            }
        }
    }

    free_liste(resultat, free_jeu);
    
    printf("\n");
}

void tests_instances () {
    for (int i = 1; i <= 60; i++) {
        char* nom_fichier = malloc(100 * sizeof(char));
        sprintf(nom_fichier, "instances/livret/defi%d", i);

        jeu* jeu_ = creer_jeu(nom_fichier);

        int nb_explo_v4 = 0;
        int nb_explo_v5 = 0;
        int nb_explo_v6 = 0;

        printf("Test v4 sur %s: ", nom_fichier);
        liste resultat_v4 = v4(*jeu_, &nb_explo_v4);
        if (est_vide_liste(resultat_v4)) {
            printf("Pas de solution\n");
        } else {
            printf("Solution en %d coups\n", longueur_liste(resultat_v4) - 1);
        }
        free_liste(resultat_v4, free_jeu);

        printf("Test v5 sur %s: ", nom_fichier);
        liste resultat_v5 = v5(*jeu_, &nb_explo_v5);
        if (est_vide_liste(resultat_v5)) {
            printf("Pas de solution\n");
        } else {
            printf("Solution en %d coups\n", longueur_liste(resultat_v5) - 1);
        }
        free_liste(resultat_v5, free_jeu);

        printf("Test v6 sur %s: ", nom_fichier);
        liste resultat_v6 = v6(*jeu_, &nb_explo_v6);
        if (est_vide_liste(resultat_v6)) {
            printf("Pas de solution\n");
        } else {
            printf("Solution en %d coups\n", longueur_liste(resultat_v6) - 1);
        }
        free_liste(resultat_v6, free_jeu);

        free(jeu_);
        free(nom_fichier);
        printf("\n");

        FILE* f = fopen("resultats.csv", "a");
        fprintf(f, "defi%d,%d,%d,%d\n", i, nb_explo_v4, nb_explo_v5, nb_explo_v6);
        fclose(f);
    }
}

int main()
{
    srand(time(NULL));
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    
    if (zmq_bind(publisher, "tcp://*:5555") != 0) {
        perror("Erreur lors de la liaison du socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        return 1;
    }
    sleep(1);

    // while (true)
    // {
        jeu* jeu_ = creation_jeu_random(13, 10);

        jeu* mjeu = melanger_jeu(jeu_, 1000);

        affiche_jeu_python(*jeu_, publisher);
        sleep(1);
        affiche_jeu_python(*mjeu, publisher);
        sleep(1);

        test_fonction("v4", "jeu", v4, mjeu, true, 1, publisher);

        free_jeu(jeu_);
        free_jeu(mjeu);
    // }

}