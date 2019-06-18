/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


double LinearTimeCurve::transform(double raw)
{
	ASSERTION_COBJECT(this);
	return raw;
}

double LinearTimeCurve::untransform(double transformed)
{
	ASSERTION_COBJECT(this);
	return transformed;
}
