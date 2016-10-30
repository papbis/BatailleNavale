#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAP_H 18
#define MAP_W 24 // maxi 26
#define NB_PLAYER 2
#define DATA 2
#define TYPE 0
#define INFO 1
#define PLAYER1_ID 0
#define PLAYER2_ID 1
#define IA 2 //Mettre a 2 pour une IA plus difficile

#define B1_NOM "corvette"
#define B1_ID 1
#define B1_TAILLE 1
#define B1_NB 1
#define B2_NOM "destroyer"
#define B2_ID 2
#define B2_TAILLE 3
#define B2_NB 2
#define B3_NOM "croiseur"
#define B3_ID 3
#define B3_TAILLE 4
#define B3_NB 2
#define B4_NOM "porte-avion"
#define B4_ID 4
#define B4_TAILLE 6
#define B4_NB 1

#define SAVE_PATH ".\\"
#define AUTO_SAVE 1
#define printIAtable 1

int static map[MAP_W][MAP_H][NB_PLAYER][DATA];
int static GameMode; // 0=PVP, 1=PVE, 2=EVE


void br(int n){
    int i;
    for(i=0;i<n;i++){
        printf("\n");
    }
}// fonction de retour a la ligne

int autreJoueur(int joueur){//fonction qui selectionne l'autre joueur
    if(joueur==PLAYER1_ID){
        return PLAYER2_ID;
    }else{
        return PLAYER1_ID;
    }
}

void color(int t,int f){// definie la couleur du texte et du bg dans la console
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
} // Fin color

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
} // Fin waitFor

int doRand(int startVal, int endVal){// fonction doRand modifiée
    waitFor(0.05);
    srand(time(NULL)*rand());
    if(startVal == 0){
        return (rand() % (endVal +1));
    }else{
        return (rand() % ((endVal + startVal -1)) + startVal);
    }
} // Fin doRand

char nbToLettre(int nb){
    return nb=(nb%26+65);
} // Fin nbToLettre

void printTableau(int player,int tableau){
    int x,y;
    for(y=-1;y<MAP_H;y++){
        color(7,0);
        if(y<9){ // ajout d'espace pour alligner
            printf(" ");
        }

        if(y>=0){ // affiche les coordonés Y
            printf("%d ",y+1);
        }else{// affiche X en haut a gauche
            printf("X ");
        }

        for(x=0;x<MAP_W;x++){
            color(7,0);
            if(y>=0){//si dans la grille (!= legende)
                if(map[x][y][tableau][0]==0){
                    color(1,1);
                    printf(" %d ",map[x][y][tableau][0]);
                }else if(map[x][y][tableau][0]==1){
                    if(player==tableau){
                        color(0,7);
                        printf(" %d ",map[x][y][tableau][INFO]);
                    }else{
                        color(1,1);
                        printf(" %d ",0);
                    }
                }else if(map[x][y][tableau][0]==-2){
                    color(7,1);
                    printf("%d ",map[x][y][tableau][0]);
                }else if(map[x][y][tableau][0]==-1){
                    color(7,4);
                    if(player==tableau){
                        printf(" %d ",map[x][y][tableau][INFO]);
                    }else{
                        printf("%d ",map[x][y][tableau][0]);
                    }
                }else{
                    color(0,7);
                    printf("%d ",map[x][y][tableau][0]);
                }
                color(7,0);
            }else{// si légende
                printf(" %c ",nbToLettre(x));
            }
        }

        if(y>=0){
            br(1);
        }else{
            br(1);
            br(1);
        }
    }
    color(7,0);
} // Fin printTableau

