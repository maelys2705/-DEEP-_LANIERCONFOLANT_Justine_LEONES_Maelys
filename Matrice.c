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
#include "Gestion.h"

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

#define COLOR_BLUE	0x00000F
#define COLOR_GREEN	0x0F0000
#define COLOR_YELLOW 0x090F00
#define COLOR_RED	0x000F00
#define COLOR_LIGHTBLUE 0x0E000F
#define COLOR_PINK 0x000F0F
#define COLOR_ORANGE 0x030F00
#define COLOR_BLACK	0x000000
#define COLOR_WHITE 0x0F0F0F

static uint32_t pixels[128];

void MATRICE_Initialisation(void){

	uint16_t j;
	LED_MATRIX_init();

	for(j=0;j<128;j++){
		pixels[j] = COLOR_BLUE ;
	}
	LED_MATRIX_display(pixels, 128);
}

void MATRICE_PerteVie(void){
	uint16_t j;
	LED_MATRIX_init();

	for(j=0;j<128;j++){
		pixels[j] = COLOR_RED ;
	}
	LED_MATRIX_display(pixels, 128);
	HAL_Delay(800);
}

void MATRICE_Affichage(char* grille){

	uint16_t i;

	for(i=0;i<128;i++)
	{
		if (grille[i]==0){
			pixels[i] = COLOR_BLACK ; //murs
		}
		if (grille[i]==1){
			pixels[i] = COLOR_BLUE ; //couloirs
		}
		if (grille[i]==2){
			pixels[i] = COLOR_PINK ; //portes
		}
		if (grille[i]==3){
			pixels[i] = COLOR_YELLOW ; //pacman
		}
		if (grille[i]==4){
			pixels[i] = COLOR_WHITE ; //pacpoints
		}
		if (grille[i]==5){
			pixels[i] = COLOR_PINK ; //fantôme rose
		}
		if (grille[i]==6){
			pixels[i] = COLOR_RED ; //fantôme rouge
		}
		if (grille[i]==7){
			pixels[i] = COLOR_LIGHTBLUE; //fantôme bleu
		}
		if (grille[i]==8){
			pixels[i] = COLOR_ORANGE ; //fantôme orange
		}

	}

	LED_MATRIX_display(pixels, 128);
	HAL_Delay(100);
}

