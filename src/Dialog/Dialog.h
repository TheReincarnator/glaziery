/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIALOG_H
#define __GLAZIERY_DIALOG_H


namespace glaziery
{
	/**
	 * The dialog is the super class for all modal windows with confirming
	 * or choosing modal buttons, e.g. for message, file, and color dialogs.
	 */
	class Dialog : public ModalWindow
	{
		protected:

			class ButtonListener : public Button::Listener
			{
				private:

					bool inResize;

				public:

					ButtonListener();

					/**
					 * Called when the component has been resized.
					 * @param component The component that received the event.
					 * @param oldSize The old size of the component before the resizing.
					 */
					virtual void onResized(Component * component, Vector oldSize);
			};

		public:

			/**
			 * The button combination to be shown. Use bit OR (|) to
			 * specify multiple buttons.
			 * If specified, buttons are shown in the following order:
			 */
			enum Buttons
			{
				// Buttons

				/**
				 * Show an OK button (resp. the OK button has been pressed).
				 */
				BUTTON_OK = 1,

				/**
				 * Show a Cancel button (resp. the Cancel button has been pressed).
				 */
				BUTTON_CANCEL = 2,

				/**
				 * Show a Close button (resp. the Close button has been pressed).
				 */
				BUTTON_CLOSE = 4,

				/**
				 * Show a Retry button (resp. the Retry button has been pressed).
				 */
				BUTTON_RETRY = 8,

				/**
				 * Show an Ignore button (resp. the Ignore button has been pressed).
				 */
				BUTTON_IGNORE = 16,

				/**
				 * Show a Yes button (resp. the Yes button has been pressed).
				 */
				BUTTON_YES = 32,

				/**
				 * Show a No button (resp. the No button has been pressed).
				 */
				BUTTON_NO = 64,

				// Short hands

				/**
				 * Short-hand for showing an OK and a Cancel button.
				 */
				BUTTONS_OKCANCEL = 3,

				/**
				 * Short-hand for showing an OK and a Cancel button.
				 */
				BUTTONS_RETRYIGNORECANCEL = 26,

				/**
				 * Short-hand for showing an OK and a Cancel button.
				 */
				BUTTONS_YESNO = 96,

				/**
				 * Short-hand for showing an OK and a Cancel button.
				 */
				BUTTONS_YESNOCANCEL = 98,

				// "No button" result

				/**
				 * No button has been pressed when showing the message dialog.
				 * This means the window has been closed or the Desktop shuts down.
				 */
				NO_BUTTON = 0
			};

		friend ArrayList<Dialog>;

		protected:

			/**
			 * Whether the dialog automatically closes when ended. Defaults to true.
			 * On false, the dialog stays open and you have to close it.
			 */
			bool autoClosingOnEnd;

			/**
			 * The button to press when this dialog automatically ends.
			 * @see autoEndTime
			 */
			Buttons autoEndButton;

			/**
			 * The delay in milli seconds when this dialog automatically ends,
			 * if no element is clicked and no key is pressed,
			 * or -1, if the dialog is not auto-ending.
			 */
			unsigned long autoEndDelay;

			/**
			 * The time when this dialog automatically ends,
			 * if no element is clicked and no key is pressed.
			 * 0, which is the default, indicates not to auto-end this dialog.
			 */
			unsigned long autoEndTime;

			/**
			 * Before the dialog is running, the number of milli seconds to inactivate the buttonDelayButtons.
			 * When the dialog is running, the time when the buttonDelayButtons will be active again.
			 * @see buttonDelayButtons
			 */
			unsigned long buttonDelayTime;

			/**
			 * The buttons that will be set inactive when the dialog starts,
			 * and back to active when buttonDelayTime is reached.
			 * @see buttonDelayTime
			 */
			int buttonDelayButtons;

			/**
			 * The buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * If specified, buttons appear in the following order:
			 * BUTTON_YES, BUTTON_NO, BUTTON_RETRY, BUTTON_IGNORE, BUTTON_OK, BUTTON_CLOSE, BUTTON_CANCEL.
			 */
			Buttons buttons;

			/**
			 * The group field containing the buttons.
			 */
			GroupField * buttonsGroup;

			/**
			 * The cancel button, if it exists, otherwise NULL.
			 * If this button exists, the close widget and cancel key
			 * are also enabled.
			 */
			Button * cancelButton;

			/**
			 * The close button, if it exists, otherwise NULL.
			 * If this button exists, the close widget, the cancel key
			 * and the enter key are also enabled.
			 * If the ok button exists too, it has precedence for the
			 * enter key.
			 * If the cancel button exists too, it has precedence for the
			 * close widget and the cancel key.
			 */
			Button * closeButton;

