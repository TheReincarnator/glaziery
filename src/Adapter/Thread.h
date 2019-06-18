/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_THREAD_H
#define __GLAZIERY_THREAD_H


namespace glaziery
{
	/**
	 * TODO
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Thread : public CObject
	#else
		class Thread
	#endif
	{
		private:

			/**
			 * TODO
			 */
			bool canceled;

			/**
			 * TODO
			 */
			int exitCode;

			/**
			 * TODO
			 */
			int (*function)(Thread & thread, void * parameter);

			/**
			 * TODO
			 */
			void * parameter;

			/**
			 * TODO
			 */
			ProgressDialog * progressDialog;

		protected:

			/**
			 * @param function The function or static method to create the thread for.
			 * @param parameter The parameter to be passed to the function. May be NULL.
			 */
			Thread(int (*function)(Thread & thread, void * parameter), void * parameter = NULL);

			/**
			 * TODO
			 */
			void setExitCode(int exitCode);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Thread);
			#endif

		public:

			/**
			 * TODO
			 */
			Thread(void * parameter = NULL);

			/**
			 * TODO
			 */
			virtual ~Thread();

			/**
			 * TODO
			 */
			void cancel();

			/**
			 * TODO
			 */
			int getExitCode();

			/**
			 * TODO
			 */
			int (*getFunction())(Thread & thread, void * parameter);

			/**
			 * TODO
			 */
			void * getParameter();

			/**
			 * TODO
			 */
			ProgressDialog * getProgressDialog();

			/**
			 * TODO
			 */
			bool isCanceled();

			/**
			 * TODO
			 */
			virtual bool isFinished() = NULL;

			/**
			 * TODO
			 */
			virtual bool isSuspended() = NULL;

			/**
			 * TODO
			 */
			virtual void kill() = NULL;

			/**
			 * TODO
			 */
			virtual void resume() = NULL;

			/**
			 * @warn Thread::runAndWait() may call the code again from where it has been
			 * called, since regular event queue handling takes place.
			 * Beware of this fact when using this method. Maybe you require
			 * guard flags to prevent duplicate (inner) execution of your code.
			 */
			int runAndWait(bool deleteThread = true, bool hourGlass = true);

			/**
			 * TODO
			 */
			void setProgressDialog(ProgressDialog * progressDialog);

			/**
			 * TODO
			 */
			virtual void suspend() = NULL;
	};
}


#endif
