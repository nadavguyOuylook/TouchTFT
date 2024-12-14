/*
 * GeneralMath.c
 *
 *  Created on: Dec 13, 2024
 *      Author: user
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>


/* Function to compute the interpolated value using cubic spline
 * requires ascending X values*/
float cubic_spline_interpolation(float x_input, uint16_t x[], uint16_t y[], int n)
{
    if (n != 3)
    {
        printf("Error: This function works for exactly 3 points.\n");
        return -1; // Error handling
    }

    float h[2], alpha[2], l[3], mu[3], z[3];
    float c[3] = {0}, b[2], d[2];
    float result = 0.0f;

    // Step 1: Calculate step sizes h[i] between points
    for (int i = 0; i < 2; i++)
    {
        h[i] = x[i + 1] - x[i];
    }

    // Step 2: Calculate alpha values
    for (int i = 1; i < 2; i++)
    {
        alpha[i] = (3.0f / h[i] * (y[i + 1] - y[i])) - (3.0f / h[i - 1] * (y[i] - y[i - 1]));
    }

    // Step 3: Solve tridiagonal system for z (natural spline boundary conditions)
    l[0] = 1.0f;
    mu[0] = 0.0f;
    z[0] = 0.0f;

    for (int i = 1; i < 2; i++)
    {
        l[i] = 2.0f * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[2] = 1.0f;
    z[2] = 0.0f;
    c[2] = 0.0f;

    for (int j = 1; j >= 0; j--)
    {
        c[j] = z[j] - mu[j] * c[j + 1];
        b[j] = (y[j + 1] - y[j]) / h[j] - h[j] * (c[j + 1] + 2.0f * c[j]) / 3.0f;
        d[j] = (c[j + 1] - c[j]) / (3.0f * h[j]);
    }

    // Step 4: Find the segment where x_input belongs and calculate interpolated y
    for (int i = 0; i < 2; i++)
    {
        if (x_input >= x[i] && x_input <= x[i + 1])
        {
            float dx = x_input - x[i];
            result = y[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
            break;
        }
    }

    return result;
}

/* Function to perform Lagrange Interpolation */
float lagrange_interpolation(float x_input, uint16_t x[], uint16_t y[], int n)
{
    float result = 0.0f;

    for (int i = 0; i < n; i++)
    {
        float term = y[i];
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                term = term * (x_input - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }

    return result;
}

/* Linear Interpolation Function */
float linear_interpolation(float x_input, uint16_t x[], uint16_t y[], int n)
{
    if (n < 2)
    {
        printf("Error: At least two points are required for linear interpolation.\n");
        return -1;  // Error handling
    }

    // Find the two points surrounding x_input
    for (int i = 0; i < n - 1; i++)
    {
        if ((x_input >= x[i] && x_input <= x[i + 1]) || (x_input <= x[i] && x_input >= x[i + 1]))
        {
            float x0 = x[i], y0 = y[i];
            float x1 = x[i + 1], y1 = y[i + 1];

            // Apply linear interpolation formula
            float result = y0 + ((x_input - x0) / (x1 - x0)) * (y1 - y0);
            return result;
        }
    }

    printf("Error: x_input is out of the provided range.\n");
    return -1;  // Return error if x_input is not in range
}

float distanceBetweenTwoPoints(uint16_t xP1, uint16_t yP1, uint16_t xP2, uint16_t yP2)
{
	float localRes = 0;

	localRes = sqrt( (float)( (xP1 - xP2) * (xP1 - xP2) + (yP1 - yP2) * (yP1 - yP2) ) );

	return (localRes);
}
