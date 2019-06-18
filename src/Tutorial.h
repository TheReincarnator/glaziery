/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TUTORIAL_H
#define __GLAZIERY_TUTORIAL_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class Tutorial : public GlazieryObject
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Tutorial);
			#endif

		private:

			bool canceled;

		protected:

			/**
			 * Simulates the BACKSPACE key (backward delete) on the focus window or popup.
			 * Use this method in tutorials only.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialBackSpace(bool visualize = true);

			/**
			 * Simulates the back TAB key on the focus window or popup.
			 * On Windows systems, this is SHIFT+TAB.
			 * Back tabs step the focus from field to field in backward order.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialBackTab(bool secondary, bool visualize = true);

			/**
			 * Simulates the cancel event on the focus window or popup.
			 * On Windows systems, this is the ESC key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialCancel(bool visualize = true);

			/**
			 * Simulates a regular character has been entered on the focus window or popup.
			 *
			 * This is used for text input fields to enter text.
			 * @param character The character pressed, using ISO-8859-1 encoding.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialCharacter(char character, bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the DELETE key (forward delete) on the focus window or popup.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialDelete(bool visualize = true);

			/**
			 * Simulates the EDIT key on the focus window or popup.
			 * On Windows systems, this is the F2 key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialEdit(bool visualize = true);

			/**
			 * Simulates the ENTER key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialEnter(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates a hot key has been pressed on the focus window or popup.
			 * Hot keys represent special characters e.g. to easily access fields
			 * (indicated by underlined characters).
			 * @param character The hot key character pressed.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialHotKey(char character, bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates a planned key stroke has been pressed on the focus window or popup.
			 * Key strokes are usually received by key stroke fields.
			 * When the desktop is in key stroke mode, the next key pressed
			 * should be sent to this method.
			 * This is useful e.g. to define keyboard layouts for games.
			 * @param keyCode The key stroke key code.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 * @see Desktop::isKeyStrokeMode()
			 */
			void tutorialKeyStroke(int keyCode, bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the DOWN key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveDown(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the LEFT key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveLeft(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the RIGHT key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveRight(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the END key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveToEnd(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the START (HOME, POS1, ...) key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveToStart(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the UP key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialMoveUp(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the PAGE DOWN key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialPageDown(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the PAGE UP key on the focus window or popup.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialPageUp(bool option1, bool option2, bool visualize = true);

			/**
			 * Simulates the user requested to select all
			 * (e.g. by pressing Ctrl+A on Windows systems).
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialSelectAll(bool visualize = true);

			/**
			 * Simulates the TAB key.
			 * Tabs step the focus from field to field.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @param visualize Whether to visualize the key press
			 * (e.g. by a little key symbol). Defaults to true.
			 */
			void tutorialTab(bool secondary, bool visualize = true);

			/**
			 * Pauses the current tutorial for a given time.
			 * @param time The time in milli seconds.
			 */
			void tutorialWait(long time);

			/**
			 * Pauses the current tutorial until the user presses SPACE or ESC.
			 */
			void tutorialWaitForStep();

		public:

			/**
			 * Creates a new tutorial.
			 */
			Tutorial();

			/**
			 * Destroys the tutorial.
			 */
			virtual ~Tutorial();

			void cancel();

			bool isCanceled();

			virtual void run() = NULL;
	};
}


#endif
