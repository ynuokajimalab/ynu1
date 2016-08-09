#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"
#include "fft.h"

/* TFFTä÷êîÇÃÉTÉìÉvÉãÇíTÇπ/**/

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

	fe = 1000.0 / pcm0.fs;
	delta = 1000.0 / pcm0.fs;

	J = (int)(3.1 / delta + 0.5) - 1;
	if (J % 2 == 1)
	{
		J++;
	}

	b = calloc((J + 1), sizeof(double));
	w = calloc((J + 1), sizeof(double));

	Hanning_window(w, J+1);

	FIR_LFF(fe, J, b, w);

	L = 256;
	N = 512;

	number_of_frame = pcm0.length;

	b_real = calloc(N, sizeof(double));
	b_imag = calloc(N, sizeof(double));
	x_real = calloc(N, sizeof(double));
	x_imag = calloc(N, sizeof(double));
	y_real = calloc(N, sizeof(double));
	y_imag = calloc(N, sizeof(double));

	for (frame = 0; frame < number_of_frame; frame++)
	{
		offset = L*frame;

		for (n = 0; n < N; n++)
		{
			x_real[n] = 0.0;
			x_imag[n] = 0.0;
		}

		for (n = 0; n < L; n++)
		{
			x_real[n] = pcm0.s[offset + n];
		}
		FFT(x_real, x_imag, N);

		for (m = 0; m < N; m++)
		{
			b_real[m] = 0.0;
			b_imag[m] = 0.0;
		}

		for (m = 0; m <= J; m++)
		{
			b_real[m] = b[m];
		}
		FFT(b_real, b_imag, N);

		for (k = 0; k < N; k++)
		{
			y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
			y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
		}
		IFFT(y_real, y_imag, N);

		for (n = 0; n < L * 2; n++)
		{
			if (offset + n < pcm1.length)
			{
				pcm1.s[offset + n] += y_real[n];
			}
		}
	}

	mono_wave_write(&pcm1, "006_160615_0941V0new");

	free(pcm0.s);
	free(pcm1.s);
	free(b);
	free(w);
	free(b_real);
	free(b_imag);
	free(x_real);
	free(x_imag);
	free(y_real);
	free(y_imag);

	return 0;
}