			/**
			 * The ignore button, if it exists, otherwise NULL.
			 */
			Button * ignoreButton;

			/**
			 * Whether the dialog has already been initialized.
			 */
			bool initialized;

			/**
			 * Whether the dialog has already been started.
			 */
			bool started;

			/**
			 * The no button, if it exists, otherwise NULL.
			 */
			Button * noButton;

			/**
			 * The ok button, if it exists, otherwise NULL.
			 * If this button exists, the close widget, the cancel key
			 * and the enter key are also enabled.
			 * If the cancel button exists too, it has precedence for the
			 * close widget and the cancel key.
			 */
			Button * okButton;

			/**
			 * The button that has been pressed in the last modal run.
			 * This attribute is reset when the window is created and when the
			 * runModally() method is called. It is set by a modal button,
			 * when it is pressed. The attribute is read by the runModally() method.
			 * When it is set, the method ends and the button is returned to the caller.
			 * @see runModally()
			 * @see Button::isModal()
			 */
			Button * pressedButton;

			/**
			 * The retry button, if it exists, otherwise NULL.
			 */
			Button * retryButton;

			/**
			 * The yes button, if it exists, otherwise NULL.
			 */
			Button * yesButton;

			/**
			 * Creates a new dialog.
			 */
			Dialog();

			/**
			 * Destroys the dialog.
			 */
			virtual ~Dialog();

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
			 * Performs operations when a modal run starts.
			 */
			virtual void onModalStart();

			/**
			 * Performs operations for each frame while the window is running modally.
			 */
			virtual void onModalUpdate();

			/**
			 * Resets the auto close time to the sum of now and the auto-end delay,
			 * if the dialog is auto-closing.
			 */
			void resetAutoEndTime();

			/**
			 * Initializes the dialog (creates fields etc.).
			 * In static creator and/or show dialog methods, call this method after having created
			 * and added the dialog to the desktop.
			 * @note In this method, you should not rely on dialog properties that tune the dialog.
			 * Perform such steps in onDialogStart() instead.
			 * @see setupContent()
			 * @see onDialogStart()
			 * @see showModally()
			 */
			virtual void setup();

			/**
			 * Initializes the buttons group of the dialog.
			 */
			virtual void setupButtons();

