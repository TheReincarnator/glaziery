/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PolynomialTimeCurve::PolynomialTimeCurve(double raw1, double transformed1, double raw2, double transformed2)
{
	ASSERTION_COBJECT(this);

	x[0] = 0.0;
	x[1] = raw1;
	x[2] = raw2;
	x[3] = 1.0;

	y[0] = 0.0;
	y[1] = transformed1;
	y[2] = transformed2;
	y[3] = 1.0;

	ux[0] = 0.0;
	ux[1] = transformed1;
	ux[2] = transformed2;
	ux[3] = 1.0;

	uy[0] = 0.0;
	uy[1] = raw1;
	uy[2] = raw2;
	uy[3] = 1.0;

	for (int i=0; i<4; i++)
		for (int j=3; j>i; j--)
		{
			y[j] = (y[j] - y[j-1]) / (x[j] - x[j-i-1]);
			uy[j] = (uy[j] - uy[j-1]) / (ux[j] - ux[j-i-1]);
		}
}

double PolynomialTimeCurve::transform(double raw)
{
	ASSERTION_COBJECT(this);

	double transformed = 0.0;
	for (int i=3; i>=0; i--)
		transformed = transformed * (raw - x[i]) + y[i];

	return transformed;
}

double PolynomialTimeCurve::untransform(double transformed)
{
	ASSERTION_COBJECT(this);

	double raw = 0.0;
	for (int i=3; i>=0; i--)
		raw = raw * (transformed - ux[i]) + uy[i];

	return raw;
}
