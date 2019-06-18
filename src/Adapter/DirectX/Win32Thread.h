/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WIN32THREAD_H
#define __GLAZIERY_WIN32THREAD_H


namespace glaziery
{
	class Win32Thread : public Thread
	{
		friend DirectXPlatformAdapter;

		private:

			HANDLE handle;
			bool suspended;

			void setExitCodeByThreadFunction(int exitCode);
			void setHandle(HANDLE handle);
			static DWORD WINAPI threadFunction(LPVOID parameter);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Win32Thread);
			#endif

		public:

			Win32Thread(int (*function)(Thread & thread, void * parameter), void * parameter = NULL);
			~Win32Thread();
			virtual bool isFinished();
			virtual bool isSuspended();
			virtual void kill();
			virtual void resume();
			virtual void suspend();
	};
}


#endif
