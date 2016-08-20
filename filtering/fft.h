int log2(int x)/* y = log2(x) */
{
	int y;

	y = 0;
	while (x > 1)
	{
		x >>= 1;
		y++;
	}

	return y;
}

int pow2(int x) /* y = 2 ^ x */
{
	int y;

	if (x == 0)
	{
		y = 1;
	}
	else
	{
		y = 2 << (x - 1);
	}

	return y;
}

void FFT(double x_real[], double x_imag[], int N)
{
	int i, j, k, n, m, r, stage, number_of_stage, *index;
	double a_real, a_imag, b_real, b_imag, c_real, c_imag, real, imag;

	/* FFT�̒i�� */
	number_of_stage = log2(N);

	/* �o�^�t���C�v�Z */
	for (stage = 1; stage <= number_of_stage; stage++)
	{
		for (i = 0; i < pow2(stage - 1); i++)
		{
			for (j = 0; j < pow2(number_of_stage - stage); j++)
			{
				n = pow2(number_of_stage - stage + 1) * i + j;
				m = pow2(number_of_stage - stage) + n;
				r = pow2(stage - 1) * j;
				a_real = x_real[n];
				a_imag = x_imag[n];
				b_real = x_real[m];
				b_imag = x_imag[m];
				c_real = cos((2.0 * M_PI * r) / N);
				c_imag = -sin((2.0 * M_PI * r) / N);
				if (stage < number_of_stage)
				{
					x_real[n] = a_real + b_real;
					x_imag[n] = a_imag + b_imag;
					x_real[m] = (a_real - b_real) * c_real - (a_imag - b_imag) * c_imag;
					x_imag[m] = (a_imag - b_imag) * c_real + (a_real - b_real) * c_imag;
				}
				else
				{
					x_real[n] = a_real + b_real;
					x_imag[n] = a_imag + b_imag;
					x_real[m] = a_real - b_real;
					x_imag[m] = a_imag - b_imag;
				}
			}
		}
	}

	/* �C���f�b�N�X�̕��ёւ��̂��߂̃e�[�u���̍쐬 */
	index =(int*) calloc(N, sizeof(int));
	for (stage = 1; stage <= number_of_stage; stage++)
	{
		for (i = 0; i < pow2(stage - 1); i++)
		{
			index[pow2(stage - 1) + i] = index[i] + pow2(number_of_stage - stage);
		}
	}

	/* �C���f�b�N�X�̕��ёւ� */
	for (k = 0; k < N; k++)
	{
		if (index[k] > k)
		{
			real = x_real[index[k]];
			imag = x_imag[index[k]];
			x_real[index[k]] = x_real[k];
			x_imag[index[k]] = x_imag[k];
			x_real[k] = real;
			x_imag[k] = imag;
		}
	}

	free(index);
}

void IFFT(double x_real[], double x_imag[], int N)
{
	int i, j, k, n, m, r, stage, number_of_stage, *index;
	double a_real, a_imag, b_real, b_imag, c_real, c_imag, real, imag;

	/* IFFT�̒i�� */
	number_of_stage = log2(N);

	/* �o�^�t���C�v�Z */
	for (stage = 1; stage <= number_of_stage; stage++)
	{
		for (i = 0; i < pow2(stage - 1); i++)
		{
			for (j = 0; j < pow2(number_of_stage - stage); j++)
			{
				n = pow2(number_of_stage - stage + 1) * i + j;
				m = pow2(number_of_stage - stage) + n;
				r = pow2(stage - 1) * j;
				a_real = x_real[n];
				a_imag = x_imag[n];
				b_real = x_real[m];
				b_imag = x_imag[m];
				c_real = cos((2.0 * M_PI * r) / N);
				c_imag = sin((2.0 * M_PI * r) / N);
				if (stage < number_of_stage)
				{
					x_real[n] = a_real + b_real;
					x_imag[n] = a_imag + b_imag;
					x_real[m] = (a_real - b_real) * c_real - (a_imag - b_imag) * c_imag;
					x_imag[m] = (a_imag - b_imag) * c_real + (a_real - b_real) * c_imag;
				}
				else
				{
					x_real[n] = a_real + b_real;
					x_imag[n] = a_imag + b_imag;
					x_real[m] = a_real - b_real;
					x_imag[m] = a_imag - b_imag;
				}
			}
		}
	}

	/* �C���f�b�N�X�̕��ёւ��̂��߂̃e�[�u���̍쐬 */
	index =(int*) calloc(N, sizeof(int));
	for (stage = 1; stage <= number_of_stage; stage++)
	{
		for (i = 0; i < pow2(stage - 1); i++)
		{
			index[pow2(stage - 1) + i] = index[i] + pow2(number_of_stage - stage);
		}
	}

	/* �C���f�b�N�X�̕��ёւ� */
	for (k = 0; k < N; k++)
	{
		if (index[k] > k)
		{
			real = x_real[index[k]];
			imag = x_imag[index[k]];
			x_real[index[k]] = x_real[k];
			x_imag[index[k]] = x_imag[k];
			x_real[k] = real;
			x_imag[k] = imag;
		}
	}

	/* �v�Z���ʂ�N�Ŋ��� */
	for (k = 0; k < N; k++)
	{
		x_real[k] /= N;
		x_imag[k] /= N;
	}

	free(index);
}