			/**
			 * Initializes the content group and other properties of the dialog,
			 * after it has been created and added to the desktop, but before the dialog is shown.
			 * This method is automatically called by the setup() method.
			 * @param area The fields area of this dialog.
			 * @param contentGroup The content group of this dialog.
			 * @note In this method, you should not rely on dialog properties that tune the dialog.
			 * Perform such steps in onDialogStart() instead.
			 * @note Implement this method in derivatives.
			 * @see setup()
			 * @see onDialogStart()
			 * @see showModally()
			 */
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup) = NULL;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Dialog);
			#endif

		public:

			/**
			 * Ends the dialog by cancelling it. This automatically sets the most appropriate
			 * cancelling button as pressed button, and ending the modal run.
			 * Cancelling the dialog behaves like pressing the close widgets on the dialog,
			 * or pressing the ESC key on Windows systems for instance.
			 * @see endDialog()
			 * @see endDialogByDefault()
			 * @see getPressedButton()
			 * @see runModally()
			 */
			void cancelDialog();

			/**
			 * Ends the dialog, setting the button that has been pressed in the modal run.
			 * @param pressedButtonId The ID of the pressed button.
			 * @see cancelDialog()
			 * @see endDialogByDefault()
			 * @see runModally()
			 */
			void endDialog(Buttons pressedButtonId);

			/**
			 * Ends the dialog, setting the button that has been pressed in the modal run.
			 * @param pressedButton The pressed button.
			 * @see cancelDialog()
			 * @see endDialogByDefault()
			 * @see runModally()
			 */
			void endDialog(Button * pressedButton);

			/**
			 * Ends the dialog, setting the most appropriate button for a regular dialog end,
			 * and ending the modal run.
			 * @see cancelDialog()
			 * @see endDialog()
			 * @see getPressedButton()
			 * @see runModally()
			 */
			void endDialogByDefault();

			/**
			 * Returns the button to press when this dialog automatically closes,
			 * if no element is clicked and no key is pressed,
			 * or NO_BUTTON, if the dialog is not auto-closing.
			 * @return The auto-close button or NO_BUTTON.
			 * @see getAutoEndTime()
			 */
			Buttons getAutoEndButton();

			/**
			 * Returns the delay in milli seconds when this dialog automatically closes,
			 * if no element is clicked and no key is pressed,
			 * or -1, if the dialog is not auto-closing.
			 * @return The auto-close delay in milli seconds or -1.
			 */
			unsigned long getAutoEndDelay();

			/**
			 * Returns the time when this dialog automatically closes,
			 * if no element is clicked and no key is pressed.
			 * 0, which is the default, indicates not to auto end this dialog.
			 * @return The auto-close time or 0.
			 */
			unsigned long getAutoEndTime();

			/**
			 * Returns a button by its ID, or NULL, if the button does not exist.
			 * @param buttonId The button ID.
			 * @return The button or NULL.
			 */
			Button * getButton(Buttons buttonId);

			/**
			 * Returns the buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * If specified, buttons appear in the following order:
			 * BUTTON_YES, BUTTON_NO, BUTTON_RETRY, BUTTON_IGNORE, BUTTON_OK, BUTTON_CLOSE, BUTTON_CANCEL.
			 * @return The buttons to be displayed
			 */
			Buttons getButtons(void);

			/**
			 * Returns the fields area of this dialog.
			 * @return The fields area of this dialog.
			 */
			FieldsArea * getFieldsArea();

			/**
			 * Returns the button that has been pressed in the last modal run (runModally()).
			 * This attribute is reset when the window is created and when the
			 * runModally() method is called. It is set by a modal button,
			 * when it is pressed. The attribute is read by the runModally() method.
			 * When it is set, the method ends and the button is returned to the caller.
			 * @return The pressed button.
			 * @see runModally()
			 * @see Button::isModal()
			 */
			Button * getPressedButton();

			/**
			 * Returns whether the dialog automatically closes when ended. Defaults to true.
			 * On false, the dialog stays open and you have to close it.
			 * @return Whether the dialog automatically closes when ended.
			 */
			bool isAutoClosingOnEnd();

			/**
			 * Returns whether the dialog has already been initialized.
			 * @return Whether the dialog has already been initialized.
			 */
			bool isInitialized();

			/**
			 * Handles the cancel event on the event target.
			 * On Windows systems, this is the ESC key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCancel();

			/**
			 * Handles the event that any key event occurred.
			 * This method is called in addition to another key method,
			 * such as onCharacter or onMoveDown. The onAnyKey method is called first.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onAnyKey(bool option1, bool option2);

			/**
			 * Handles the event that the event target is clicked
			 * (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onPress()
			 * @see onRelease()
			 * @see onDoubleClick()
			 */
			virtual void onClick(Vector position, bool option1, bool option2);

			/**
			 * Handles the event that the close widget has been clicked.
			 */
			virtual void onCloseWidgetClicked();

			/**
			 * Handles the event that the event target is context-clicked
			 * (usually using the right mouse button).
			 * @param position The position where the context click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContextClick(Vector position, bool option1, bool option2);

			/**
			 * Sets whether the dialog automatically closes when ended. Defaults to true.
			 * On false, the dialog stays open and you have to close it.
			 * @param autoClosingOnEnd Whether the dialog automatically closes when ended.
			 */
			void setAutoClosingOnEnd(bool autoClosingOnEnd);

			/**
			 * Sets that this dialog auto-closes, pressing a button,
			 * if no element is clicked and no key is pressed for a given time.
			 * @param autoEndDelay The auto-close delay in milli seconds.
			 * -1 disables auto-closing.
			 * @param autoEndButton The auto-close button.
			 */
			void setAutoEnding(unsigned long autoEndDelay, Buttons autoEndButton);

			/**
			 * Sets that a given set of buttons should be inactive at dialog start,
			 * and after a given interval, should automatically become active.
			 * @param buttons The buttons to temporarily inactivate.
			 * @param delayMilliSeconds The milli seconds to inactivate the buttons.
			 */
			void setButtonDelay(Buttons buttons, unsigned long delayMilliSeconds);

			/**
			 * Sets the buttons to be displayed, taken from the Buttons enum.
			 * You can add multiple buttons by using the bitwise OR (|).
			 * If specified, buttons appear in the following order:
			 * BUTTON_YES, BUTTON_NO, BUTTON_RETRY, BUTTON_IGNORE, BUTTON_OK, BUTTON_CLOSE, BUTTON_CANCEL.
			 * @param buttons The buttons to be displayed
			 */
			void setButtons(Buttons buttons);

			/**
			 * Sets the text (the caption) of a button.
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 * @param buttonId The ID of the button to be changed.
			 * @param text The text (the caption) of the button.
			 * @note Call setup first, before calling this method.
			 */
			void setButtonText(int buttonId, const String & text);

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
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
