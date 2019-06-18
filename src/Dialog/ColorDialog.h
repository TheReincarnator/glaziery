/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_COLORDIALOG_H
#define __GLAZIERY_COLORDIALOG_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class ColorDialog : public Dialog
	{
		protected:

			/**
			 * Creates a new message window.
			 */
			ColorDialog();

			/**
			 * Destroys the message dialog.
			 */
			virtual ~ColorDialog();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ColorDialog);
			#endif

		public:
	};
}


#endif
