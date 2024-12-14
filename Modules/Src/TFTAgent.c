/*
 * TFTAgent.c
 *
 *  Created on: Dec 7, 2024
 *      Author: user
 */
#include "main.h"
#include <stdint.h>
#include <string.h>
#include "TFTAgent.h"
#include "spi.h"



uint8_t tftDisplayNextBuffer[153600] = {0x00};

void writeNextBufferToDisplay(void)
{

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET); //CS - PE11
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET); //DC - PE12
	HAL_SPI_Transmit(&hspi1, &tftDisplayNextBuffer[0], 38400, 150);
	HAL_SPI_Transmit(&hspi1, &tftDisplayNextBuffer[38400], 38400, 150);
	HAL_SPI_Transmit(&hspi1, &tftDisplayNextBuffer[76800], 38400, 150);
	HAL_SPI_Transmit(&hspi1, &tftDisplayNextBuffer[115200], 38400, 150);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET); //CS - PE11
}

void prepareNewBufferForDisplay(void)
{
	memset(&tftDisplayNextBuffer[0], 0xFF, 153600);
}

// Function to read data from XPT2046
uint16_t XPT2046_Read(uint8_t cmd)
{
    uint8_t txData[3], rxData[3];
    uint16_t result;

    // Prepare the command to send
    txData[0] = cmd;
    txData[1] = 0x00;  // Dummy bytes
    txData[2] = 0x00;

    // Pull CS low to select the XPT2046
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);

    // Transmit command and receive data
    HAL_SPI_TransmitReceive(&hspi2, txData, rxData, 3, HAL_MAX_DELAY);

    // Pull CS high to deselect the XPT2046
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    // Combine the received data (12 bits)
    result = ((rxData[1] << 8) | rxData[2]) >> 4;  // Align 12-bit data

    return result;
}

// Function to get X and Y positions
void XPT2046_GetTouchPosition(uint16_t *x, uint16_t *y)
{
    *x = XPT2046_Read(XPT2046_CMD_X);
    *y = XPT2046_Read(XPT2046_CMD_Y);
}

void updatePixelInBuffer(uint16_t xPos, uint16_t yPos, uint16_t selectedColor)
{
	uint32_t localPosInBuffer = 0; // Y 0 - 239, X 0 - 319
	localPosInBuffer = (240 * xPos +  yPos) * 2;

	tftDisplayNextBuffer[localPosInBuffer] = (uint8_t)(selectedColor & 0x00FF);
	tftDisplayNextBuffer[localPosInBuffer + 1] = (uint8_t)((selectedColor & 0xFF00)>>8);
}
