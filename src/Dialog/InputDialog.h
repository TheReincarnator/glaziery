/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTDIALOG_H
#define __GLAZIERY_INPUTDIALOG_H


namespace glaziery
{
	/**
	 * The input dialog is a helper to easily create a dialog to enter a text.
	 * You can use the input dialog best by calling one of its static
	 * members.
	 */
	class InputDialog : public Dialog
	{
		protected:

			/**
			 * The string variable to be filled with the text input.
			 */
			String & input;

			/**
			 * The input field.
			 */
			TextField * inputField;

			/**
			 * The message to be displayed.
			 */
			String message;

			/**
			 * Creates a new input dialog.
			 * @param message The main message text.
			 * @param input The string variable to be filled with the text input.
			 */
			InputDialog(const String & message, String & input);

			/**
			 * Destroys the input dialog.
			 */
			virtual ~InputDialog();

			/**
			 * Performs operations when the dialog has been dismissed.
			 * @param The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the input dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
			 * Vetoing this method will not close the dialog.
			 * @see VetoMode
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual VetoMode onDialogEnd(Buttons button);

			/**
			 * Initializes the content group:
			 * @param area The fields area of this dialog.
			 * @param contentGroup The content group of this dialog.
			 */
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(InputDialog);
			#endif

		public:

			/**
			 * Creates a new input dialog.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param input The string variable to be filled with the text input.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * Defaults to OK and CANCEL.
			 * @return The new input dialog.
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static InputDialog * createDialog(const String & title, const String & message,
				String & input, Dialog::Buttons buttons = Dialog::BUTTONS_OKCANCEL);

			/**
			 * Shows a input dialog with a given title and message text,
			 * displays an icon and a set of buttons, and goes into a modal loop.
			 * The user must choose one of the buttons provided, which closes
			 * the window and an ID for the pressed button is returned.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * Defaults to OK and CANCEL.
			 * @param input The string variable to be filled with the text input.
			 * @return The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the input dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 */
			static Dialog::Buttons showDialog(const String & title, const String & message,
				String & input, Dialog::Buttons buttons = Dialog::BUTTONS_OKCANCEL);
	};
}


#endif
