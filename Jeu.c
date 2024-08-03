#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "WS2812S.h"
#include "config.h"
#include "stm32f1_fonts.h"
#include "Menus.h"
#include "matrice.h"
#include "Gestion.h"
#include "Boutons.h"


#define COLOR_BLUE	0x00000F
#define COLOR_RED	0x00FF00
#define COLOR_GREEN	0xFF0000
#define COLOR_BLACK	0x000000
#define COLOR_YELLOW 0x0F0F00

#define COULOIRS 0
#define MURS 1
#define PORTES 2
#define PACMAN 3
#define PACPOINTS 4
#define FANTOME_ROSE 5
#define FANTOME_ROUGE 6
#define FANTOME_BLEU 7
#define FANTOME_ORANGE 8

#define HAUT 8
#define BAS -8
#define GAUCHE 1
#define DROITE -1

static uint32_t pixels[128];
static char score = 0;
static char vies = 3;
static char niveau = 1;

static int listePACPOINTS[] = {11 ,13, 75, 86, 118, 116, 53, 50};
static size_t tailleListePACPOINTS = sizeof(listePACPOINTS) / sizeof(listePACPOINTS[0]);
static char grille[128];
static int spawnfantomes[4] = {55, 112, 104, 47};

void JEU_SetListePACPOINTS(){
	int copieliste[] = {11, 13, 75, 86, 118, 116, 53, 50};
	for (int i=0; i<8; i++){
		listePACPOINTS[i]=copieliste[i];
	}
	tailleListePACPOINTS = 8;
}
//void JEU_CreationGrille(int listeMURS[], int listePACPOINTS[], char grille[], size_t tailleListeMURS, size_t tailleListePACPOINTS){
void JEU_CreationGrille(int* listeMURS, int *listePACPOINTS, char* grille, size_t tailleListeMURS, size_t tailleListePACPOINTS){
	for (int i = 0; i < tailleListeMURS; ++i){
		grille[listeMURS[i]]= MURS;
	}
	for (size_t i = 0; i < tailleListePACPOINTS; ++i){
			grille[listePACPOINTS[i]]= PACPOINTS;
	}

	//points de spawn des fantômes et de Pac-Man
	grille[9]=PACMAN;
	grille[112]=FANTOME_ROSE;
	grille[104]=FANTOME_BLEU;
	grille[55]=FANTOME_ORANGE;
	grille[47]=FANTOME_ROUGE;
}

void JEU_initialisationParametres(char viesToInit, char scoreToInit, char niveauToInit, char* grilleToInit){
	for (int i;i<128;i++){
		grille[i] = grilleToInit[i];
	}
	score = scoreToInit;
	vies = viesToInit;
	niveau = niveauToInit;
}


char JEU_GetScore(){
	return score;
}
char JEU_GetNiveau(){
    return niveau;
}

void JEU_ChangementDeNiveau(){
    niveau++;
}

char JEU_GetVies(){
	return vies;
}

