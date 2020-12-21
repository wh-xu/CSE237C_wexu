#include <ap_fixed.h>

#define W			21 // Total size of fixed-point representation. Must be W >= (I+MAN_BITS)
#define I			2  // Size of integer part of fixed-point (1 bit sign + I-1 bit integer)
#define MAN_BITS	19 // Size of fractional part. Vary this number to increase/decrease the resolution.

#define W_sum       14
#define I_sum       11
#define MAN_BITS_sum 3


typedef float DTYPE;
typedef ap_fixed<W, I, AP_RND, AP_WRAP, 1> data_fixed;
typedef ap_fixed<W_sum, I_sum, AP_RND, AP_WRAP, 1> data_sum_fixed;


#define SEQ_LENGTH 512 /* sequence length */
#define D_MODEL 16


const DTYPE Talor_factor[] = {1.0, 0.5, 0.16666};


//const DTYPE CORDIC_value[] ={0.2231, 0.1178, 0.0606, 0.0308, 0.0155, 0.0078};
//const int CORDIC_shift[] ={2, 3, 4, 5, 6, 7};
//const int CORDIC_size = 6;

void attention(
    DTYPE *input_seq,
    DTYPE *weight_v,
    DTYPE *weight_q,
    DTYPE *weight_k,
    DTYPE *output
	);

void CORDIC_exp(data_fixed *x, data_fixed *exp_x);

