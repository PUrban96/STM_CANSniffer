#include "main.h"
#include "CANCommunication.h"
#include "ICAN_LowLevel.h"

static ICANLowLevel *CANCommunication_LowLevel;

void CANCommunication_Init(void)
{
    CANCommunication_LowLevel = ICANLowLevel_Init();
    
    CANCommunication_LowLevel->init();
}