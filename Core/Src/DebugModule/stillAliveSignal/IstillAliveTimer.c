/*
 * IstillAliveTimer.c
 *
 *  Created on: 1 sie 2023
 *      Author: purban
 */

#include "IstillAliveTimer.h"
#include "stillAliveSignal.h"
#include "timer2_irq.h"

static IstillAliveTimer timer;

IstillAliveTimer *IstillAliveTimer_Init()
{
	timer.init = timer2_init;
	timer.start = timer2_start;
	timer.stop = timer2_stop;
	timer.subscribe = timer2_subscribe_irq;

	return &timer;
}
