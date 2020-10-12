/*
	Filename: fir.cpp
		FIR lab wirtten for WES/CSE237C class at UCSD.

	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output

*/

#include "fir.h"

void fir (
  data_t *y,
  data_t x
  )
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};


	// Write your code here
	static data_t reg_x[N];
	acc_t ACC = 0;

	TDL:
	for (int i = N-1; i >= 0; i--)
	{
		if(i==0)
			reg_x[0] = x;
		else
			reg_x[i] = reg_x[i-1];
	}


	MAC:
	for (int i = N-1; i >= 0; i--)
	{
		ACC += c[i]*reg_x[i];
	}

	*y = ACC;	
}


