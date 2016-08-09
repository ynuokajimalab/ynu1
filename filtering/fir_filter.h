FIR_LFF(double fe, int J, double b[], double w[])
{
	int m;
	int offset;
	offset = J / 2;
	double M_PI;
	for (m = -J / 2; m <= J / 2; m++)
	{
		b[offset + m] = 2.0*fe*sinc(2.0*M_PI*fe*m);
	}

	for (m = 0; m < J + 1; m++)
	{
		b[m] *= w[m];
	}
}