void JEU_DeplacementPacMan(int mouvement, char *grille) { //mouvement peut prendre 1, -1, 8, -8

	int positionpacman ;

	// Récupérer la position de PacMan dans la grille
    for (int i = 0; i < 128 ; ++i) {
        if (grille[i]==3) {
            positionpacman = i;
        }
    }

    // Calcul de la nouvelle position
    int nouvelleposition;
    nouvelleposition = positionpacman + mouvement;

    // VERIFICATION N°1 : Vérifie si la prochaine case est un pacpoint
    if (grille[nouvelleposition] == PACPOINTS) {
        score += 10;
        MENUS_ActualisationJeuEnCours(vies, score, niveau);

        // Recherche de l'indice du Pac-Points à manger dans listePACPOINTS
        int indicePacPointMange = -1;
        for (int i = 0; i < tailleListePACPOINTS; i++) {
            if (nouvelleposition == listePACPOINTS[i]) {
                indicePacPointMange = i;
                break;
            }
        }

        // Suppression du Pac-Points mangé dans listePACPOINTS
        for (int i = indicePacPointMange; i < tailleListePACPOINTS - 1; i++) {
        	listePACPOINTS[i] = listePACPOINTS[i + 1];
        }
        tailleListePACPOINTS--;
    }
    // VERIFICATION N°2 : Vérifie si la nouvelle position est dans la liste des murs (listes des LED bleu)
    int isValidMove = 0;
    if (grille[nouvelleposition]==1) {
        isValidMove = 1;
    }

    // VERIFICATION N°3 : Vérifie si la nouvelle position est un fantôme
    if (grille[nouvelleposition]==FANTOME_ROSE || grille[nouvelleposition]==FANTOME_BLEU || grille[nouvelleposition]==FANTOME_ORANGE || grille[nouvelleposition]==FANTOME_ROUGE) {
    	isValidMove = 1;
        vies = vies-1 ;
        MENUS_ActualisationJeuEnCours(vies, score, niveau);
        MATRICE_PerteVie();

        // Si le joueur touche le fantôme respawn sur l'un des 4 points de spawn disponible
        for (int i=0;i<3;i++){
        	if (spawnfantomes[i]!=FANTOME_ROSE && spawnfantomes[i]!=FANTOME_BLEU && spawnfantomes[i]!=FANTOME_ORANGE && spawnfantomes[i]!=FANTOME_ROUGE){
        		grille[spawnfantomes[i]]=grille[nouvelleposition];
        		grille[nouvelleposition]=COULOIRS;
        		break;
        	}
        }
    	MATRICE_Affichage(grille);
    	isValidMove =1;
    }

    // Pour passer d'une matrice à l'autre
    if (positionpacman == 31 && mouvement == GAUCHE && isValidMove == 1){
    	nouvelleposition = 88 ;
    	isValidMove = 0 ;
    }
    if (positionpacman == 88 && mouvement == DROITE && isValidMove == 1){
        nouvelleposition = 31 ;
        isValidMove = 0 ;
    }
    if (positionpacman == 47 && mouvement == GAUCHE && isValidMove == 1){
        nouvelleposition = 104 ;
        isValidMove = 0 ;
    }
    if (positionpacman == 104 && mouvement == DROITE && isValidMove == 1){
        nouvelleposition = 47 ;
        isValidMove = 0 ;
    }


    // Si isValidMove est à 0, on peut faire avancer Pac Man car celui-ci n'avance pas dans un mur ou un fantome
    if (isValidMove==0) {
    	grille[positionpacman]=0;
    	grille[nouvelleposition]=3;
    }
}

int JEU_DeplacementFantomes(char *grille) {

    int positionsFantomes[4]; // Tableau pour stocker les positions des 4 fantômes (rose, bleu, orange, rouge)
    // Récupérer la position de chaque fantôme dans la grille
    for (int i = 0; i < 128; ++i) {
        if (grille[i] == FANTOME_ROSE || grille[i] == FANTOME_BLEU || grille[i] == FANTOME_ORANGE || grille[i] == FANTOME_ROUGE) {
            positionsFantomes[grille[i]-5] = i;
        }
    }

    // Déplacement de chaque fantôme
    for (int i = 0; i < 4; ++i) {
        int positionFantome = positionsFantomes[i];
        int mouvementsPossibles[] = {-1, 1, -8, 8};
        int mouvement = mouvementsPossibles[rand() % 4];
        int nouvellePosition = positionFantome + mouvement;

    // Pour passer d'une matrice à l'autre
    	// Entre Matrices 1 & 2
    if (positionFantome == 31 && mouvement == GAUCHE){
    	nouvellePosition = 88 ;
    }
    if (positionFantome == 88 && mouvement == DROITE){
    	nouvellePosition = 31 ;
    }
    if (positionFantome == 47 && mouvement == GAUCHE){
    	nouvellePosition = 104 ;
    }
    if (positionFantome == 104 && mouvement == DROITE){
    	nouvellePosition = 47 ;
    }

    	//Détecte la présence ou non d'un mur/pac-points
        if (grille[nouvellePosition] == COULOIRS || grille[nouvellePosition] == PACPOINTS) {
            grille[positionFantome] = 0;
            grille[nouvellePosition] = i+5;
        }


        //Le joueur se fait toucher
        if (grille[nouvellePosition] == PACMAN) {
            //Vérifier sur le fantôme peut respawn
            for (int i=0;i<3;i++){
            	if (spawnfantomes[i]!=FANTOME_ROSE && spawnfantomes[i]!=FANTOME_BLEU && spawnfantomes[i]!=FANTOME_ORANGE && spawnfantomes[i]!=FANTOME_ROUGE){
            		grille[spawnfantomes[i]]=grille[positionFantome];
            		grille[positionFantome]=COULOIRS;
            		break;
            	}
            }
            vies = vies-1 ;
            MENUS_ActualisationJeuEnCours(vies, score, niveau);
            MATRICE_PerteVie();
        	MATRICE_Affichage(grille);
        }

        //Remplace les pac-points dès que le fantôme est passé dessus
                for (int i=0; i<tailleListePACPOINTS ; i++){
                	if (grille[listePACPOINTS[i]]==COULOIRS){
                		grille[listePACPOINTS[i]]=PACPOINTS;
                	}
                }

    }
}

int JEU_ifLevelDone(totalpoints){
	if (score==totalpoints){
		return 1;
	}
	return 0;
}


