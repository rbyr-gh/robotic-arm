/*
 * fonction.h
 *
 *  Created on: 13 déc. 2025
 *      Author: romainbyr
 */

#ifndef FONCTION_H_
#define FONCTION_H_

#include "config.h"
#include "stm32g4_utils.h"

void TIMER2_user_handler_it(void);
void temps_us(uint32_t us);

#endif /* FONCTION_H_ */
