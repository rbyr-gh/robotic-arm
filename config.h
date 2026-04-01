/**
 *******************************************************************************
 * @file 	config.h
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de configuration de votre projet sur carte Nucléo STM32G431KB.
 * 			Permet d'activer les différents modules logiciels à votre disposition.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CONFIG_H_
#define CONFIG_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Defines -------------------------------------------------------------------*/
#define LED_GREEN_PIN 		GPIO_PIN_8
#define LED_GREEN_GPIO 		GPIOB

#define UART2_ON_PB4_PB3
#define UART1_ON_PA10_PA9

void temps_us(uint32_t us);

#define USE_BSP_TIMER		1
#define USE_BSP_EXTIT		1

#define USE_ADC				1
	/* Configuration pour activer les entrées analogiques souhaitées */
	#define USE_IN1		1 //Broche correspondante: PA0
	#define USE_IN2		1 //Broche correspondante: PA1
	#define USE_IN3		0 //Broche correspondante: PA6
	#define USE_IN4		0 //Broche correspondante: PA7
	#define USE_IN10	1 //Broche correspondante: PF1 /!\ Broche indisponible sur g431 sauf si SB11 et pas SB10 (petits pads à souder sur la carte)
	#define USE_IN15	1 //Broche correspondante: PA5

#define USE_DAC				0

/*------------------Afficheurs------------------*/
#define USE_ILI9341			1 // �cran TFT
#if USE_ILI9341
	#define USE_XPT2046			0
	#define USE_FONT7x10		0
	#define USE_FONT11x18		0
	#define USE_FONT16x26		1
#endif

/*------------------P�riph�riques------------------*/

#define I2C_TIMEOUT			5//ms

#if USE_ILI9341 || USE_SD_CARD || USE_MCP23S17 || USE_EPAPER // Liste des modules utilisants le p�riph�rique SPI
	#define USE_SPI				1
#else
	#ifndef USE_SPI
		#define USE_SPI				1
	#endif
#endif

#endif /* CONFIG_H_ */
