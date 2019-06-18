/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POINTERWHEELEVENT_H
#define __GLAZIERY_POINTERWHEELEVENT_H


namespace glaziery
{
	/**
	 * The event that the position of a scrool wheel of a mouse, joystick etc.
	 * has been changed.
	 * The input handling of the adapter instantiates this class and adds them
	 * to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class PointerWheelEvent : public InputEvent
	{
		friend Desktop;

		private:

			/**
			 * The relative movement of the wheel.
			 */
			int delta;

		protected:

			/**
			 * Executes (handles) the event.
			 */
			virtual void execute();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PointerWheelEvent);
			#endif

		public:

			/**
			 * Creates a new pointer wheel event.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param delta The relative movement of the wheel.
			 */
			PointerWheelEvent(bool option1, bool option2, int delta);
	};
}


#endif
