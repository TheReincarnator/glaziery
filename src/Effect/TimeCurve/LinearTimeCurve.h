/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LINEARTIMECURVE_H
#define __GLAZIERY_LINEARTIMECURVE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class LinearTimeCurve : public TimeCurve
	{
		public:

			virtual double transform(double raw);
			virtual double untransform(double transformed);
	};
}


#endif
