#include "librairies.h"
#include "jeu.h"
#include "liste_chaine.h"

#include "resolution.h"

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

int main()
{
    bool affiche_solution = false;
    int pause = 1;

    /* STARTER 1 */
    jeu* starter1 = creer_jeu("instances/starter1");

    /* JUNIOR 17 */
    jeu* junior17 = creer_jeu("instances/junior17");

    /* WIZARD 60 */
    jeu* wizard60 = creer_jeu("instances/wizard60");

    /* EXPERT 33 */
    jeu* expert33 = creer_jeu("instances/expert33");
    // pas de sol : sortie piece verte 

    // JUNIOR 14 pièce jaune sortie dès le départ

    /* WIZARD 52 */
    jeu* wizard52 = creer_jeu("instances/wizard52");

    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    
    if (zmq_bind(publisher, "tcp://*:5555") != 0) {
        perror("Erreur lors de la liaison du socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        return 1;
    }

    // test_fonction("v1", "starter 1", v1, starter1, affiche_solution, pause, publisher);
    // test_fonction("v2", "starter 1", v2, starter1, affiche_solution, pause, publisher);
    // test_fonction("v3", "starter 1", v3, starter1, affiche_solution, pause, publisher);
    // test_fonction("v4", "starter 1", v4, starter1, affiche_solution, pause, publisher);
    // printf("\n");

    // test_fonction("v2", "junior 17", v2, junior17, affiche_solution, pause, publisher);
    // test_fonction("v3", "junior 17", v3, junior17, affiche_solution, pause, publisher);
    // test_fonction("v4", "junior 17", v4, junior17, affiche_solution, pause, publisher);
    // printf("\n");

    // test_fonction("v2", "wizard 60", v2, wizard60, affiche_solution, pause, publisher);
    // test_fonction("v3", "wizard 60", v3, wizard60, affiche_solution, pause, publisher);
    // test_fonction("v4", "wizard 60", v4, wizard60, affiche_solution, pause, publisher);
    // printf("\n");

    // test_fonction("v2", "expert 33", v2, expert33, affiche_solution, pause, publisher);
    // test_fonction("v3", "expert 33", v3, expert33, affiche_solution, pause, publisher);
    // test_fonction("v4", "expert 33", v4, expert33, affiche_solution, pause, publisher);
    // printf("\n");

    // test_fonction("v2", "wizard 52", v2, wizard52, affiche_solution, pause, publisher);
    // test_fonction("v3", "wizard 52", v3, wizard52, affiche_solution, pause, publisher);
    // test_fonction("v4", "wizard 52", v4, wizard52, affiche_solution, pause, publisher);



    // TEST ALEATOIRE SUR PLUSIEURS RESOLUTIONS
    // test_fonction("v3", "wizard 60", v3, wizard60, publisher);

    // double temps_total = 0;

    // printf("Test v4 sur wizard 60: \n");

    // const int nb_essais = 25;

    // for (int i = 0; i < nb_essais; i++) {
    //     clock_t debut = clock();
    //     liste resultat = v4(*wizard60);
    //     clock_t fin = clock();

    //     printf("Solution en %d coups\n", longueur_liste(resultat) - 1);

    //     double temps_execution = (double)(fin - debut) / CLOCKS_PER_SEC;
    //     printf("Temps d'exécution %f s\n\n", temps_execution);

    //     temps_total = temps_total + temps_execution;

    //     free_liste(resultat, free_jeu);
    // }

    // printf("Temps d'exécution moyen %f s\n", temps_total / nb_essais);

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    free_jeu(starter1);
    free_jeu(junior17);
    free_jeu(wizard60);
    free_jeu(expert33);
    free_jeu(wizard52);

    return 0;
}