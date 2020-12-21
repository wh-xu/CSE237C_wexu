#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "attention.h"

const DTYPE scaling = 1 / sqrtf(D_MODEL);

void attention(
	DTYPE *input_seq,
	DTYPE *weight_v,
	DTYPE *weight_q,
	DTYPE *weight_k,
	DTYPE *output)
{
	// Write your code here
	#pragma HLS INTERFACE axis depth=64 port=input_seq
	#pragma HLS INTERFACE axis depth=64 port=weight_v
	#pragma HLS INTERFACE axis depth=64 port=weight_q
	#pragma HLS INTERFACE axis depth=64 port=weight_k
	#pragma HLS INTERFACE axis depth=64 port=output

	int i, j, k;


	DTYPE buf_V[SEQ_LENGTH*D_MODEL];
	DTYPE buf_Q[SEQ_LENGTH*D_MODEL];
	DTYPE buf_K[SEQ_LENGTH*D_MODEL];
	DTYPE buf_S[SEQ_LENGTH*SEQ_LENGTH];


	CALC_Q:
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			for (j = 0; j < D_MODEL; j++)
			{
				DTYPE psum = 0.0;
				for (k = 0; k < D_MODEL; k++)
				{
					psum += input_seq[i * D_MODEL + k] * weight_q[k * D_MODEL + j];
	//				printf("i=%d, j=%d, k=%d,\t %f x %f = %f, psum=%f\n", i, j, k, input_seq[i * D_MODEL + k], weight_q[k * D_MODEL + j], input_seq[i * D_MODEL + k]*weight_q[k * D_MODEL + j], psum);
				}
				buf_Q[i * D_MODEL + j] = psum;
	//			printf("%f\t", buf_Q[i * D_MODEL + j]);
			}
	//		printf("\n");
		}

	CALC_K:
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			for (j = 0; j < D_MODEL; j++)
			{
				DTYPE psum = 0.0;
				for (k = 0; k < D_MODEL; k++)
				{
					psum += input_seq[i * D_MODEL + k] * weight_k[k * D_MODEL + j];
				}
				buf_K[i * D_MODEL + j] = psum;
	//			printf("%f\t", buf_K[i * D_MODEL + j]);
			}
	//		printf("\n");
		}

	CALC_V:
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			for (j = 0; j < D_MODEL; j++)
			{
				DTYPE psum = 0.0;
				for (k = 0; k < D_MODEL; k++)
				{
					psum += input_seq[i * D_MODEL + k] * weight_v[k * D_MODEL + j];
				}
				buf_V[i * D_MODEL + j] = psum;
	//			printf("%f\t", buf_V[i * D_MODEL + j]);
			}
	//		printf("\n");
		}

	CALC_S:
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			for (j = 0; j < SEQ_LENGTH; j++)
			{
				DTYPE psum = 0.0;
				for (k = 0; k < D_MODEL; k++)
				{
					psum += buf_Q[i * D_MODEL + k] * buf_K[j * D_MODEL + k];
				}
				buf_S[i * SEQ_LENGTH + j] = psum * scaling;
	//			printf("%f\t", buf_S[i * SEQ_LENGTH + j]);
			}
	//		printf("\n");
		}

	CALC_SOFTMAX:
		DTYPE row_sum[SEQ_LENGTH];
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			DTYPE temp_exp;
			row_sum[i] = 0.0;
			for (j = 0; j < SEQ_LENGTH; j++)
			{
				temp_exp = expf(buf_S[i*SEQ_LENGTH+j]);
				buf_S[i * SEQ_LENGTH + j] = temp_exp;
				row_sum[i] += temp_exp;
			}
		}

		for (i = 0; i < SEQ_LENGTH; i++)
			for (j = 0; j < SEQ_LENGTH; j++)
				buf_S[i * SEQ_LENGTH + j] = buf_S[i * SEQ_LENGTH + j] / row_sum[i];


	CALC_OUTPUT:
		for (i = 0; i < SEQ_LENGTH; i++)
		{
			for (j = 0; j < D_MODEL; j++)
			{
				DTYPE psum = 0.0;
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
