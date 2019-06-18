/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MUTEX_H
#define __GLAZIERY_MUTEX_H


namespace glaziery
{
	/**
	 * TODO
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Mutex : public CObject
	#else
		class Mutex
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Mutex);
			#endif

		public:

			/**
			 * TODO
			 */
			static const long NO_TIMEOUT;

			/**
			 * TODO
			 */
			virtual ~Mutex();

			/**
			 * TODO
			 */
			virtual bool lock(long timeout = NO_TIMEOUT) = NULL;

			/**
			 * TODO
			 */
			virtual void release() = NULL;
	};
}


#endif
