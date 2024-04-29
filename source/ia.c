#include "ia.h"


kd_trees* create_kd_trees(donnee d, int profondeur) {

    kd_trees* arbre = malloc(sizeof(kd_trees));
    arbre->donnee = d;
    arbre->profondeur = profondeur;
    arbre->gauche = NULL;
    arbre->droit = NULL;

    return arbre;
}


void tri_rapide_donnee(donnee* donnees, int nb_donnees, int coordonnee, int debut, int fin) {
    /*
        * Tri rapide d'un tableau de données sur une coordonnée donnée
        *
        * @param donnees: tableau de données à trier
        * @param nb_donnees: nombre de données dans le tableau
        * @param coordonnee: coordonnée sur laquelle on trie
        * @param debut: indice de début du tableau (inclus)
        * @param fin: indice de fin du tableau (inclus)
    */

    if (debut < fin) {
        int pivot = donnees[fin].coordonnee.c[coordonnee];
        int i = debut - 1;

        for (int j = debut; j < fin; j++) {
            if (donnees[j].coordonnee.c[coordonnee] < pivot) {
                i++;
                donnee temp = donnees[i];
                donnees[i] = donnees[j];
                donnees[j] = temp;
            }
        }

        donnee temp = donnees[i + 1];
        donnees[i + 1] = donnees[fin];
        donnees[fin] = temp;

        tri_rapide_donnee(donnees, nb_donnees, coordonnee, debut, i);
        tri_rapide_donnee(donnees, nb_donnees, coordonnee, i + 2, fin);
    }
    
}

donnee* calcul_donnees (jeu* jeux, int nb_jeux, fct_coordonnee fct_coor, fct_resolution resolutions[], int nb_resolutions) {
    /*
        * Calcul des données pour l'entrainement de l'algorithme des k plus proches voisins
        *
        * @param jeux: tableau de jeux
        * @param nb_jeux: nombre de jeux dans le tableau
        * @param fct_coor: fonction qui retourne les coordonnées d'un jeu
        * @param resolutions: tableau de fonctions de résolution
        * @param nb_resolutions: nombre de fonctions de résolution
        *
        * @return: tableau de données
    */

    donnee* donnees = malloc(nb_jeux * sizeof(donnee));
    for (int i = 0; i < nb_jeux; i++) {
        donnees[i].coordonnee = fct_coor(jeux[i]);

        printf("Calcul des données pour le jeu %d\n", i);
        
        int* nb_explos = malloc(nb_resolutions * sizeof(int));
        for (int j = 0; j < nb_resolutions; j++) {
            nb_explos[j] = 0;

            liste* resultat = resolutions[j](jeux[i], &nb_explos[j]);
            free_liste(resultat, free_jeu);
        }

        // on choisit la résolution qui a exploré le moins de noeuds
        donnees[i].heuristique = 0;
        for (int j = 1; j < nb_resolutions; j++) {
            if (nb_explos[j] < nb_explos[donnees[i].heuristique]) {
                donnees[i].heuristique = j;
            }
        }

        free(nb_explos);
    }

    return donnees;
}

kd_trees* remplissage_kd_trees (donnee* donnees, int nb_donnees, int profondeur) {
    /*
        * Remplissage d'un arbre kd à partir d'un tableau de données
        *
        * @param donnees: tableau de données
        * @param nb_donnees: nombre de données dans le tableau
        * @param profondeur: profondeur de l'arbre
        *
        * @return: l'arbre kd rempli
    */

    if (nb_donnees == 0) {
        return NULL;
    }

    int dimension = donnees[0].coordonnee.dimension;
    int coordonnee = profondeur % dimension;

    tri_rapide_donnee(donnees, nb_donnees, coordonnee, 0, nb_donnees - 1);

    int mediane = nb_donnees / 2;

    kd_trees* arbre = create_kd_trees(donnees[mediane], profondeur);

    arbre->gauche = remplissage_kd_trees(donnees, mediane, profondeur + 1);
    arbre->droit = remplissage_kd_trees(donnees + mediane + 1, nb_donnees - mediane - 1, profondeur + 1);

    return arbre;
}

kd_trees_context* train_knn(jeu* jeux, int nb_jeux, fct_coordonnee fct_coor, fct_resolution resolutions[], int nb_resolutions) {    
    donnee* donnees = calcul_donnees(jeux, nb_jeux, fct_coor, resolutions, nb_resolutions);
    kd_trees* arbre = remplissage_kd_trees(donnees, nb_jeux, 0);

    free(donnees); 

    kd_trees_context* context = malloc(sizeof(kd_trees_context));

    context->arbre = arbre;
    context->dimension = arbre->donnee.coordonnee.dimension;
    context->fct_coor = fct_coor;
    context->nb_resolutions = nb_resolutions;
    context->resolutions = malloc(nb_resolutions * sizeof(fct_resolution));

    for (int i = 0; i < nb_resolutions; i++) {
        context->resolutions[i] = resolutions[i];
    }

    return context;
}
    
