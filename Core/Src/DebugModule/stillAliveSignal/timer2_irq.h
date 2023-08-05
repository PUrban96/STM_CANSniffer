/*
 * timer2_irq.h
 *
 *  Created on: 31 lip 2023
 *      Author: Pietia
 */

#pragma once

#include "main.h"
#include "IstillAliveTimer.h"

void timer2_init(void);
void timer2_start(void);
void timer2_stop(void);
void timer2_subscribe_irq(timer_Callback_t callback);


