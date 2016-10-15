#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"
#include "fft.h"
#include "countSound.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int n, m, k, J, L, N, offset, frame, number_of_frame,countsound;
	double fe, delta, threshold, *b, *w, *b_real, *b_imag, *x_real, *x_imag, *y_real, *y_imag;

	mono_wave_read(&pcm0, "005_160615_0941V0.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */

	pcm1.fs = pcm0.fs; /* �W�{�����g�� */
	pcm1.bits = pcm0.bits; /* �ʎq�����x */
	pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double)); /* �������̊m�� */

	fe = 600.0 / pcm0.fs; /* �G�b�W���g�� */
	delta = 500.0 / pcm0.fs; /* �J�ڑш敝 */

	J = (int)(3.1 / delta + 0.5) - 1; /* �x����̐� */
	if (J % 2 == 1)
	{
		J++; /* J+1����ɂȂ�悤�ɒ������� */
	}

	b = (double*) calloc((J + 1), sizeof(double)); /* �������̊m�� */
	w = (double*)calloc((J + 1), sizeof(double)); /* �������̊m�� */

	Hanning_window(w, (J + 1)); /* �n�j���O�� */

	FIR_LPF(fe, J, b, w); /* FIR�t�B���^�̐݌v */

	L = 256; /* �t���[���̒��� */
	N = 512; /* DFT�̃T�C�Y */

	countsound = 0; /* ����臒l�𒴂�����*/
	threshold = 100.0;/* 臒l*/

	number_of_frame = pcm0.length / L; /* �t���[���̐� */

	b_real = (double*)calloc(N, sizeof(double)); /* �������̊m�� */
	b_imag = (double*)calloc(N, sizeof(double)); /* �������̊m�� */
	x_real = (double*)calloc(N, sizeof(double)); /* �������̊m�� */
	x_imag = (double*)calloc(N, sizeof(double)); /* �������̊m�� */
	y_real = (double*)calloc(N, sizeof(double)); /* �������̊m�� */
	y_imag = (double*)calloc(N, sizeof(double)); /* �������̊m�� */

	for (frame = 0; frame < number_of_frame; frame++)
	{
		offset = L * frame;

		/* x(n)��FFT */
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

		/* b(m)��FFT */
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

		/* �t�B���^�����O */
		for (k = 0; k < N; k++)
		{
			y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
			y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
		}

		countsound += judgeSounnd(y_real,N,threshold);
		
		IFFT(y_real, y_imag, N);

		/* �t�B���^�����O���ʂ̘A�� */
		for (n = 0; n < L * 2; n++)
		{
			if (offset + n < pcm1.length)
			{
				pcm1.s[offset + n] += y_real[n];
			}
		}
	}

	mono_wave_write(&pcm1, "ex6_4.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */

	free(pcm0.s); /* �������̉�� */
	free(pcm1.s); /* �������̉�� */
	free(b); /* �������̉�� */
	free(w); /* �������̉�� */
	free(b_real); /* �������̉�� */
	free(b_imag); /* �������̉�� */
	free(x_real); /* �������̉�� */
	free(x_imag); /* �������̉�� */
	free(y_real); /* �������̉�� */
	free(y_imag); /* �������̉�� */

	printf("countsound =%d\n",countsound);
	printf("finish");

	return 0;
}
