/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POINTERBUTTONEVENT_H
#define __GLAZIERY_POINTERBUTTONEVENT_H


namespace glaziery
{
	/**
	 * The event that a button of the pointer (mouse, joystick etc.) is pressed or released.
	 * The input handling of the adapter instantiates this class and adds them
	 * to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class PointerButtonEvent : public InputEvent
	{
		friend Desktop;

		private:

			/**
			 * The button was pressed (otherwise, it was released).
			 */
			bool pressed;

			/**
			 * The primary (select) button caused the event.
			 * Otherwise, it is the secondary (context) button.
			 */
			bool primary;

			/**
			 * Destroys all popup windows on top of a given hit popup,
			 * or destroys all popup windows, if the given hit target is no popup at all.
			 * @param hitTarget The target hit by the pointer.
			 */
			void cancelFocusLossPopups(EventTarget * hitTarget);

			/**
			 * Fills a given ArrayList with a event target and all its ancestors, in that order.
			 * @param ancestors The ArrayList to be filled.
			 * @param eventTarget The event target to start with.
			 */
			void collectAncestors(ArrayList<EventTarget> & ancestors, EventTarget * eventTarget);

		protected:

			/**
			 * Executes (handles) the event.
			 */
			virtual void execute();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PointerButtonEvent);
			#endif

		public:

			/**
			 * Creates a new pointer button event.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @param pressed The button was pressed (otherwise, it was released).
			 * @param primary The primary (select) button caused the event.
			 * Otherwise, it is the secondary (context) button.
			 */
			PointerButtonEvent(bool option1, bool option2, bool pressed, bool primary);
	};
}


#endif
