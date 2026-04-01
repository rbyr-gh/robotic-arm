/**
 *******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte Nucléo STM32G431KB
 *******************************************************************************
 */

#include "config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "stm32g4_sys.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"

#include <../drivers/bsp/tft_ili9341/stm32g4_ili9341.h>
#include <../drivers/bsp/tft_ili9341/stm32g4_fonts.h>
#include <../drivers/bsp/stm32g4_adc.h>
#include <../drivers/bsp/stm32g4_timer.h>
#include <fonction.h>

#define BANDEMORTE 1


volatile uint8_t expire = 0;
/**
  * @brief  Point d'entrée de votre application
  */
int main(void)
{

	float f_tmp;
	volatile uint16_t u8_moteur_pos = 0;
	uint16_t u8_moteur_cible_w 		= 0;
	uint16_t u8_moteur_cible 		= 0;

	uint16_t 	u8_pota1; // ADC1
	uint16_t 	u8_pota1_avt = 0;

	uint16_t 	u8_pota2; //ADC2
	uint16_t 	u8_pota2_avt = 0;

	uint16_t 	u8_pota3; //ADC15
	uint16_t 	u8_pota3_avt = 0;

	uint16_t 	u8_pota4_avt = 0; //ADC10
	uint16_t 	u8_pota4_act;


	BSP_UART_init(UART2_ID, 115200);
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	BSP_ADC_init(); // Initialisation de l'ADC

	ILI9341_Init();  // Initialisation de l'écran

	BSP_TIMER_run_us(TIMER3_ID,200000,false);
	BSP_TIMER_run_us(TIMER4_ID,200000,false);
	BSP_TIMER_run_us(TIMER1_ID,4000,true);


	// Initialisation des PWM
	BSP_TIMER_enable_PWM(TIMER3_ID,TIM_CHANNEL_1,500,true,false); // PB4 - ADC1 (PA0)
	BSP_TIMER_enable_PWM(TIMER4_ID,TIM_CHANNEL_2,90,false,false); // PA12 - ADC2 (PA1)
	BSP_TIMER_enable_PWM(TIMER4_ID,TIM_CHANNEL_1,90,true,false); // PB6 - ADC15 (PB0)
	BSP_TIMER_enable_PWM(TIMER1_ID,TIM_CHANNEL_2,1000,false,false); // PA9 - ADC10 (PF0)
	BSP_TIMER_enable_PWM(TIMER1_ID,TIM_CHANNEL_3,1000,false,false); // PA10


	while(1)
	{


		/*
		 *  Contrôle du premier servomoteur
		 */
		u8_pota1 = BSP_ADC_getValue(ADC_1);
		u8_pota1_avt = (u8_pota1_avt * 14 + u8_pota1) / 15;
		f_tmp = (float)u8_pota1_avt;
		f_tmp /= 4095.0f;
		f_tmp *= 100.0f;
		f_tmp += 25.0f;
		u8_pota1 = (uint16_t)f_tmp;
		BSP_TIMER_set_duty(TIMER3_ID,TIM_CHANNEL_1,u8_pota1);


		/*
		 *  Contrôle du second servomoteur
		 */
		u8_pota2 = BSP_ADC_getValue(ADC_2);
		u8_pota2_avt = (u8_pota2_avt * 14 + u8_pota2) / 15;
		f_tmp = (float)u8_pota2_avt;
		f_tmp /= 4095.0f;
		f_tmp *= 100.0f;
		f_tmp += 25.0f;
		u8_pota2 = (uint16_t)f_tmp;
		BSP_TIMER_set_duty(TIMER4_ID,TIM_CHANNEL_2,u8_pota2);


		/*
		 *  Contrôle du troisième servomoteur
		 */
		u8_pota3 = BSP_ADC_getValue(ADC_15);
		u8_pota3_avt = (u8_pota3_avt * 14 + u8_pota3) / 15;
		f_tmp = (float)u8_pota3_avt;
		f_tmp /= 4095.0f;
		f_tmp *= 100.0f;
		f_tmp += 25.0f;
		u8_pota3 = (uint16_t)f_tmp;
		BSP_TIMER_set_duty(TIMER4_ID,TIM_CHANNEL_1,u8_pota3);



		/*
		 *  Contrôle du moteur pas à pas
		 */

		u8_pota4_act = (uint16_t)(BSP_ADC_getValue(ADC_10)/8);
		u8_pota4_avt = (u8_pota4_avt * 25 + u8_pota4_act) / 26;
		f_tmp = (float)u8_pota4_avt;
		f_tmp /= 4095.0f;
		f_tmp *= 3200.0f;
		f_tmp /= 1.5f;
		u8_moteur_cible = (uint16_t)f_tmp;


		if (abs(u8_moteur_pos - u8_moteur_cible) > BANDEMORTE){
			u8_moteur_cible_w = u8_moteur_cible;
		}



		if (u8_moteur_pos < u8_moteur_cible_w){
			BSP_TIMER_set_duty(TIMER1_ID,TIM_CHANNEL_2,1000);
			BSP_TIMER_set_duty(TIMER1_ID,TIM_CHANNEL_3,500);

			while (u8_moteur_pos < u8_moteur_cible_w){
				temps_us(31995);
				u8_moteur_pos++;
			temps_us(2000);
			}

		}
		else if (u8_moteur_pos > u8_moteur_cible_w){
			BSP_TIMER_set_duty(TIMER1_ID,TIM_CHANNEL_2,0);
			BSP_TIMER_set_duty(TIMER1_ID,TIM_CHANNEL_3,500);
			while (u8_moteur_pos > u8_moteur_cible_w){
				temps_us(31995);
				u8_moteur_pos--;
			temps_us(2000);
			}
		}
		BSP_TIMER_set_duty(TIMER1_ID,TIM_CHANNEL_3,1000);


		/*
		 * Fin de la boucle
		 */
	}
}


