/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_KEYEVENT_H
#define __GLAZIERY_KEYEVENT_H


namespace glaziery
{
	/**
	 * Base class for all key related input events.
	 * The input handling of the adapter instantiates sub classes of this class
	 * and adds them to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class KeyEvent : public InputEvent
	{
		protected:

			/**
			 * Whether the key is currently held down.
			 */
			bool held;

			/**
			 * Whether the key is newly pressed down.
			 */
			bool pressed;

			/**
			 * Whether the key has been released.
			 */
			bool released;

			/**
			 * The number of key repeats.
			 */
			int repeats;

			/**
			 * The scan code of the key that caused the event.
			 */
			int scanCode;

			/**
			 * Executes (handles) the event.
			 */
			virtual void execute();

			/**
			 * Executes (handles) the event on a given event target.
			 * The default implementation does nothing and returns false.
			 * @param target The event target to execute the event on.
			 */
			virtual void executeOn(EventTarget * target);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(KeyEvent);
			#endif

		public:

			/**
			 * Creates a new key event.
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
			 */
			KeyEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
				bool released, int repeats);
	};
}


#endif