void free_kd_trees(kd_trees* arbre) {
    if (arbre == NULL) {
        return;
    }

    free_kd_trees(arbre->gauche);
    free_kd_trees(arbre->droit);

    free_coordonnee(arbre->donnee.coordonnee);
    free(arbre);
}

void free_kd_trees_context(kd_trees_context* context) {
    free_kd_trees(context->arbre);
    free(context->resolutions);
    free(context);
}

void export_kd_trees(kd_trees* arbre, FILE* fichier) {
    if (arbre == NULL) {
        fprintf(fichier, "N\n");
        return;
    }

    fprintf(fichier, "%d %d ", arbre->profondeur, arbre->donnee.heuristique);
    for (int i = 0; i < arbre->donnee.coordonnee.dimension; i++) {
        fprintf(fichier, "%d ", arbre->donnee.coordonnee.c[i]);
    }
    fprintf(fichier, "\n");

    export_kd_trees(arbre->gauche, fichier);
    export_kd_trees(arbre->droit, fichier);
}

void export_kd_trees_context(kd_trees_context* context, char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "w");

    fprintf(fichier, "%d %d ", context->dimension, context->nb_resolutions);

    // on demande à l'utilisateur de rentrer le nom de la fonction de coordonnée
    char* nom_fonction = malloc(100 * sizeof(char));
    printf("Entrez le nom de la fonction de coordonnée: ");
    scanf("%s", nom_fonction);
    fprintf(fichier, "%s ", nom_fonction);
    free(nom_fonction);

    // on demande à l'utilisateur de rentrer le nom des fonctions de résolution
    for (int i = 0; i < context->nb_resolutions; i++) {
        char* nom_fonction = malloc(100 * sizeof(char));
        printf("Entrez le nom de la fonction de résolution %d: ", i);
        scanf("%s", nom_fonction);
        fprintf(fichier, "%s ", nom_fonction);
        free(nom_fonction);
    }

    fprintf(fichier, "\n");

    export_kd_trees(context->arbre, fichier);

    fclose(fichier);
}

kd_trees* import_kd_trees(FILE* fichier, int dimension, int profondeur) {
    /*
        * Importe un arbre kd depuis un fichier
        *
        * @param fichier: fichier depuis lequel on importe l'arbre
        * @param dimension: dimension des coordonnées des données
        * @param profondeur: profondeur de l'arbre
        *
        * @return: l'arbre kd importé
    */

    int profondeur_arbre;
    donnee d;

    if (fgetc(fichier) == 'N') {
        assert(fgetc(fichier) == '\n');
        return NULL;
    } else {
        fseek(fichier, -1, SEEK_CUR);
    }

    int code = fscanf(fichier, "%d %d ", &profondeur_arbre, &d.heuristique);

    if (code == EOF) {  // fin de fichier
        return NULL;
    }

    assert(profondeur_arbre == profondeur);

    d.coordonnee.dimension = dimension;
    d.coordonnee.c = malloc(dimension * sizeof(int));
    for (int i = 0; i < d.coordonnee.dimension; i++) {
        fscanf(fichier, "%d ", &d.coordonnee.c[i]);
    }

    kd_trees* arbre = create_kd_trees(d, profondeur);

    arbre->gauche = import_kd_trees(fichier, dimension, profondeur + 1);
    arbre->droit = import_kd_trees(fichier, dimension, profondeur + 1);

    return arbre;
}
    

kd_trees_context* import_kd_trees_context(char* nom_fichier, fct_coordonnee fct_coor, fct_resolution resolutions[], int nb_resolutions) {
    FILE* fichier = fopen(nom_fichier, "r");

    kd_trees_context* context = malloc(sizeof(kd_trees_context));

    fscanf(fichier, "%d %d ", &context->dimension, &context->nb_resolutions);

    char* nom_fonction = malloc(100 * sizeof(char));
    fscanf(fichier, "%s ", nom_fonction);
    printf("Fonction de coordonnée: %s\n", nom_fonction);
    free(nom_fonction);

    context->fct_coor = fct_coor;

    context->resolutions = malloc(context->nb_resolutions * sizeof(fct_resolution));
    for (int i = 0; i < context->nb_resolutions; i++) {
        char* nom_fonction = malloc(100 * sizeof(char));
        fscanf(fichier, "%s ", nom_fonction);
        printf("Fonction de résolution %d: %s\n", i, nom_fonction);
        free(nom_fonction);

        context->resolutions[i] = resolutions[i];
    }

    context->arbre = import_kd_trees(fichier, context->dimension, 0);
    fclose(fichier);

    return context;
}



