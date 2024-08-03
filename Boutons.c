
#include "stm32f1_gpio.h"
#include "stm32f1xx_hal.h"
#include "config.h"

//Boutons sur les broches A0, A1, A2, A3, A4
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

// Fonction pour lire l'état du bouton (pressé ou non)
int BOUTONS_SiPresse(uint16_t buttonPin, GPIO_TypeDef *buttonPort){
    return !(buttonPort->IDR & buttonPin); // Le bouton est pressé lorsque le bit correspondant est à 0
 }
