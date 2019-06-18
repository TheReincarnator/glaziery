/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CHARACTEREVENT_H
#define __GLAZIERY_CHARACTEREVENT_H


namespace glaziery
{
	/**
	 * The event that a character has been entered.
	 * This is used for text input fields to enter text.
	 * The input handling of the adapter instantiates this class and adds them
	 * to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class CharacterEvent : public KeyEvent
	{
		friend Desktop;

		private:

			/**
			 * The first character that has been entered, using the ISO-8859-1 encoding,
			 * or 0, if this character is not valid.
			 */
			char character1;

			/**
			 * The second character that has been entered, using the ISO-8859-1 encoding,
			 * or 0, if this character is not valid.
			 */
			char character2;

		protected:

			/**
			 * Executes (handles) the event on a given event target.
			 * The default implementation does nothing and returns false.
			 * @param target The event target to execute the event on.
			 */
			virtual void executeOn(EventTarget * target);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(CharacterEvent);
			#endif

		public:

			/**
			 * Creates a new character event.
			 *
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param scanCode The scan code of the key that caused the event.
			 * @param held Whether the key is currently held down.
			 * @param pressed Whether the key is newly pressed down.
			 * @param released Whether the key has been released.
			 * @param repeats The number of key repeats.
			 * @param character1 The first character that has been entered, using the ISO-8859-1 encoding,
			 * or 0, if this character is not valid.
			 * @param character2 The second character that has been entered, using the ISO-8859-1 encoding,
			 * or 0, if this character is not valid.
			 */
			CharacterEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
				bool released, int repeats, char character1, char character2);
	};
}


#endif
