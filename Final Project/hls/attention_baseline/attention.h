
typedef float DTYPE;
#define SEQ_LENGTH 512 /* sequence length */
#define D_MODEL 16

void attention(
    DTYPE *input_seq,
    DTYPE *weight_v,
    DTYPE *weight_q,
    DTYPE *weight_k,
    DTYPE *output
	);

