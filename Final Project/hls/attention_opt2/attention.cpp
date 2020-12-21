#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "attention.h"

const data_fixed scaling = 1 / sqrtf(D_MODEL);

void attention(
	DTYPE *input_seq,
	DTYPE *weight_v,
	DTYPE *weight_q,
	DTYPE *weight_k,
	DTYPE *output)
{
// Write your code here
#pragma HLS INTERFACE axis depth = 64 port = input_seq
#pragma HLS INTERFACE axis depth = 64 port = weight_v
#pragma HLS INTERFACE axis depth = 64 port = weight_q
#pragma HLS INTERFACE axis depth = 64 port = weight_k
#pragma HLS INTERFACE axis depth = 64 port = output

	int i, j, k;

	data_fixed buf_V[SEQ_LENGTH * D_MODEL];
	data_fixed buf_Q[SEQ_LENGTH * D_MODEL];
	data_fixed buf_K[SEQ_LENGTH * D_MODEL];
	//	data_fixed buf_S[SEQ_LENGTH * SEQ_LENGTH];
	data_fixed buf_S_row[SEQ_LENGTH];

	data_fixed output_psum[SEQ_LENGTH * D_MODEL];

	data_fixed temp_in;
	data_fixed temp_w;

CALC_Q:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < D_MODEL; j++)
		{
			data_fixed psum = 0.0;

			for (k = 0; k < D_MODEL; k++)
			{
				temp_in = input_seq[i * D_MODEL + k];
				temp_w = weight_q[k * D_MODEL + j];
				psum += temp_in * temp_w;
			}
			buf_Q[i * D_MODEL + j] = psum;
		}
	}

CALC_K:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < D_MODEL; j++)
		{
			data_fixed psum = 0.0;
			for (k = 0; k < D_MODEL; k++)
			{
				temp_in = input_seq[i * D_MODEL + k];
				temp_w = weight_k[k * D_MODEL + j];
				psum += temp_in * temp_w;
			}
			buf_K[i * D_MODEL + j] = psum;
			//			printf("%f\t", float(buf_K[i * D_MODEL + j]));
		}
		//		printf("\n");
	}

CALC_V:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < D_MODEL; j++)
		{
			data_fixed psum = 0.0;
			for (k = 0; k < D_MODEL; k++)
			{
				temp_in = input_seq[i * D_MODEL + k];
				temp_w = weight_v[k * D_MODEL + j];
				psum += temp_in * temp_w;
			}
			buf_V[i * D_MODEL + j] = psum;
			//			printf("%f\t", float(buf_V[i * D_MODEL + j]));
		}
		//		printf("\n");
	}


CALC_S_OUT:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		// Calculate one row of mat. S
		for (j = 0; j < SEQ_LENGTH; j++)
		{
			data_fixed psum = 0.0;
			for (k = 0; k < D_MODEL; k++)
			{

				psum += buf_Q[i * D_MODEL + k] * buf_K[j * D_MODEL + k];
			}
			data_fixed temp_exp;
			data_fixed temp_scaled_psum = psum * scaling;
			CORDIC_exp(&temp_scaled_psum, &temp_exp);
			buf_S_row[j] = temp_exp * data_fixed(0.001953125);
		}

		// Calculate partial sum of mat. output
		for (k = 0; k < SEQ_LENGTH; k++)
			for (j = 0; j < D_MODEL; j++)
				output_psum[i * D_MODEL + j] += buf_S_row[k] * buf_V[k * D_MODEL + j];
	}

OUTPUT_RESULT:
	for (i = 0; i < SEQ_LENGTH; i++)
		for (j = 0; j < D_MODEL; j++)
			output[i * D_MODEL + j] = output_psum[i * D_MODEL + j];
}

void CORDIC_exp(data_fixed *x, data_fixed *exp_x)
{
	data_fixed temp = *x;
	*exp_x = data_fixed(1.0) + temp;

	temp *= *x;
	*exp_x += temp << 1;
//
//	 temp *= *x;
//	 *exp_x += temp*data_fixed(0.1666666666);
//
//	 temp *= *x;
//	 *exp_x += temp*data_fixed(0.0416666666);

	// temp *= *x;
	// *exp_x += temp*data_fixed(0.0083333333);
}
