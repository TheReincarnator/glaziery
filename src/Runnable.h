/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_RUNNABLE_H
#define __GLAZIERY_RUNNABLE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Runnable : public CObject
	#else
		class Runnable
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Runnable);
			#endif

		public:

			/**
			 * Performs the operation.
			 */
			virtual void run() = NULL;
	};
}


#endif
