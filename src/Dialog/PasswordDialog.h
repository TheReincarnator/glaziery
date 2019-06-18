/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_PASSWORDDIALOG_H
#define __GLAZIERY_PASSWORDDIALOG_H


namespace glaziery
{
	class PasswordDialog : public Dialog
	{
		protected:

			String message;
			TextField * password;

			PasswordDialog(const String & message);

			virtual VetoMode onDialogEnd(Buttons button);
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup);

		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PasswordDialog);
			#endif
			static String showDialog(const String & title, const String & message);
	};
}


#endif
