/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WIN32CRASHDIALOG_H
#define __GLAZIERY_WIN32CRASHDIALOG_H


namespace glaziery
{
	/**
	 * The crash dialog shown if a Glaziery application fails.
	 * @author Thomas Jacob
	 */
	class Win32CrashDialog : public CDialog
	{
		private:

			const char * applicationName;
			const char * applicationVersion;
			CxImage balloonImage;
			bool restart;
			const char * exceptionClassName;
			long exceptionLineNumber;
			const char * exceptionMessage;
			const char * exceptionSource;
			long * lastTraceLineNumbers;
			char ** lastTraceSources;

		// Declaration macros require their own protected section.
		protected:

			/**
			 * Message map declaration.
			 */
			DECLARE_MESSAGE_MAP()

		protected:

			void OnDestroy();

			void OnDrawItem(int controlId, LPDRAWITEMSTRUCT drawItemInfo);

			/**
			 * Initializes the dialog after all controls have been placed,
			 * but before the dialog is displayed.
			 * @return 0 if the method has set the focus on a control,
			 * 1 if the framework should do that.
			 */
			virtual int OnInitDialog();

			void OnDoNotSendClicked();

			void OnSendClicked();

		public:

			/**
			 * Creates a new crash dialog.
			 */
			Win32CrashDialog(const char * applicationName, const char * applicationVersion,
				const char * exceptionClassName, const char * exceptionMessage,
				const char * exceptionSource, long exceptionLineNumber,
				char ** lastTraceSources, long * lastTraceLineNumbers);
			bool IsRestart();
	};
}


#endif
