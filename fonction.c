/*
 * fonction.c
 *
 *  Created on: 13 déc. 2025
 *      Author: romainbyr
 */


#include <fonction.h>

#include <stdint.h>


#include <../drivers/bsp/stm32g4_timer.h>
#include <../drivers/bsp/stm32g4_adc.h>


static uint16_t expire;

void TIMER2_user_handler_it(void){
	expire = 1;
	BSP_TIMER_stop(TIMER2_ID);

}

void temps_us(uint32_t us){
	expire = 0;

	BSP_TIMER_run_us(TIMER2_ID,us,true);

	while(!expire){

	}
}


/*
 *
 *  	Fonction pour l'écran TFT
 *
 */
