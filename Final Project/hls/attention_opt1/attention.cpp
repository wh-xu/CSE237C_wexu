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
	data_fixed buf_S[SEQ_LENGTH * SEQ_LENGTH];

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
//				printf("i=%d, j=%d, k=%d,\t %f x %f = %f, psum=%f\n", i, j, k, input_seq[i * D_MODEL + k], weight_q[k * D_MODEL + j], input_seq[i * D_MODEL + k]*weight_q[k * D_MODEL + j], psum);
			}
			buf_Q[i * D_MODEL + j] = psum;
//			printf("%f\t", float(buf_Q[i * D_MODEL + j]));
		}
//		printf("\n");
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

CALC_S:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < SEQ_LENGTH; j++)
		{
			data_fixed psum = 0.0;
			for (k = 0; k < D_MODEL; k++)
			{

				psum += buf_Q[i * D_MODEL + k] * buf_K[j * D_MODEL + k];
			}
			buf_S[i * SEQ_LENGTH + j] = psum * scaling;
//			printf("%f\t", float(buf_S[i * SEQ_LENGTH + j]));
		}
//		printf("\n");
	}

CALC_SOFTMAX:
	data_sum_fixed row_sum[SEQ_LENGTH];

//	float test_row_sum;
//	float test_exp;

	for (i = 0; i < SEQ_LENGTH; i++)
	{
		data_fixed temp_exp;
		row_sum[i] = 0.0;

//		test_row_sum = 0.0;

		for (j = 0; j < SEQ_LENGTH; j++)
		{
			// temp_exp = expf(float(buf_S[i*SEQ_LENGTH+j]));
//			test_exp = expf(float(buf_S[i*SEQ_LENGTH+j]));
//			printf("%f\t", float(test_exp));
			CORDIC_exp(&buf_S[i * SEQ_LENGTH + j], &temp_exp);
			buf_S[i * SEQ_LENGTH + j] = temp_exp;
			row_sum[i] += temp_exp;

//			test_row_sum += test_exp;
		}
//		printf("\n");
//		printf("%f\n", float(row_sum[i]));
//		printf("%f\n", test_row_sum);
	}

	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < SEQ_LENGTH; j++)
		{
			buf_S[i * SEQ_LENGTH + j] = buf_S[i * SEQ_LENGTH + j] / row_sum[i];
//			printf("%f\t", float(buf_S[i * SEQ_LENGTH + j]));
		}
//		printf("\n");
	}


CALC_OUTPUT:
	for (i = 0; i < SEQ_LENGTH; i++)
	{
		for (j = 0; j < D_MODEL; j++)
		{
			data_fixed psum = 0.0;
			for (k = 0; k < SEQ_LENGTH; k++)
			{
				psum += buf_S[i * SEQ_LENGTH + k] * buf_V[k * D_MODEL + j];
			}
			output[i * D_MODEL + j] = psum;

//			printf("%f\t", output[i * D_MODEL + j]);
		}
//		printf("\n");
	}
}

void CORDIC_exp(data_fixed *x, data_fixed *exp_x)
{
	data_fixed temp=*x;
	*exp_x = data_fixed(1.0) + temp;

	temp *= *x;
	*exp_x += temp<<1;

	// temp *= *x;
	// *exp_x += temp*data_fixed(0.1666666666);

	// temp *= *x;
	// *exp_x += temp*data_fixed(0.0416666666);

	// temp *= *x;
	// *exp_x += temp*data_fixed(0.0083333333);


//	DTYPE temp_sqrt = *x;
//	DTYPE temp_fact = 1.0;
//
//	for (unsigned i = 1; i < 2; i++)
//	{
//		*exp_x += temp_sqrt/temp_fact
//	}

//	for (unsigned i = 0; i < CORDIC_size; i++)
//	{
//		if (CORDIC_value[i] < *x)
//		{
//			*x -= CORDIC_value[i];
//			// *exp_x += (*exp_x) >> CORDIC_shift[i];
//			*exp_x += (*exp_x) * (2 >> CORDIC_shift[i]);
//		}
//	}
}
