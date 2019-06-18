/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POINTERMOVEEVENT_H
#define __GLAZIERY_POINTERMOVEEVENT_H


namespace glaziery
{
	/**
	 * The event that the position of the pointer (mouse, joystick etc.)
	 * is moved relative to its current position. You do not need to ensure
	 * that the position stays within the screen, the event handler does that for you.
	 * The input handling of the adapter instantiates this class and adds them
	 * to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class PointerMoveEvent : public InputEvent
	{
		friend Desktop;

		private:

			/**
			 * The old position of the pointer.
			 */
			Vector oldPosition;

			/**
			 * The new position of the pointer.
			 */
			Vector newPosition;

		protected:

			/**
			 * Executes (handles) the event.
			 */
			virtual void execute();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PointerMoveEvent);
			#endif

		public:

			/**
			 * Creates a new pointer move event.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param oldPosition The old position of the pointer.
			 * @param newPosition The new position of the pointer.
			 */
			PointerMoveEvent(bool option1, bool option2, Vector oldPosition, Vector newPosition);
	};
}


#endif
