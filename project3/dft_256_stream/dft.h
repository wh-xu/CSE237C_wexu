#include "hls_stream.h"

typedef float DTYPE;
#define SIZE 256 		/* SIZE OF DFT */

void dft(hls::stream<DTYPE> &real_sample_stream, hls::stream<DTYPE> &out_real_stream, hls::stream<DTYPE> &out_imag_stream);
