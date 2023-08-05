/*
 * stillAliveSignal.c
 *
 *  Created on: 31 lip 2023
 *      Author: Pietia
 */

#include "main.h"
#include "stillAliveSignal.h"
#include "IstillAliveTimer.h"
#include "led_blink.h"

static IstillAliveTimer *stillAliveTimer;

void stillAliveSignal_Init(void)
{
	stillAliveTimer = IstillAliveTimer_Init();

	led_init();

	stillAliveTimer->subscribe(led_toggle);
	stillAliveTimer->init();
}

void stillAliveSignal_Start(void)
{
	stillAliveTimer->start();
}

void stillAliveSignal_Stop(void)
{
	stillAliveTimer->stop();
}