void recherche_kd_trees(kd_trees* arbre, coordonnee_t coordonnee_jeu, tas* k_plus_proches, int k) {
    /*
        * Recherche des k plus proches voisins d'un jeu dans un arbre kd
        *
        * @param arbre: arbre kd dans lequel on recherche
        * @param j: jeu pour lequel on recherche les k plus proches voisins
        * @param k: nombre de plus proches voisins à trouver
        * @param k_plus_proches: tas contenant les k plus proches voisins
    */
    
    if (arbre == NULL) {
        return;
    }

    int distance = distance_euclidienne(arbre->donnee.coordonnee, coordonnee_jeu);

    if (k_plus_proches->taille < k) {
        inserer_tas(arbre->donnee.heuristique, distance, k_plus_proches);
    }else if (distance < k_plus_proches->tab[0].priorite) {
        extraire_tas(k_plus_proches);
        inserer_tas(arbre->donnee.heuristique, distance, k_plus_proches);
    }

    int coordonnee = arbre->profondeur % arbre->donnee.coordonnee.dimension;

    if (coordonnee_jeu.c[coordonnee] < arbre->donnee.coordonnee.c[coordonnee]) {
        recherche_kd_trees(arbre->gauche, coordonnee_jeu, k_plus_proches, k);
        if (k_plus_proches->taille < k || abs(coordonnee_jeu.c[coordonnee] - arbre->donnee.coordonnee.c[coordonnee]) < k_plus_proches->tab[0].priorite) {
            recherche_kd_trees(arbre->droit, coordonnee_jeu, k_plus_proches, k);
        }
    }
    else {
        recherche_kd_trees(arbre->droit, coordonnee_jeu, k_plus_proches, k);
        if (k_plus_proches->taille < k || abs(coordonnee_jeu.c[coordonnee] - arbre->donnee.coordonnee.c[coordonnee]) < k_plus_proches->tab[0].priorite) {
            recherche_kd_trees(arbre->gauche, coordonnee_jeu, k_plus_proches, k);
        }
    }
}

fct_resolution knn(kd_trees_context* context, jeu j, int k) {
    tas k_plus_proches = creer_tas(k, false);

    coordonnee_t coordonnee_jeu = context->fct_coor(j);

    recherche_kd_trees(context->arbre, coordonnee_jeu, &k_plus_proches, k);

    int* occurences = malloc(context->nb_resolutions * sizeof(int));
    for (int i = 0; i < context->nb_resolutions; i++) {
        occurences[i] = 0;
    }

    for (int i = 0; i < k; i++) {
        int heuristique = k_plus_proches.tab[i].valeur;
        occurences[heuristique]++;
    }

    int max_occurences = 0;
    int idc_heuristique = 0;
    for (int i = 0; i < context->nb_resolutions; i++) {
        if (occurences[i] > max_occurences) {
            max_occurences = occurences[i];
            idc_heuristique = i;
        }
    }

    free_tas(&k_plus_proches, NULL);
    free_coordonnee(coordonnee_jeu);
    free(occurences);

    return context->resolutions[idc_heuristique];
}

void matrice_confusion (jeu* jeux, int nb_jeux, kd_trees_context* context, int k) {
    // initialisation de la matrice de confusion
    int** matrice = malloc(context->nb_resolutions * sizeof(int*));
    for (int i = 0; i < context->nb_resolutions; i++) {
        matrice[i] = malloc(context->nb_resolutions * sizeof(int));
        for (int j = 0; j < context->nb_resolutions; j++) {
            matrice[i][j] = 0;
        }
    }

    donnee* donnees = calcul_donnees(jeux, nb_jeux, context->fct_coor, context->resolutions, context->nb_resolutions);
    
    for (int i = 0; i < nb_jeux; i++) {
        fct_resolution guess_heuristique = knn(context, jeux[i], k);
        
        int idc_guess = -1;
        for (int j = 0; j < context->nb_resolutions; j++) {
            if (context->resolutions[j] == guess_heuristique) {
                idc_guess = j;
                break;
            }
        }

        matrice[donnees[i].heuristique][idc_guess]++;
    }

    // libération des données
    for (int i = 0; i < nb_jeux; i++) {
        free_coordonnee(donnees[i].coordonnee);
    }
    free(donnees);

    // affichage de la matrice de confusion
    printf("Matrice de confusion:\n");
    for (int i = 0; i < context->nb_resolutions; i++) {
        for (int j = 0; j < context->nb_resolutions; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    // libération de la matrice de confusion
    for (int i = 0; i < context->nb_resolutions; i++) {
        free(matrice[i]);
    }
    free(matrice);
}