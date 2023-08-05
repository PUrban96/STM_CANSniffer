/*
 * IstillAliveTimer.h
 *
 *  Created on: 1 sie 2023
 *      Author: purban
 */

#pragma once

#include "main.h"


typedef void (*timer_Callback_t)(void);

typedef void (*timer_Init_t)(void);
typedef void (*timer_Start_t)(void);
typedef void (*timer_Stop_t)(void);
typedef void (*timer_Subscribe_t)(timer_Callback_t callback);

typedef struct IstillAliveTimer
{
	timer_Init_t init;
	timer_Start_t start;
	timer_Stop_t stop;
	timer_Subscribe_t subscribe;
}IstillAliveTimer;


IstillAliveTimer *IstillAliveTimer_Init();

