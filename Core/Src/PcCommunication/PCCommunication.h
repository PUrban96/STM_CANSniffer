#pragma once

// TODO: Add send and received circular buffer

void PCComm_Init(void);

void PCComm_SendCanFrame(uint32_t CAN_ID, uint8_t CAN_DLC, const uint8_t Data[8]);