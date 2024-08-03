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
#include "Jeu.h"
#include "Matrice.h"
#include "Boutons.h"


#define BUTTON_PIN0 GPIO_PIN_0
#define BUTTON_PORT0 GPIOA
#define BUTTON_PIN1 GPIO_PIN_1
#define BUTTON_PORT1 GPIOA
#define BUTTON_PIN2 GPIO_PIN_2
#define BUTTON_PORT2 GPIOA
#define BUTTON_PIN3 GPIO_PIN_3
#define BUTTON_PORT3 GPIOA
#define BUTTON_PIN4 GPIO_PIN_4
#define BUTTON_PORT4 GPIOA

#define COULOIRS 0
#define MURS 1
#define PORTES 2
#define PACMAN 3
#define PACPOINTS 4

#define HAUT 8
#define BAS -8
#define GAUCHE 1
#define DROITE -1

#define START 0
#define QUIT 1

#define BOUTON_DROIT BUTTON_PIN3, BUTTON_PORT3
#define BOUTON_HAUT BUTTON_PIN0, BUTTON_PORT0
#define BOUTON_CENTRE BUTTON_PIN2, BUTTON_PORT2
#define BOUTON_BAS BUTTON_PIN4, BUTTON_PORT4
#define BOUTON_GAUCHE BUTTON_PIN1, BUTTON_PORT1

// Définition des états de la machine à états
typedef enum {
    STATE_MENUPRINCIPAL,
    STATE_INGAME,
    STATE_GAMEOVER,
	STATE_QUIT,
} GameState;

static void GESTION_GameOver();
void GESTION_Quit();

// Variable d'état globale
GameState currentState = STATE_MENUPRINCIPAL;

void GESTION_main() {

    while (1) {
        // Machine à états principale
        switch (currentState) {
            case STATE_MENUPRINCIPAL:
                GESTION_Accueil();
                break;

            case STATE_INGAME:
                GESTION_InGame();
                break;

            case STATE_GAMEOVER:
                GESTION_GameOver();
                break;

            case STATE_QUIT:
                GESTION_Quit();
                break;
        }
    }

}

// Fonction pour gérer l'état du menu
void GESTION_Accueil() {

    int pointeur = START;

    MENUS_Principal(pointeur);
    MATRICE_Initialisation();

    while (currentState == STATE_MENUPRINCIPAL) {
        if (BOUTONS_SiPresse(BOUTON_HAUT)) {
            pointeur = START;
        	MENUS_Principal(pointeur);
        }

        if (BOUTONS_SiPresse(BOUTON_CENTRE)) {
            if (pointeur == START) {
            	currentState = STATE_INGAME;

            } else if (pointeur == QUIT) {
                currentState = STATE_QUIT;
            }
        }

        if (BOUTONS_SiPresse(BOUTON_BAS)) {
            pointeur = QUIT;
            MENUS_Principal(pointeur);
        }

        HAL_Delay(100);  // Délai pour éviter le traitement continu des boutons
    }
}

void GESTION_InGame(){

	// Initialisation du début du jeu
	int listeMURS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 18, 19, 21, 22, 24, 26, 32, 34, 36, 38, 39, 40, 44, 48, 52, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 79, 80, 81, 82, 84, 85, 87, 92, 95, 96, 97, 103, 107, 109, 111, 113, 115, 117, 119, 120, 121, 122, 123, 124, 125, 126, 127};
	int listePACPOINTS[] = {11 ,13, 75, 86, 118, 116, 53, 50};
	char grille[128] = {COULOIRS};
	char copiegrille[128];

	char vies = 3 ;
	char score = 0 ;
	char niveau = 1 ;

	size_t tailleListeMURS = sizeof(listeMURS) / sizeof(listeMURS[0]);
	size_t tailleListePACPOINTS = sizeof(listePACPOINTS) / sizeof(listePACPOINTS[0]);

	int totalpoints = tailleListePACPOINTS*10;

	JEU_initialisationParametres(3, 0, 1, grille);
	JEU_CreationGrille(listeMURS, listePACPOINTS, grille, tailleListeMURS, tailleListePACPOINTS);
	for (int i=0;i<128;i++){
		copiegrille[i] = grille[i];
	}
	MENUS_ActualisationJeuEnCours(vies, score, niveau);

	while(currentState == STATE_INGAME){
	// Vérifie si le joueur à réussi le niveau
		if (JEU_ifLevelDone(totalpoints)){
			MATRICE_Initialisation();
			for (int i=0;i<128;i++){
				grille[i] = copiegrille[i];
			}
			JEU_ChangementDeNiveau();
			niveau = JEU_GetNiveau();
			score = JEU_GetScore();
			vies = 3;

			totalpoints = totalpoints+tailleListePACPOINTS*10;
			JEU_SetListePACPOINTS();
			JEU_initialisationParametres(3, score, niveau, grille);
			MENUS_ActualisationJeuEnCours(vies, score, niveau);
			HAL_Delay(800);
			MATRICE_Affichage(grille);
		}
		for(int i=0;i<niveau;i++){
			JEU_DeplacementFantomes(grille);
		}
		if (JEU_GetVies()==0){
			currentState = STATE_GAMEOVER;
			break;
		}
		MATRICE_Affichage(grille);
		if(currentState == STATE_INGAME){
				    if (BOUTONS_SiPresse(BOUTON_DROIT)) {
				    	JEU_DeplacementPacMan(DROITE, grille);
				    	if (JEU_GetVies()==0){
				    		currentState = STATE_GAMEOVER;
				    	}
				    	MATRICE_Affichage(grille);
				    }
				    if (BOUTONS_SiPresse(BOUTON_HAUT)) {
				    	JEU_DeplacementPacMan(HAUT, grille);
				    	if (JEU_GetVies()==0){
				    		currentState = STATE_GAMEOVER;
				    	}
				    	MATRICE_Affichage(grille);
				    }
				    if (BOUTONS_SiPresse(BOUTON_BAS)) {
				    	JEU_DeplacementPacMan(BAS, grille);
				    	if (JEU_GetVies()==0){
				    		currentState = STATE_GAMEOVER;
				    	}
				    	MATRICE_Affichage(grille);
				    }
				    if (BOUTONS_SiPresse(BOUTON_GAUCHE)) {
				    	JEU_DeplacementPacMan(GAUCHE, grille);
				    	if (JEU_GetVies()==0){
				    		currentState = STATE_GAMEOVER;
				    	}
				    	MATRICE_Affichage(grille);
				    }

	HAL_Delay(50);
	}
	}
}
static void GESTION_GameOver(){
	char score = JEU_GetScore();
	MENUS_GameOver(score);
	HAL_Delay(3000);
	currentState = STATE_MENUPRINCIPAL;
}
void GESTION_Quit(){
	MENUS_Sortie();
}
