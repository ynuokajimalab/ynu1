#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"
#include "fft.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int n, m, k, J, L, N, offset, frame, number_of_frame;
	double fe, delta, *b, *w, *b_real, *b_imag, *x_real, *x_imag, *y_real, *y_imag;
	mono_wave_read(&pcm0, "005_160615_0941V0");

	pcm1.fs = pcm0.fs;
	pcm1.bits = pcm0.bits;
	pcm1.length = pcm0.length;
	pcm1.s = calloc(pcm1.length, sizeof(double));

	

	mono_wave_write(&pcm1, "006_160615_0941V0new");

	free(pcm0.s);
	free(pcm1.s);
	//free(b);
	//free(w);
	//free(b_real);
	//free(b_imag);
	//free(x_real);
	//free(x_imag);
	//free(y_real);
	//free(y_imag);

	return 0;
}