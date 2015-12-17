//
//  main.c
//  DS 20 janvier 2015
//
//  Created by CAILLOT Antoine on 17/12/2015.
//  Copyright © 2015 CAILLOT Antoine. All rights reserved.
//

#include <stdio.h>
#include <string.h>

struct Date{
    int jour;
    int mois;
    int annee;
};

struct Produit {
    char designation[25];
    float prix;
    int pourcentage;
    struct Date date_limite;
};

void ajouter_produit();
void modif_pourcent(char destination[25], int pourcentage);
void moyenne_pourcentage();
void lire_fichier();
void afficher(struct Produit prod);
void supr(struct Date d);

int main(int argc, const char * argv[]) {
    int boucle = 1, choix = 0;
    char des[25];
    int pourcentage = 0;
    struct Date d;
    while (boucle) {
        printf("========== MENU ==========\n"
               "1. ajouter\n"
               "2. afficher\n"
               "3. moyenne\n"
               "4. modifier pourcentage\n"
               "5. suprimmer\n"
               "0. Quitter\n");
        scanf("%d", &choix);
        getchar(); // il y a un '\n' (truc qui fait tout crasher) dans stdin: on le retire
        switch (choix) {
            case 0:
                boucle = 0; //fait sortir de la boucle
                break;
                
            case 1:
                ajouter_produit();
                break;
                
            case 2:
                lire_fichier();
                break;
                
            case 3:
                moyenne_pourcentage();
                break;
                
            case 4:
                printf("Designation : ");
                fgets(des, 25, stdin);
                printf("Pourcentage : ");
                scanf("%d", &pourcentage);
                getchar();
                modif_pourcent(des, pourcentage);
                break;
                
            case 5:
                printf("Date limite de consomation ( jj mm aaaa) : ");
                scanf("%2d %2d %4d", &d.jour, &d.mois, &d.annee);
                supr(d);
                break;
                
            default:
                break;
        }
    }
    return 0;
}

void ajouter_produit() {
    struct Produit prod;
    FILE* fic = NULL;
    printf("Désignation : ");
    fgets(prod.designation, 25, stdin);
    printf("Prix : ");
    scanf("%f", &prod.prix);
    getchar();
    printf("Pourcentage : ");
    scanf("%d", &prod.pourcentage);
    getchar();
    printf("Date limite de consomation ( jj mm aaaa) : ");
    scanf("%2d %2d %4d", &prod.date_limite.jour, &prod.date_limite.mois, &prod.date_limite.annee); //cela est dangereux, ne pas utiliser, faire 3 scanf est mieux
    fic = fopen("produits.dat", "a"); //mode ajout
    if (fic != NULL) {
        fwrite(&prod, sizeof(prod), 1, fic);
        fclose(fic); //ne pas oublier
    }
    else{
        printf("erreur fichier\n");
    }
}

void afficher(struct Produit prod){
    printf("Produit : %sPrix %.2f\nSolde %d%%\nDate limite : %2d/%2d/%4d\n", prod.designation, prod.prix, prod.pourcentage, prod.date_limite.jour, prod.date_limite.mois, prod.date_limite.annee);
}

void lire_fichier(){
    struct Produit prod;
    FILE* fic = NULL;
    fic = fopen("produits.dat", "r");
    if (fic != NULL) {
        while (fread(&prod, sizeof(prod), 1, fic) == 1 && !feof(fic)){
            afficher(prod);
        }
        fclose(fic);
    }
    else{
        printf("erreur fichier\n");
    }
}

void moyenne_pourcentage(){
    struct Produit prod;
    FILE* fic = NULL;
    int n = 0, somme = 0;
    fic = fopen("produits.dat", "r");
    if (fic != NULL) {
        n = 0;
        while (fread(&prod, sizeof(prod), 1, fic) == 1 && !feof(fic)){
            somme += prod.pourcentage;
            n++;
        }
        fclose(fic);
        printf("Moyenne des pourcentages : %.3f\n", (float) somme / (float) n);
    }
    else{
        printf("erreur fichier\n");
    }
}


void modif_pourcent(char destination[25], int pourcentage){
    struct Produit liste[200], prod;
    FILE* fic = NULL;
    int n=0, i = 0, flag = 0;
    fic = fopen("produits.dat", "r");
    if (fic != NULL) {
        n = 0;
        while (fread(&prod, sizeof(prod), 1, fic) == 1 && !feof(fic)){
            liste[n] = prod;
            n++;
        }
        fclose(fic);
    }
    else{
        printf("erreur fichier\n");
    }
    for (i = 0; i<n; i++) {
        if (strcmp(liste[i].designation, destination) == 0) { // il y a 0 difference ?
            liste[i].pourcentage = pourcentage;
            flag++;
        }
    }
    if(flag == 0){
        printf("Il n'y a pas de %s", destination);
    }
    fic = fopen("produits.dat", "w"); //on écrase le fichier avec un nouveau pour mettre celui modifié
    if (fic != NULL) {
        for (i = 0; i<n; i++) {
            fwrite(&liste[i], sizeof(prod), 1, fic);
        }
        fclose(fic);
    }
    else{
        printf("erreur fichier\n");
    }
}

void supr(struct Date d){
    struct Produit prod, liste[200];
    FILE* fic = NULL;
    int n=0, i = 0;
    fic = fopen("produits.dat", "r");
    if (fic != NULL) {
        n = 0;
        while (fread(&prod, sizeof(prod), 1, fic) == 1 && !feof(fic)){
            if ((prod.date_limite.annee < d.annee) || (prod.date_limite.annee == d.annee && prod.date_limite.mois < d.mois) || (prod.date_limite.annee == d.annee && prod.date_limite.mois == d.mois && prod.date_limite.jour < d.jour)) {
                afficher(prod);
            }
            else{ // on enregistre ce qui on une date >= a la date entrée
                liste[n] = prod;
                n++;
            }
        }
        fclose(fic);
    }
    else{
        printf("erreur fichier\n");
    }
    fic = fopen("produits.dat", "w");
    if (fic != NULL) {
        for (i = 0; i<n; i++) {
            fwrite(&liste[i], sizeof(prod), 1, fic);
        }
        fclose(fic);
    }
    else{
        printf("erreur fichier\n");
    }
}


