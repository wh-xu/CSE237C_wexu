#include "cordiccart2pol.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_TIMES 100
#define PI 3.141592653589793238

struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse()
	{
		num_sq = 0;
		sum_sq = 0;
		error = 0;
	}

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n * d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}
};

Rmse rmse_theta;
Rmse rmse_r;

void run_test(data_t x, data_t y, double golden_theta, double golden_r)
{
	data_t r;
	data_t theta;

	cordiccart2pol(x, y, &r, &theta);

	printf("Test: x=%.4f, y=%.4f,\t golden theta=%.4f,\t golden r=%.4f,\t your theta=%.4f,\t your r=%.4f\n",
		   (double)x, (double)y, golden_theta, golden_r, (double)theta, (double)r);

	float err_theta = abs((float)theta - golden_theta);
	float err_r = abs((float)r - golden_r);

	rmse_theta.add_value(err_theta);
	rmse_r.add_value(err_r);

	if (err_theta > 1e-2 || err_r > 1e-2)
		printf("Error exceeds threshold!\n");
}

void gen_random_data(data_t *a, data_t *b, data_t *golden_theta, data_t *golden_r)
{
	*a = (data_t)((rand() % 20000) - 1e4) / 1e4;
	*b = (data_t)((rand() % 20000) - 1e4) / 1e4;

	*golden_theta = atan(*b / (*a));

	if ((*a < 0) && (*b > 0))
		*golden_theta += PI;

	if ((*a < 0) && (*b < 0))
		*golden_theta += -PI;

	*golden_r = sqrt((*a) * (*a) + (*b) * (*b));
}

int main()
{
	data_t x;
	data_t y;
	data_t r;
	data_t theta;

	printf("---Testing results----------------------------------\n");

	// Test1
	run_test(0.8147, 0.1269, 0.154521173805634, 0.824523923242982);

	// Test 2
	run_test(0.6323, -0.2785, -0.414888412375609, 0.690916449362729);

	// Test 3
	run_test(-0.5469, -0.9575, -2.089751217098013, 1.102681214132172);

	// Test 4
	run_test(-0.4854, 0.7003, 2.176897679895152, 0.852075847562880);

	for (int i = 0; i < TEST_TIMES; i++)
	{
		gen_random_data(&x, &y, &theta, &r);
		run_test(x, y, theta, r);
	}

	printf("---RMS error----------------------------------\n");
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(Theta)\n");
	printf("%0.15f %0.15f\n", rmse_r.error, rmse_theta.error);
	printf("----------------------------------------------\n");

	float error_threshold = 0.001;

	int success = (rmse_r.error < error_threshold) && (rmse_theta.error < error_threshold);

	if (success)
		return 0;
	else
		return 1;
}
