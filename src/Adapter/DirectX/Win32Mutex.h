/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WIN32MUTEX_H
#define __GLAZIERY_WIN32MUTEX_H


namespace glaziery
{
	class Win32Mutex : public Mutex
	{
		friend DirectXPlatformAdapter;

		private:

			HANDLE handle;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Win32Mutex);
			#endif

		public:

			Win32Mutex();
			virtual ~Win32Mutex();
			virtual bool lock(long timeout);
			virtual void release();
	};
}


#endif
