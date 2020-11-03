#include <math.h>
#include "dft.h"
#include "coefficients256.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE out_real[SIZE], DTYPE out_img[SIZE])
{
	//Write your code here
	int i, j;
	int index;

	DFT:
	for (i = 0; i < SIZE; i++)
	{
		out_real[i] = 0.0;
		out_img[i]  = 0.0;
		
		for (j = 0; j < SIZE; j++)
		{
			index = (i * j) % SIZE;
			out_real[i] += real_sample[j] * cos_coefficients_table[index];
			out_img[i] += real_sample[j] * sin_coefficients_table[index];
		}
	}
}
