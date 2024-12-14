/*
 * TFTAgent.h
 *
 *  Created on: Dec 7, 2024
 *      Author: user
 */

#ifndef INC_TFTAGENT_H_
#define INC_TFTAGENT_H_

// GPIO configuration for Chip Select (CS) pin
#define TOUCH_CS_PORT  GPIOD
#define TOUCH_CS_PIN   GPIO_PIN_15

// Define XPT2046 Control Bytes for X and Y positions
#define XPT2046_CMD_X  0x90  // 0b10010000 - X Position, 12-bit, differential
#define XPT2046_CMD_Y  0xD0  // 0b11010000 - Y Position, 12-bit, differential


extern uint8_t tftDisplayNextBuffer[153600];

extern void writeNextBufferToDisplay(void);
extern void prepareNewBufferForDisplay(void);
extern void updatePixelInBuffer(uint16_t xPos, uint16_t yPos, uint16_t selectedColor);

extern uint16_t XPT2046_Read(uint8_t cmd);
extern void XPT2046_GetTouchPosition(uint16_t *x, uint16_t *y);

#endif /* INC_TFTAGENT_H_ */
