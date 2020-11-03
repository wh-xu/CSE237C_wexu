#include <math.h>
#include "dft.h"
#include "coefficients256.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	//Write your code here
	int i, j;
	DTYPE dft_real[SIZE];
	DTYPE dft_img[SIZE];
	int index;

	DFT:
	for (i = 0; i < SIZE; i++)
	{
		dft_real[i] = 0.0;
		dft_img[i] = 0.0;

		for (j = 0; j < SIZE; j++)
		{
			index = (i * j) % SIZE;
			dft_real[i] += real_sample[j] * cos_coefficients_table[index];
			dft_img[i] += real_sample[j] * sin_coefficients_table[index];
		}
	}

	for (i = 0; i < SIZE; i++)
	{
		real_sample[i] = dft_real[i];
		imag_sample[i] = dft_img[i];
	}
}
