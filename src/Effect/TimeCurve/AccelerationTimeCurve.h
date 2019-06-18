/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_ACCELERATIONTIMECURVE_H
#define __GLAZIERY_ACCELERATIONTIMECURVE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class AccelerationTimeCurve : public TimeCurve
	{
		public:

			virtual double transform(double raw);
			virtual double untransform(double transformed);
	};
}


#endif
