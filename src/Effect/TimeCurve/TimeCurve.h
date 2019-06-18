/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TIMECURVE_H
#define __GLAZIERY_TIMECURVE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class TimeCurve : public CObject
	#else
		class TimeCurve
	#endif
	{
		public:

			virtual ~TimeCurve();

			virtual double transform(double raw) = NULL;
			virtual double untransform(double transformed) = NULL;
	};
}


#endif
