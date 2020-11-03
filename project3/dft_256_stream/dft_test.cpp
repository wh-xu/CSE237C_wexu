/*
This is DFT computation using matrix vector multiplication form.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal in time domain.
OUTPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal in frequency domain.

*/

#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#include <math.h>
#include "dft.h"

struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}

};


Rmse rmse_R,  rmse_I;

DTYPE In_R[SIZE], In_I[SIZE];

int main()
{
	int index;
	DTYPE gold_R, gold_I;

	FILE * fp = fopen("out.gold.dat","r");

	hls::stream<DTYPE> in_real_sample_stream;
	// getting input data
	for(int i=0; i<SIZE; i++)
	{
		In_R[i] = i;
		In_I[i] = 0.0;

		// streaming input data for DFT
		in_real_sample_stream.write(In_R[i]);
	}


	hls::stream<DTYPE> out_real_sample_stream;
	hls::stream<DTYPE> out_imag_sample_stream;
	dft(in_real_sample_stream, out_real_sample_stream, out_imag_sample_stream);


	// comparing with golden output
	for(int i=0; i<SIZE; i++)
	{
		DTYPE out_R;
		out_real_sample_stream.read(out_R);

		DTYPE out_I;
		out_imag_sample_stream.read(out_I);

		fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
		printf("i=%d,\tdft_real=%f,\tdft_img=%f\n", i, out_R, out_I);

		rmse_R.add_value((float)out_R - gold_R);
		rmse_I.add_value((float)out_I - gold_I);
	}

	fclose(fp);


	// printing error results
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(I)\n");
	printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
	printf("----------------------------------------------\n");

	if (rmse_R.error > 0.1 || rmse_I.error > 0.1 ) {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
	    return 1;
	}else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
	    return 0;
	}

}
