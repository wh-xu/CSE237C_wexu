#include <math.h>
#include "dft.h"

const float w = 2 * 3.1415926535 / SIZE;

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	//Write your code here
	int i, j;
	DTYPE dft_real[SIZE];
	DTYPE dft_img[SIZE];
	float angle;

	DFT:
	for (i = 0; i < SIZE; i++)
	{
		dft_real[i] = 0.0;
		dft_img[i] = 0.0;

		for (j = 0; j < SIZE; j++)
		{
			angle = w * i * j;
			dft_real[i] += real_sample[j] * cos(angle);
			dft_img[i] += -real_sample[j] * sin(angle);
		}
	}

	for (i = 0; i < SIZE; i++)
	{
		real_sample[i] = dft_real[i];
		imag_sample[i] = dft_img[i];
	}
}
