/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


double AccelerationTimeCurve::transform(double raw)
{
	ASSERTION_COBJECT(this);

	if (raw <= 0.5)
		return 2.0 * raw * raw;
	else
	{
		raw = 1.0 - raw;
		return 1.0 - 2.0 * raw * raw;
	}
}

double AccelerationTimeCurve::untransform(double transformed)
{
	ASSERTION_COBJECT(this);

	if (transformed <= 0.0)
		return 0.0;
	else if (transformed <= 0.5)
		return sqrt(transformed / 2.0);
	else
	{
		transformed = 1.0 - transformed;
		return 1.0 - sqrt(transformed / 2.0);
	}
}
