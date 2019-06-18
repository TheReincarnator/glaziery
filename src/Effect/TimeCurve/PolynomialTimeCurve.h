/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POLYNOMIALTIMECURVE_H
#define __GLAZIERY_POLYNOMIALTIMECURVE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class PolynomialTimeCurve : public TimeCurve
	{
		private:

			double x[4];
			double y[4];
			double ux[4];
			double uy[4];

		public:

			PolynomialTimeCurve(double raw1, double transformed1, double raw2, double transformed2);
			virtual double transform(double raw);
			virtual double untransform(double transformed);
	};
}


#endif
