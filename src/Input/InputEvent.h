/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTEVENT_H
#define __GLAZIERY_INPUTEVENT_H


namespace glaziery
{
	/**
	 * An input event issued by the platform adapter.
	 * The input handling of the adapter instantiates sub classes of this class
	 * and adds them to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class InputEvent : public CObject
	#else
		class InputEvent
	#endif
	{
		friend ArrayList<InputEvent>;
		friend Desktop;
		friend Window;

		protected:

			/**
			 * Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 */
			bool option1;

			/**
			 * Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			bool option2;

			/**
			 * Creates a new input event.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			InputEvent(bool option1, bool option2);

			/**
			 * Destroys the input event.
			 */
			virtual ~InputEvent();

			/**
			 * Executes (handles) the event.
			 */
			virtual void execute() = NULL;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(InputEvent);
			#endif
	};
}


#endif