void getUserPos(int *x,int *y){
    int valide = 0;
    char pos[3] = {0};
    while(valide==0){
        scanf("%3[a-zA-Z0-9]",&pos);
        fflush(stdin);

        if((int)pos[0]<=90&&(int)pos[0]>=65){ // X (en lettre)
            *x=(int)pos[0]-65;
            valide = 1;
        }else if((int)pos[0]<=122&&(int)pos[0]>=97){
            *x=(int)pos[0]-97;
            valide = 1;
        }
        if((int)pos[1]<=57&&(int)pos[1]>=48&&valide==1){ // Y (en chiffre)
            *y=(int)pos[1]-49;
            valide = 1;
        }else{
            valide = 0;
        }
        if((int)pos[2]<=57&&(int)pos[2]>=48&&valide==1){ // si Y > 9
            *y=10*(*y+1)+((int)pos[2]-49);
            valide = 1;
        }
        if(valide == 1 && ( !( *x < MAP_W && *x >= 0 ) || !( *y < MAP_H && *y >= 0 ) ) ){ // si hors grille
            valide=0;
        }
    }
} // Fin getUserPos

void initPos(int *x,int *y,char nom[10],int taille,int player,int id){
    int O,i;
    int valide =0;
    while(valide!=1){

    if(player+GameMode<=1){//si joueur != IA
        printf("entrez la position du %s (%d cases)",nom,taille);
        getUserPos(x,y);
    }else{
        *x=doRand(0,MAP_W-1);
        *y=doRand(0,MAP_H-1);
    }


    valide = 0;
    if(player+GameMode<=1){//si joueur != IA
        printf("entrez l'orientation ( 1-Haut / 2-Bas / 3-Gauche / 4-Droite )");
        scanf("%d",&O);
        fflush(stdin);
    }else{
        O=doRand(1,4);
    }

        switch(O){// Verifie puis positionne le bateau selon orientation choisie :
            case 1://Haut
                if(*y+1>=taille){//Verification
                        valide = 1;
                    for(i=*y+1-taille;i<=*y;i++){
                        if(valide==1 && map[*x][i][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }// Fin verication
                if(valide == 1){// si valide : positionner le bateau
                    for(i=*y+1-taille;i<=*y;i++){
                        map[*x][i][player][TYPE]=1;
                        map[*x][i][player][INFO]=id;
                    }
                }//fin positionnement
            break;

            case 2://Bas
                if(*y<=MAP_H-taille){
                    valide = 1;
                    for(i=*y-1+taille;i>=*y;i--){
                        if(valide==1 && map[*x][i][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(i=*y-1+taille;i>=*y;i--){
                        map[*x][i][player][TYPE]=1;
                        map[*x][i][player][INFO]=id;
                    }
                }
            break;
            case 3://Gauche
                if(*x+1>=taille){
                        valide = 1;
                    for(i=*x+1-taille;i<=*x;i++){
                        if(valide==1 && map[i][*y][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(i=*x+1-taille;i<=*x;i++){
                        map[i][*y][player][TYPE]=1;
                        map[i][*y][player][INFO]=id;
                    }
                }
            break;
            case 4://Droite
                if(*x<=MAP_H-taille){
                    valide = 1;
                    for(i=*x-1+taille;i>=*x;i--){
                        if(valide==1 && map[i][*y][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(i=*x-1+taille;i>=*x;i--){
                        map[i][*y][player][TYPE]=1;
                        map[i][*y][player][INFO]=id;
                    }
                }
            break;
        }

    }
} // Fin initPos

void initMap(int player){
    int i,j,k,l,X,Y,O;

    printf("Tour du joueur %d\n",player+1);
    getchar();

    //Initialise le tableau a 0
    for(i=0;i<MAP_H;i++){ // init map {0}
        for(j=0;j<MAP_W;j++){
            for(k=0;k<DATA;k++){
                    map[j][i][player][k]=0;
            }
        }
    }

    for(i=0;i<B4_NB;i++){ // place les bateaux de type 4
        printTableau(player,player);
        initPos(&X,&Y,B4_NOM,B4_TAILLE,player,4);
    }
    for(i=0;i<B3_NB;i++){ // place les bateaux de type 3
        printTableau(player,player);
        initPos(&X,&Y,B3_NOM,B3_TAILLE,player,3);
    }
    for(i=0;i<B2_NB;i++){ // place les bateaux de type 2
        printTableau(player,player);
        initPos(&X,&Y,B2_NOM,B2_TAILLE,player,2);
    }
    for(i=0;i<B1_NB;i++){ // place les bateaux de type 1
        printTableau(player,player);
        initPos(&X,&Y,B1_NOM,B1_TAILLE,player,1);
    }

    printTableau(player,player);
    getchar();
    system("cls");
} // Fin initMap

int getNbSave(){
    int nb, fini;
    char path[128]="";
    FILE* sauvegarde =NULL;
    fini=0;
    nb=0;
    while(fini == 0){
        sprintf(&path,"%s%d.save",SAVE_PATH,nb+1);
        sauvegarde = fopen(path,"r");
        if (sauvegarde == NULL){
            fini = 1;
        }else{
            nb++;
        }
        fclose(sauvegarde);
    }
    return nb;
}

void menu(int* toLoad){
    //Variables
    int a,b,c,d,i,j;
    int menu = 0;
    int nbSave=getNbSave();
    char path[200];
    FILE* sauvegarde= NULL;
    //Execution

    if (nbSave>0){
        printf("Une sauvegarde a ete trouvee, voulez vous :\n");
        printf("1 - Charger la derniere sauvegarde\n2 - Charger une autre sauvegarde\n3 - Commencer une nouvelle partie\n");
        scanf("%d",&menu);
        fflush(stdin);
        system("cls");
        if (menu == 1){ // Charger une sauvegarde
            *toLoad=nbSave;
            menu=-1;
        }else if (menu == 2){
            printf("Il y %d sauvegardes, laquelle voulez vous charger ?\n",nbSave);
            do{
                scanf("%d",toLoad);

                fflush(stdin);
            }while(*toLoad<=0 || *toLoad>nbSave);
            menu=-1;
        }else{
            menu=0;
        }
    }

    if (menu!=-1){
        // Nouvelle partie
        printf("Nouvelle Partie\n\n");
        *toLoad=nbSave+1;

        printf("Choisissez un mode de jeu :\n");
        printf("1 - Joueur contre Joueur\n2 - Joueur contre ordinateur\n3 - Ordinateur contre ordinateur\n");
        scanf("%d",&menu);
        fflush(stdin);
        //system("cls");
        GameMode = menu-1;

        sprintf(&path,"%s%d.save",SAVE_PATH,*toLoad);
        sauvegarde=fopen(path,"w");
            fprintf(sauvegarde,"%d %d %d",MAP_W,MAP_H,GameMode);
        fclose(sauvegarde);


        initMap(PLAYER1_ID);
        initMap(PLAYER2_ID);
        printf("tout les bateaux sont en place\n");

    }else{
        int w,h;
        sprintf(&path,"%s%d.save",SAVE_PATH,*toLoad);
        sauvegarde=fopen(path,"r");
            fscanf(sauvegarde,"%d %d %d",&w,&h,&GameMode);
            if (w!=MAP_W || h!=MAP_H){
                printf("ERREUR : la taille de la sauvegarde ne correspond pas a la configuration du jeu\n");
                exit(1);
            }else{
                printf("Chargement de la sauvegarde n %d ",*toLoad);
                    sprintf(&path,"%s%d.save",SAVE_PATH,*toLoad);
                    sauvegarde=fopen(path,"r");
                printf(".");
                    fscanf(sauvegarde,"%d %d %d",&a,&b,&c);
                printf(".");

                    for(i=0;i<MAP_H;i++){ // init map {0}
                        for(j=0;j<MAP_W;j++){
                            fscanf(sauvegarde,"%d:%d %d:%d",&a,&b,&c,&d);
                            map[j][i][PLAYER1_ID][TYPE]=a;
                            map[j][i][PLAYER1_ID][INFO]=b;
                            map[j][i][PLAYER2_ID][TYPE]=c;
                            map[j][i][PLAYER2_ID][INFO]=d;
                        }
                    }

                printf(". ");
                    fclose(sauvegarde);
                printf("Termine !\n");
                }
        fclose(sauvegarde);
    }
} // Fin menuPrincipal

int testFin(int player){
    int x,y,total;
    total=0;
    for(y=0;y<MAP_H;y++){
        for(x=0;x<MAP_W;x++){
            if(map[x][y][player][TYPE]==1){
                total++;
            }
        }
    }
    if (total==0){
        printf("Joueur %d a gagne !\n",player+1);
        return 1;
    }else{
        printf("Joueur %d n'a pas fini, il lui reste %d cases \n",player+1,total);
        return 0;
    }
}

void tir(int player){
    int valide,x,y;
    valide = 0;
    while(valide == 0){
        if(player+GameMode<=1){//si joueur != IA
            printf("entrez la position cible");
            getUserPos(&x,&y);
        }else if(IA==2){
            int total,i,j;
            total=0;
            for(j=0;j<MAP_H;j++){
                for(i=0;i<MAP_W;i++){
                    if(map[i][j][autreJoueur(player)][TYPE]==-1){
                        if(j>0 && map[i][j-1][autreJoueur(player)][TYPE]>=0){
                            x=i;
                            y=j-1;
                            total++;
                        }
                        if(i>0 && map[i-1][j][autreJoueur(player)][TYPE]>=0){
                            x=i-1;
                            y=j;
                            total++;
                        }
                        if(j<MAP_H-1 && map[i][j+1][autreJoueur(player)][TYPE]>=0){
                            x=i;
                            y=j+1;
                            total++;
                        }
                        if(i<MAP_W-1 && map[i+1][j][autreJoueur(player)][TYPE]>=0){
                            x=i+1;
                            y=j;
                            total++;
                        }
                    }
                    if(total>=1){break;}
                }
                if(total>=1){break;}
            }
            if(total==0){
                x=doRand(0,MAP_W-1);
                y=doRand(0,MAP_H-1);
            }

        }else{
            x=doRand(0,MAP_W-1);
            y=doRand(0,MAP_H-1);
        }
        if(map[x][y][autreJoueur(player)][TYPE]>=0){
            map[x][y][autreJoueur(player)][TYPE]-=2;
            valide = 1;
        }


    }
}

void sauvegarde(int aSauvegarder){
    char path[128];
    int i,j;

    sprintf(&path,"%s%d.save",SAVE_PATH,aSauvegarder);
    FILE* save= NULL;
        save=fopen(path,"w");
            fprintf(save,"%d %d %d\n",MAP_W,MAP_H,GameMode);
            for(i=0;i<MAP_H;i++){ // init map {0}
                for(j=0;j<MAP_W;j++){
                    fprintf(save,"%d:%d %d:%d\n",map[j][i][PLAYER1_ID][TYPE],map[j][i][PLAYER1_ID][INFO],map[j][i][PLAYER2_ID][TYPE],map[j][i][PLAYER2_ID][INFO]);
                }
            }
        fclose(save);
}

int main()
{
    int save=0;
    int tourJoueur=PLAYER1_ID;
    // Execution
    printf("Bataille navale Dv:3\n\n");
    menu(&save);

    while(testFin(PLAYER1_ID)==0&&testFin(PLAYER2_ID)==0){
        if(tourJoueur+GameMode<=1 || printIAtable == 1 || testFin(tourJoueur) == 1){//si joueur ou affichage tableau IA activé ou fini
            br(10);
            printf("Grille adversaire\n");
            printTableau(tourJoueur,autreJoueur(tourJoueur));
            printf("\nPosition des navires\n");
            printTableau(tourJoueur,tourJoueur);
        }
        tir(tourJoueur);
        printf(" joueur %d a tire\n",tourJoueur+1);
        tourJoueur=autreJoueur(tourJoueur);
        sauvegarde(save);
    }
    printf("Tableau du Joueur %d :\n",tourJoueur+1);
    printTableau(tourJoueur,tourJoueur);
    tourJoueur=autreJoueur(tourJoueur);
    printf("Tableau du Joueur %d :\n",tourJoueur+1);
    printTableau(tourJoueur,tourJoueur);
    return 0;
}
