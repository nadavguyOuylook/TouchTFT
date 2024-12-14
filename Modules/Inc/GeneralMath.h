/*
 * GeneralMath.h
 *
 *  Created on: Dec 13, 2024
 *      Author: user
 */

#ifndef INC_GENERALMATH_H_
#define INC_GENERALMATH_H_



float cubic_spline_interpolation(float x_input, uint16_t x[], uint16_t y[], int n);
float lagrange_interpolation(float x_input, uint16_t x[], uint16_t y[], int n);
float linear_interpolation(float x_input, uint16_t x[], uint16_t y[], int n);
float distanceBetweenTwoPoints(uint16_t xP1, uint16_t yP1, uint16_t xP2, uint16_t yP2);

#endif /* INC_GENERALMATH_H_ */
