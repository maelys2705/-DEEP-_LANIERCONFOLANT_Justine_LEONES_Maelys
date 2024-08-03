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
#include "matrice.h"
#include "Gestion.h"

#define START 0
#define QUIT 1

void MENUS_Principal (int selection){

	ILI9341_Init();
	ILI9341_setConfig();

	ILI931_Options_t options = ILI9341_getOptions();
	uint16_t screenWidth = options.width;
	uint16_t screenHeight = options.height;

	// Choisir la position pour centrer le texte "Pac Man"
	uint16_t x = ((screenWidth)/2);
	uint16_t y = ((screenHeight)/2);

    ILI9341_Fill(ILI9341_COLOR_BLACK);
	ILI9341_Puts(x-40, y-120, "Pac Man", &Font_11x18, 0xFFFF00, 0x000000);
	ILI9341_Puts(x-30, y+20, "START", &Font_11x18, 0xFFFFFF, 0x000000);
	ILI9341_Puts(x-23, y+60, "QUIT", &Font_11x18, 0xFFFFFF, 0x000000);
	ILI9341_Puts(x-60, screenHeight-30, "Justine & Maelys", &Font_11x18, 0xFFFFFF, 0x000000);
	ILI9341_DrawFilledCircle((screenWidth)/2, y-50, 30, 0xFFFF00);
	ILI9341_DrawLine((screenWidth/2), y-50, (screenHeight/2)+1.22*30, (y+(1/2)*30)-50, 0x000000);
	ILI9341_DrawLine((screenWidth/2), y-50, (screenHeight/2)+1.22*30, (y-(1/2)*30)-50, 0x000000);
	ILI9341_DrawFilledCircle(((screenWidth)/2)+10, y-60, 3, 0x000000);
	
	if (selection==0){
		ILI9341_DrawCircle(x-40, y+20, 3, 0xFFFFFF);
	}
	if (selection==1){
		ILI9341_DrawCircle(x-33, y+60, 3, 0xFFFFFF);
	}
	
}


void MENUS_ActualisationJeuEnCours(char vies, char score, char niveau){

	ILI931_Options_t options = ILI9341_getOptions();
	uint16_t screenWidth = options.width;
	uint16_t screenHeight = options.height;
	uint16_t x = ((screenWidth)/2);
	uint16_t y = ((screenHeight)/2);

	char scoreStr[10];
    char viesStr[10];
    char niveauStr[10];

    // Conversion des valeurs en chaînes de caractères
    sprintf(scoreStr, "Score : %d", score);
    sprintf(viesStr, "Vies : %d", vies);
    sprintf(niveauStr, "Niveau : %d", niveau);

    ILI9341_Fill(ILI9341_COLOR_BLACK);
    ILI9341_DrawRectangle(20, 20, screenWidth-20, screenHeight-20, 0xFFFFFF);
    ILI9341_Puts(x-60, y - 100, scoreStr, &Font_11x18, 0xFFFFFF, 0x000000);
    ILI9341_Puts(x-60, y - 80, viesStr, &Font_11x18, 0xFFFFFF, 0x000000);
    ILI9341_Puts(x-60, y - 120, niveauStr, &Font_11x18, 0xFFFFFF, 0x000000);
}

void MENUS_GameOver(char score){
	ILI931_Options_t options = ILI9341_getOptions();
		uint16_t screenWidth = options.width;
		uint16_t screenHeight = options.height;

		// Choisir la position pour centrer le texte "Pac Man"
		uint16_t x = ((screenWidth)/2);
		uint16_t y = ((screenHeight)/2);

	    ILI9341_Fill(ILI9341_COLOR_BLACK);
		ILI9341_Puts(x-90, y-40, "G A M E   O V E R", &Font_11x18, 0xFFFFFF, 0x000000);

		char scoreStr[10];
		sprintf(scoreStr, "Score : %d", score);
		ILI9341_Puts(x-50, y+20, scoreStr, &Font_11x18, 0xFFFFFF, 0x000000);

		ILI9341_DrawLine((screenWidth/2)-40, y-50, (screenHeight/2)+1.22*30-40, (y+(1/2)*30)-50, 0x000000);
		ILI9341_DrawLine((screenWidth/2)-40, y-50, (screenHeight/2)+1.22*30-40, (y-(1/2)*30)-50, 0x000000);
		ILI9341_DrawFilledCircle(((screenWidth)/2)-30, y-60, 3, 0x000000);
}

void MENUS_Sortie(void){
	ILI931_Options_t options = ILI9341_getOptions();
	uint16_t screenWidth = options.width;
	uint16_t screenHeight = options.height;
	uint16_t x = ((screenWidth)/2);
	uint16_t y = ((screenHeight)/2);
	
	ILI9341_Fill(ILI9341_COLOR_BLACK);
	ILI9341_Puts(x-17, y-50, "Bye !", &Font_11x18, 0xFFFFFF, 0x000000);
	HAL_Delay(1500);
	ILI9341_Fill(ILI9341_COLOR_BLACK);
}
