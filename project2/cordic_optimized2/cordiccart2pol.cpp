#include "cordiccart2pol.h"
#include <stdio.h>

//data_t Kvalues[NO_ITER] = {0.70710678118655, 0.63245553203368, 0.61357199107790, 0.60883391251775, 0.60764825625617, 0.60735177014130, 0.60727764409353, 0.60725911229889, 0.60725447933256, 0.60725332108988, 0.60725303152913, 0.60725295913894, 0.60725294104140, 0.60725293651701, 0.60725293538591, 0.60725293510314};
data_fixed angles[NO_ITER] = {0.785398163397448, 0.463647609000806, 0.244978663126864, 0.124354994546761, 0.0624188099959574, 0.0312398334302683, 0.0156237286204768, 0.00781234106010111, 0.00390623013196697, 0.00195312251647882, 0.000976562189559320, 0.000488281211194898, 0.000244140620149362, 0.000122070311893670, 6.10351561742088e-05, 3.05175781155261e-05};

void cordiccart2pol(data_t x, data_t y, data_t *r, data_t *theta)
{
	// Write your code here
	// Convert the floating-point inputs to fixed-point representation
	data_fixed fixed_x = x;
	data_fixed fixed_y = y;
	data_fixed x_old;
	data_fixed fixed_theta = 0.0;
	bool flag_neg_x, flag_neg_y;

	flag_neg_x = fixed_x[W - 1]? 1: 0;
	flag_neg_y = fixed_y[W - 1]? 1: 0;

	Preprocessing:
	if (flag_neg_x)
	{
		x_old = fixed_x;

		if (flag_neg_y)
		{
			fixed_x = -fixed_y;
			fixed_y = x_old;
			fixed_theta = -1.57079632;
		}
		else
		{
			fixed_x = fixed_y;
			fixed_y = -x_old;
			fixed_theta = 1.57079632;
		}
	}

	Rotation:
	for (unsigned char i = 0; i < ITER_RUN; i++)
	{
		x_old = fixed_x;

		if (flag_neg_y)
		{
			fixed_x -= fixed_y >> i;
			fixed_y += x_old >> i;
			fixed_theta -= angles[i];
		}
		else
		{
			fixed_x += fixed_y >> i;
			fixed_y -= x_old >> i;
			fixed_theta += angles[i];
		}

		flag_neg_y = fixed_y[W - 1]? 1: 0;
	}

	*r = fixed_x * (data_fixed)(0.60725293510314);
	*theta = fixed_theta;
}
