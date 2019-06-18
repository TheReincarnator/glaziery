/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_PROGRESSDIALOG_H
#define __GLAZIERY_PROGRESSDIALOG_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class ProgressDialog : public Dialog
	{
		friend Desktop;

		protected:

			/**
			 * TODO
			 */
			bool autoEndingOnCompletion;

			/**
			 * TODO
			 */
			bool canceled;

			/**
			 * TODO
			 */
			bool finished;

			/**
			 * TODO
			 */
			int (*function)(ProgressDialog & dialog, void * parameter);

			/**
			 * TODO
			 */
			bool functionCalled;

			/**
			 * TODO
			 */
			void * functionParameter;

			/**
			 * TODO
			 */
			int functionResult;

			/**
			 * TODO
			 */
			Label * itemLabel;

			/**
			 * TODO
			 */
			bool mayCancel;

			/**
			 * TODO
			 */
			ProgressBar * progressBar;

			/**
			 * TODO
			 */
			Label * summaryLabel;

			/**
			 * TODO
			 */
			Thread * thread;

			/**
			 * TODO
			 */
			ProgressDialog(Thread * thread);

			/**
			 * TODO
			 */
			ProgressDialog(int (*function)(ProgressDialog & dialog, void * parameter), void * parameter);

			/**
			 * Performs operations when the dialog has been dismissed.
			 * @param The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
			 * Vetoing this method will not close the dialog.
			 * @see VetoMode
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual VetoMode onDialogEnd(Buttons button);

			/**
			 * Performs final operations before the dialog start, e.g. fills or adjusts fields
			 * according to dialog property set after setupContent().
			 * @note Implement this method in derivatives, if necessary.
			 * @see setupContent()
			 */
			virtual void onDialogStart();

			/**
			 * Performs operations for each frame while the window is running modally.
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual void onModalUpdate();

			/**
			 * Helper to access the setup method in derivatives.
			 * Initializes the dialog (creates fields etc.).
			 * In static creator and/or show dialog methods, call this method after having created
			 * and added the dialog to the desktop.
			 * @param dialog The dialog to be initialized.
			 * @param title The window title bar string.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @note In this method, you should not rely on dialog properties that tune the dialog.
			 * Perform such steps in onDialogStart() instead.
			 * @see setupContent()
			 * @see onDialogStart()
			 * @see showModally()
			 */
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup);

		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ProgressDialog);
			#endif

			/**
			 * TODO
			 */
			void cancel();

			/**
			 * TODO
			 * @warn Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use Desktop::deferOperation() from any concurrent thread.
			 * Some classes also provide convenience methods, usually suffixed by 'Deferred'.
			 * Also note that the thread's methods are safe to use from both the main and
			 * the concurrent thread.
			 */
			static ProgressDialog * createDialog(Thread * thread);

			/**
			 * TODO
			 */
			static ProgressDialog * createDialog(int (*function)(ProgressDialog & dialog, void * parameter),
				void * parameter);

			/**
			 * TODO
			 */
			Label * getItemLabel();

			/**
			 * TODO
			 */
			ProgressBar * getProgressBar();

			/**
			 * TODO
			 */
			int getResult();

			/**
			 * TODO
			 */
			Label * getSummaryLabel();

			/**
			 * TODO
			 */
			bool isAutoEndingOnCompletion();

			/**
			 * TODO
			 */
			bool isCanceled();

			/**
			 * TODO
			 */
			bool isMayCancel();

			/**
			 * TODO
			 */
			void setAutoEndingOnCompletion(bool autoEndingOnCompletion);

			/**
			 * TODO
			 */
			void setMayCancel(bool mayCancel);
	};
}


#endif
