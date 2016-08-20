#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int n, m, I, J, L, offset, frame, number_of_frame;
	double fc, Q, a[3], b[3], *x, *y;

	mono_wave_read(&pcm0, "sample04.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */

	pcm1.fs = pcm0.fs; /* �W�{�����g�� */
	pcm1.bits = pcm0.bits; /* �ʎq�����x */
	pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double)); /* �������̊m�� */

	fc = 1000.0 / pcm0.fs; /* �Ւf���g�� */
	Q = 1.0 / sqrt(2.0); /* �N�I���e�B�t�@�N�^ */
	I = 2; /* �x����̐� */
	J = 2; /* �x����̐� */

	IIR_LPF(fc, Q, a, b); /* IIR�t�B���^�̐݌v */

	L = 256; /* �t���[���̒��� */

	number_of_frame = pcm0.length / L; /* �t���[���̐� */

	x = (double*)calloc((L + J), sizeof(double)); /* �������̊m�� */
	y = (double*)calloc((L + I), sizeof(double)); /* �������̊m�� */

	for (frame = 0; frame < number_of_frame; frame++)
	{
		offset = L * frame;

		/* ���O�̃t���[���̌㔼��J�T���v������������ */
		for (n = 0; n < L + J; n++)
		{
			if (offset - J + n < 0)
			{
				x[n] = 0.0;
			}
			else
			{
				x[n] = pcm0.s[offset - J + n];
			}
		}

		/* ���O�̃t�B���^�����O���ʂ̌㔼��I�T���v������������ */
		for (n = 0; n < L + I; n++)
		{
			if (offset - I + n < 0)
			{
				y[n] = 0.0;
			}
			else
			{
				y[n] = pcm1.s[offset - I + n];
			}
		}

		/* �t�B���^�����O */
		for (n = 0; n < L; n++)
		{
			for (m = 0; m <= J; m++)
			{
				y[I + n] += b[m] * x[J + n - m];
			}
			for (m = 1; m <= I; m++)
			{
				y[I + n] += -a[m] * y[I + n - m];
			}
		}

		/* �t�B���^�����O���ʂ̘A�� */
		for (n = 0; n < L; n++)
		{
			pcm1.s[offset + n] = y[I + n];
		}
	}

	mono_wave_write(&pcm1, "ex6_4.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */

	free(pcm0.s); /* �������̉�� */
	free(pcm1.s); /* �������̉�� */

	return 0;
}
