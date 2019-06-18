/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MESSAGEDIALOG_H
#define __GLAZIERY_MESSAGEDIALOG_H


namespace glaziery
{
	/**
	 * The message dialog is a helper to easily create frequently required
	 * dialogs that have a message and a set of buttons to confirm or
	 * answer this message. This includes notes, warnings, error messages,
	 * and questions, indicated by a respective icon. You can choose from
	 * a set of confirmation buttons, including ok/cancel, yes/no, and others.
	 * You can use the message dialog best by calling one of its static
	 * members.
	 */
	class MessageDialog : public Dialog
	{
		public:

			/**
			 * The icon to be shown.
			 */
			enum Icon
			{
				/**
				 * Show no icon.
				 */
				ICON_NONE,

				/**
				 * Show an information icon.
				 */
				ICON_INFORMATION,

				/**
				 * Show a question icon.
				 */
				ICON_QUESTION,

				/**
				 * Show a warning icon.
				 */
				ICON_WARNING,

				/**
				 * Show an error icon.
				 */
				ICON_ERROR
			};

		protected:

			/**
			 * The icon to be displayed.
			 */
			Image * iconImage;

			/**
			 * The message to be displayed.
			 */
			String message;

			/**
			 * The optional checkbox for remembering the decision for yes or no.
			 */
			CheckBox * rememberCheckbox;

			/**
			 * If set, creates a checkbox to remember the decision for yes or no.
			 * If the user checks the checkbox and presses yes or no, next time,
			 * the dialog does not appear, and returns the last value.
			 * If the ID is not set, no checkbox is shown.
			 */
			String rememberId;

			/**
			 * Creates a new message dialog.
			 * @param message The main message text.
			 * @param iconImage The image of the icon to be displayed,
			 * which defines the type of the window. May be NULL.
			 */
			MessageDialog(const String & message, Image * iconImage);

			/**
			 * Destroys the message dialog.
			 */
			virtual ~MessageDialog();

			/**
			 * Initializes the content group:
			 * @param area The fields area of this dialog.
			 * @param contentGroup The content group of this dialog.
			 */
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(MessageDialog);
			#endif

		public:

			/**
			 * Creates a new message dialog.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param icon The icon to be displayed,
			 * which defines the type of the window.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @return The new message dialog.
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static MessageDialog * createDialog(const String & title, const String & message,
				Icon icon, Dialog::Buttons buttons);

			/**
			 * Creates a new message dialog.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param iconImageId The ID of the image of the icon to be displayed,
			 * which defines the type of the window.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @return The new message dialog.
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static MessageDialog * createDialog(const String & title, const String & message,
				const String & iconImageId, Dialog::Buttons buttons);

			/**
			 * Creates a new message dialog.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param iconImage The image of the icon to be displayed,
			 * which defines the type of the window. May be NULL.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @return The new message dialog.
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static MessageDialog * createDialog(const String & title, const String & message,
				Image * iconImage, Dialog::Buttons buttons);

			/**
			 * Returns the remember ID. If set, creates a checkbox to remember the decision for yes or no.
			 * If the user checks the checkbox and presses yes or no, next time,
			 * the dialog does not appear, and returns the last value.
			 * If the ID is not set, no checkbox is shown.
			 * @return The remember ID.
			 */
			const String & getRememberId();

			/**
			 * Sets the remember ID. If set, creates a checkbox to remember the decision for yes or no.
			 * If the user checks the checkbox and presses yes or no, next time,
			 * the dialog does not appear, and returns the last value.
			 * If the ID is not set, no checkbox is shown.
			 * @param rememberId The remember ID.
			 */
			void setRememberId(const String & rememberId);

			/**
			 * Initializes and performs the dialog, creating groups and buttons, calling setup(),
			 * and running the dialog modally. Finally, returns the button pressed.
			 * @return The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @see Buttons
			 * @note Call this method from a static showDialog method in derivatives.
			 * In that method, first create a dialog instance, add it to the desktop,
			 * and finally call this method.
			 */
			virtual Buttons showModally();

			/**
			 * Shows a message dialog with a given title and message text,
			 * displays an icon and a set of buttons, and goes into a modal loop.
			 * The user must choose one of the buttons provided, which closes
			 * the window and an ID for the pressed button is returned.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param icon The icon to be displayed,
			 * which defines the type of the window.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @param autoEndDelay The delay in milli seconds when this dialog automatically closes,
			 * if no element is clicked and no key is pressed, or -1,
			 * if the dialog is not auto-closing. Disabled by default.
			 * @param autoEndButton The button to press when this dialog automatically closes.
			 * @return The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @see Buttons
			 */
			static Dialog::Buttons showDialog(const String & title, const String & message,
				Icon icon, Dialog::Buttons buttons, unsigned long autoEndDelay = -1,
				Buttons autoEndButton = NO_BUTTON);

			/**
			 * Shows a message dialog with a given title and message text,
			 * displays an icon and a set of buttons, and goes into a modal loop.
			 * The user must choose one of the buttons provided, which closes
			 * the window and an ID for the pressed button is returned.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param iconImageId The ID of the image of the icon to be displayed,
			 * which defines the type of the window.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @param autoEndDelay The delay in milli seconds when this dialog automatically closes,
			 * if no element is clicked and no key is pressed, or -1,
			 * if the dialog is not auto-closing. Disabled by default.
			 * @param autoEndButton The button to press when this dialog automatically closes.
			 * @return The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @see Buttons
			 */
			static Dialog::Buttons showDialog(const String & title, const String & message,
				const String & iconImageId, Dialog::Buttons buttons, unsigned long autoEndDelay = -1,
				Buttons autoEndButton = NO_BUTTON);

			/**
			 * Shows a message dialog with a given title and message text,
			 * displays an icon and a set of buttons, and goes into a modal loop.
			 * The user must choose one of the buttons provided, which closes
			 * the window and an ID for the pressed button is returned.
			 * @param title The window title bar string.
			 * @param message The main message text.
			 * @param iconImage The image of the icon to be displayed,
			 * which defines the type of the window. May be NULL.
			 * @param buttons The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * @param autoEndDelay The delay in milli seconds when this dialog automatically closes,
			 * if no element is clicked and no key is pressed, or -1,
			 * if the dialog is not auto-closing. Disabled by default.
			 * @param autoEndButton The button to press when this dialog automatically closes.
			 * @return The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @see Buttons
			 */
			static Dialog::Buttons showDialog(const String & title, const String & message,
				Image * iconImage, Dialog::Buttons buttons, unsigned long autoEndDelay = -1,
				Buttons autoEndButton = NO_BUTTON);
	};
}


#endif
