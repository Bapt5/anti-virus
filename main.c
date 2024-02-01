#include "librairies.h"
#include "jeu.h"
#include "liste_chaine.h"

#include "resolution.h"

#include <time.h>

liste get_solution(liste(*f)(jeu), jeu* j) {
    clock_t debut = clock();
    liste resultat = f(*j);
    clock_t fin = clock();

    double temps_execution = (double)(fin - debut) / CLOCKS_PER_SEC;

    if (est_vide_liste(resultat)) {
        printf("Temps d'exécution %f s\n", temps_execution);
        printf("Pas de solution\n");
    } else {
        printf("Solution trouvée en %f s\n", temps_execution);
        printf("Solution en %d coups\n", longueur_liste(resultat) - 1);

    }

    return resultat;
}

void test_fonction(char* nom_fonction, char* nom_jeu, liste(*f)(jeu), jeu* j, void* publisher) {
    printf("Test %s sur %s: \n", nom_fonction, nom_jeu);
    sleep(1);
    affiche_jeu_python(*j, publisher);

    liste resultat = get_solution(f, j);
    free_liste(resultat, free_jeu);
    
    printf("\n");
}

int main()
{
    /* STARTER 1 */
    jeu* jeu1 = creer_jeu("instances/starter1");

    /* JUNIOR 17 */
    jeu* jeu2 = creer_jeu("instances/junior17");

    /* WIZARD 60 */
    jeu* jeu3 = creer_jeu("instances/wizard60");

    /* EXPERT 33 */
    jeu* jeu4 = creer_jeu("instances/expert33");
    // pas de sol : sortie piece verte 

    // JUNIOR 14 pièce jaune sortie dès le départ

    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    
    if (zmq_bind(publisher, "tcp://*:5555") != 0) {
        perror("Erreur lors de la liaison du socket");
        zmq_close(publisher);
        zmq_ctx_destroy(context);
        return 1;
    }

    // test_fonction("v1", "starter 1", v1, jeu1, publisher);
    // test_fonction("v2", "starter 1", v2, jeu1, publisher);
    // test_fonction("v3", "starter 1", v3, jeu1, publisher);
    // test_fonction("v4", "starter 1", v4, jeu1, publisher);
    // printf("\n");

    // test_fonction("v2", "junior 17", v2, jeu2, publisher);
    // test_fonction("v3", "junior 17", v3, jeu2, publisher);
    // test_fonction("v4", "junior 17", v4, jeu2, publisher);
    // printf("\n");

    // test_fonction("v2", "wizard 60", v2, jeu3, publisher);
    // test_fonction("v3", "wizard 60", v3, jeu3, publisher);
    // test_fonction("v4", "wizard 60", v4, jeu3, publisher);
    // printf("\n");

    // test_fonction("v2", "expert 33", v2, jeu4, publisher);
    // test_fonction("v3", "expert 33", v3, jeu4, publisher);
    // test_fonction("v4", "expert 33", v4, jeu4, publisher);

    test_fonction("v3", "wizard 60", v3, jeu3, publisher);

    double temps_total = 0;

    printf("Test v4 sur wizard 60: \n");

    const int nb_essais = 25;

    for (int i = 0; i < nb_essais; i++) {
        clock_t debut = clock();
        liste resultat = v4(*jeu3);
        clock_t fin = clock();

        printf("Solution en %d coups\n", longueur_liste(resultat) - 1);

        double temps_execution = (double)(fin - debut) / CLOCKS_PER_SEC;
        printf("Temps d'exécution %f s\n\n", temps_execution);

        temps_total = temps_total + temps_execution;

        free_liste(resultat, free_jeu);
    }

    printf("Temps d'exécution moyen %f s\n", temps_total / nb_essais);


    // liste resultat = v4(*jeu3);
    // printf("Solution en %d coups\n", longueur_liste(resultat) - 1);
    // while (true) {       
    //     printf("Afficher la solution ? (Y/n) ");
    //     char c;
    //     scanf(" %c", &c);

    //     if (c == 'Y') {
    //         affiche_sol_python(resultat, publisher);
    //     }else {
    //         break;
    //     }
    // }
    // free_liste(resultat, free_jeu);

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    free_jeu(jeu1);
    free_jeu(jeu2);
    free_jeu(jeu3);
    free_jeu(jeu4);

    return 0;
}