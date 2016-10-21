#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAP_H 10
#define MAP_W 10
#define NB_PLAYER 2
#define DATA 2
#define TYPE 0
#define INFO 1
#define PLAYER1_ID 0
#define PLAYER2_ID 1
//#define IA 1 //Mettre a 2 pour une IA plus difficile

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

int static map[MAP_W][MAP_H][NB_PLAYER][DATA];
int static GameMode; // 0=PVP, 1=PVE, 2=EVE


void br(){printf("\n");}// fonction de retour a la ligne
void indev(){

    printf("Fonctionalite en cours developpement ...");
    getchar();
    exit(0);
} // Fin indev

void color(int t,int f){
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
} // Fin color

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
} // Fin waitFor

int doRand(int startVal, int endVal){
    waitFor(0.05);
    srand(time(NULL)*rand());
    if(startVal == 0 && endVal == 1){
        return rand() % 2;
    }else{
        return (rand() % ((endVal + startVal -1)) + startVal);
    }
} // Fin doRand

char nbToLettre(int nb){
    return nb=(nb%26+65);;
} // Fin nbToLettre

void printTableau(int player){
    int x,y;
    for(y=-1;y<MAP_H;y++){
        color(7,0);
        if(y<9){
            printf(" ");
        }

        if(y>=0){
            printf("%d ",y+1);
        }else{
            printf("X ");
        }

        for(x=0;x<MAP_W;x++){
            color(7,0);
            if(y>=0){
                if(map[x][y][player][0]==0){
                    color(1,1);
                }else{
                    color(0,7);
                }
                printf("%d ",map[x][y][player][0]);
            }else{
                printf("%c ",nbToLettre(x));
            }
        }

        if(y>=0){
            br();
        }else{
            br();
            br();
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

void initPos(int *x,int *y,char nom[10],int taille,int player){
    int O,i,j;
    int valide =0;
    while(valide!=1){

    if(player+GameMode<=1){//si joueur != IA
        printf("entrez la position du %s (%d cases)",nom,taille);
        getUserPos(x,y);
    }else{
        *x=doRand(0,MAP_W-1);
        *y=doRand(0,MAP_H-1);
    }

    // debug
    printf("\n x:%d y:%d \n",*x,*y);
    // fin debug

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
                    for(j=*y+1-taille;j<=*y;j++){
                        if(valide==1 && map[*x][j][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }// Fin verication
                if(valide == 1){// si valide : positionner le bateau
                    for(j=*y+1-taille;j<=*y;j++){
                        map[*x][j][player][TYPE]=1;
                    }
                }//fin positionnement
            break;

            case 2://Bas
                if(*y<=MAP_H-taille){
                    valide = 1;
                    for(j=*y-1+taille;j>=*y;j--){
                        if(valide==1 && map[*x][j][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(j=*y-1+taille;j>=*y;j--){
                        map[*x][j][player][TYPE]=1;
                    }
                }
            break;
            case 3://Gauche
                if(*x+1>=taille){
                        valide = 1;
                    for(j=*x+1-taille;j<=*x;j++){
                        if(valide==1 && map[j][*y][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(j=*x+1-taille;j<=*x;j++){
                        map[j][*y][player][TYPE]=1;
                    }
                }
            break;
            case 4://Droite
                if(*x<=MAP_H-taille){
                    valide = 1;
                    for(j=*x-1+taille;j>=*x;j--){
                        if(valide==1 && map[j][*y][player][TYPE]!=0){
                            valide = 0;
                        }
                    }
                }
                if(valide == 1){
                    for(j=*x-1+taille;j>=*x;j--){
                        map[j][*y][player][TYPE]=1;
                    }
                }
            break;
        }

    }
} // Fin initPos

void initMap(int player){
    int i,j,k,l,X,Y,O;
    int valide = 0;

    printf("Tour du joueur %d\n",player+1);
    getchar();

    //Initialise le tableau a 0
    for(i=0;i<MAP_H;i++){ // init map {0}
        for(j=0;j<MAP_W;j++){
            for(k=0;k<NB_PLAYER;k++){
                for(l=0;l<DATA;l++){
                    map[j][i][k][l]=0;
                }
            }
        }
    }

    for(i=0;i<B4_NB;i++){ // place les bateaux de type 4
        printTableau(player);
        initPos(&X,&Y,B4_NOM,B4_TAILLE,player);
    }
    for(i=0;i<B3_NB;i++){ // place les bateaux de type 3
        printTableau(player);
        initPos(&X,&Y,B3_NOM,B3_TAILLE,player);
    }
    for(i=0;i<B2_NB;i++){ // place les bateaux de type 2
        printTableau(player);
        initPos(&X,&Y,B2_NOM,B2_TAILLE,player);
    }
    for(i=0;i<B1_NB;i++){ // place les bateaux de type 1
        printTableau(player);
        initPos(&X,&Y,B1_NOM,B1_TAILLE,player);
    }

    printTableau(player);
    getchar();
    system("cls");
} // Fin initMap

void menuPrincipal(){
    //Variables
    int menu = 0;
    //Execution
    printf("Voulez vous :\n");
    printf("1 - Charger une sauvegarde\n2 - Commencer une nouvelle partie\n");
    scanf("%d",&menu);
    fflush(stdin);
    system("cls");

    if (menu == 1){ // Charger une sauvegarde
        indev();
    }else{ // Nouvelle partie
        printf("Choisissez un mode de jeu :\n");
        printf("1 - Joueur contre Joueur\n2 - Joueur contre ordinateur\n3 - Ordinateur contre ordinateur\n");
        scanf("%d",&menu);
        fflush(stdin);
        system("cls");
        GameMode = menu-1;
    }
} // Fin menuPrincipal


int main()
{
    // Execution
    printf("Bataille navale Dv:2\n\n");
    menuPrincipal();
    initMap(PLAYER1_ID);
    initMap(PLAYER2_ID);
    return 0;
}
