/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SPECIALEVENT_H
#define __GLAZIERY_SPECIALEVENT_H


namespace glaziery
{
	/**
	 * A special event, e.g. select all or backspace, usually raised by the keyboard.
	 * The input handling of the adapter instantiates this class and adds them
	 * to the desktop's event queue by using its addEvent() method.
	 * @see Desktop::addEvent()
	 */
	class SpecialEvent : public KeyEvent
	{
		friend Desktop;

		public:

			enum Type
			{
				/**
				 * No event.
				 */
				EVENT_NONE,

				/**
				 * The BACKSPACE key (backward delete) has been pressed.
				 */
				EVENT_BACK_SPACE,

				/**
				 * The primary back TAB combination has been pressed.
				 * On Windows systems, this is SHIFT+TAB.
				 * Primary back tabs step the focus from field to field in backward order.
				 */
				EVENT_BACK_TAB,

				/**
				 * The cancel event.
				 * On Windows systems, this is the ESC key.
				 */
				EVENT_CANCEL,

				/**
				 * The clipboard copy event.
				 * On Windows systems, this is the Ctrl+C or the Ctrl+Ins key combination.
				 */
				EVENT_COPY,

				/**
				 * The clipboard cut event.
				 * On Windows systems, this is the Ctrl+X or the Shift+Del key combination.
				 */
				EVENT_CUT,

				/**
				 * The DELETE key (forward delete) has been pressed.
				 */
				EVENT_DELETE,

				/**
				 * The EDIT key has been pressed.
				 * On Windows systems, this is the F2 key.
				 */
				EVENT_EDIT,

				/**
				 * The ENTER key has been pressed.
				 */
				EVENT_ENTER,

				/**
				 * The DOWN key has been pressed.
				 */
				EVENT_MOVE_DOWN,

				/**
				 * The LEFT key has been pressed.
				 */
				EVENT_MOVE_LEFT,

				/**
				 * The END key has been pressed.
				 */
				EVENT_MOVE_TO_END,

				/**
				 * The START key has been pressed.
				 * On Windows systems, this is the HOME or POS1 key.
				 */
				EVENT_MOVE_TO_START,

				/**
				 * The RIGHT key has been pressed.
				 */
				EVENT_MOVE_RIGHT,

				/**
				 * The UP key has been pressed.
				 */
				EVENT_MOVE_UP,

				/**
				 * The PAGE DOWN key has been pressed.
				 */
				EVENT_PAGE_DOWN,

				/**
				 * The PAGE UP key has been pressed.
				 */
				EVENT_PAGE_UP,

				/**
				 * The clipboard paste event.
				 * On Windows systems, this is the Ctrl+V or the Shift+Ins key combination.
				 */
				EVENT_PASTE,

				/**
				 * The secondary back TAB combination has been pressed.
				 * On Windows systems, this is SHIFT+CTRL+TAB.
				 * Back tabs step the tabbing area to tabbing area in backward order.
				 */
				EVENT_SECONDARY_BACK_TAB,

				/**
				 * The secondary TAB combination has been pressed.
				 * On Windows systems, this is CTRL+TAB.
				 * Back tabs step the tabbing area to tabbing area.
				 */
				EVENT_SECONDARY_TAB,

				/**
				 * The user has requested to select all
				 * (e.g. by pressing Ctrl+A on Windows systems).
				 */
				EVENT_SELECT_ALL,

				/**
				 * The primary TAB combination has been pressed.
				 * On Windows systems, this is TAB.
				 * Primary tabs step the focus from field to field.
				 */
				EVENT_TAB
			};

		protected:

			/**
			 * The type of the special event.
			 */
			Type type;

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
				DECLARE_DYNAMIC(SpecialEvent);
			#endif

		public:

			/**
			 * Creates a new special event with two options.
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
			 * @param type The type of the special event.
			 */
			SpecialEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
				bool released, int repeats, Type type);
	};
}


#endif
