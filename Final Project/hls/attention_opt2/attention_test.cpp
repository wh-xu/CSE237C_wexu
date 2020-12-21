/*
This is DFT computation using matrix vector multiplication form.
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "attention.h"

struct Rmpe
{
	int num;
	float sum;
	float error;

	Rmpe()
	{
		num = 0;
		sum = 0;
		error = 0;
	}

	float add_value(float truth, float d_n)
	{
		num++;
		sum += abs(d_n / truth);
		error = sqrtf(sum / num);
		return error;
	}
};

void read_mat(FILE *fp, DTYPE *in_mat, int row, int col)
{
	float temp;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			fscanf(fp, "%f ", &temp);
			in_mat[i*col+j] = temp;

//			printf("%f\t", in_mat[i*col+j]);
		}
//		printf("\n");
	}
}

Rmpe rmpe;

DTYPE *input_seq = (DTYPE*)(malloc(sizeof(DTYPE)*SEQ_LENGTH*D_MODEL));
DTYPE *weight_v = (DTYPE*)(malloc(sizeof(DTYPE)*D_MODEL*D_MODEL));
DTYPE *weight_q = (DTYPE*)(malloc(sizeof(DTYPE)*D_MODEL*D_MODEL));
DTYPE *weight_k = (DTYPE*)(malloc(sizeof(DTYPE)*D_MODEL*D_MODEL));
DTYPE *output = (DTYPE*)(malloc(sizeof(DTYPE)*SEQ_LENGTH*D_MODEL));


int main()
{
	int index;
	FILE *fp_input_seq = fopen("input.input.dat", "r");
	FILE *fp_weight_v = fopen("input.weight_V.dat", "r");
	FILE *fp_weight_q = fopen("input.weight_Q.dat", "r");
	FILE *fp_weight_k = fopen("input.weight_K.dat", "r");

	// getting input data
	if(fp_input_seq==NULL || fp_weight_v==NULL || fp_weight_q==NULL || fp_weight_k==NULL)
	{
		printf("Failed file opening!\n");
		exit(0);
	}

	read_mat(fp_input_seq, input_seq, SEQ_LENGTH, D_MODEL);
	read_mat(fp_weight_v, weight_v, D_MODEL, D_MODEL);
	read_mat(fp_weight_q, weight_q, D_MODEL, D_MODEL);
	read_mat(fp_weight_k, weight_k, D_MODEL, D_MODEL);

	fclose(fp_input_seq);
	fclose(fp_weight_v);
	fclose(fp_weight_q);
	fclose(fp_weight_k);

	// Attention
	attention(input_seq, weight_v, weight_q, weight_k, output);

	// comparing with golden output
	float gold_out;
	FILE *fp_output = fopen("output.output.dat", "r");

	for (int i = 0; i < SEQ_LENGTH; i++)
	{
		for (int j = 0; j < D_MODEL; j++)
		{
			fscanf(fp_output, "%f", &gold_out);
//			printf("%f\t", output[i*D_MODEL+j]);
			rmpe.add_value(gold_out, (float)output[i*D_MODEL+j] - gold_out);
		}
//		printf("\n");
	}
	fclose(fp_output);

	// printing error results
	printf("----------------------------------------------\n");
	printf("RMPE\n");
	printf("%0.15f\n", rmpe.error);
	printf("----------------------------------------------\n");

	if (rmpe.error > 0.03)
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
		return 1;
	}
	else
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}
}
