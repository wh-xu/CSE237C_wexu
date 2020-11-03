#include <math.h>
#include "dft.h"
#include "coefficients256.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE out_real[SIZE], DTYPE out_img[SIZE])
{
	//Write your code here
	int i, j;
	int index;
	DTYPE temp_R, temp_I;

	DFT:
	for (i = 0; i < SIZE; i++)
	{
		temp_R = 0.0;
		temp_I = 0.0;

		for (j = 0; j < SIZE; j++)
		{
			#pragma HLS unroll factor = 64
			index = (i * j) % SIZE;
			temp_R += real_sample[j] * cos_coefficients_table[index];
			temp_I += real_sample[j] * sin_coefficients_table[index];
		}

		out_real[i] = temp_R;
		out_img[i]  = temp_I;
	}
}
