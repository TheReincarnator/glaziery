/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTFIELD_H
#define __GLAZIERY_INPUTFIELD_H


namespace glaziery
{
	class InputField : public Field
	{
		private:

			/**
			 * Whether the content of this input field (text etc.) can be edited.
			 */
			bool editable;

		protected:

			/**
			 * Creates a new input field.
			 */
			InputField();

			/**
			 * Destroys the input field.
			 */
			virtual ~InputField();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(InputField);
			#endif

		public:

			/**
			 * Returns whether the ENTER key is entirely consumed by this input field.
			 * If not, it may press the field area's default button.
			 * Defaults to false.
			 * @return Whether the ENTER key is entirely consumed by this input field.
			 */
			virtual bool consumingEnter();

			/**
			 * Returns whether the content of this input field (text etc.) can be edited.
			 * @return Whether the content of this input field can be edited.
			 */
			bool isEditable();

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
			 * Handles the event that the ENTER key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onEnter(bool option1, bool option2);

			/**
			 * Handles the event that the input field has been activated by its hot key.
			 * The hot key dispatching has already been performed.
			 * @param direct Whether the pressed hot key is the one of this field.
			 * If false, a preceeding non-input field's hot key (e.g. a label) matched.
			 */
			virtual void onMatchedHotKey(bool direct);

			/**
			 * Sets whether this input field can get focus and therefore
			 * can be edited or selected. Not active fields are usually grayed out.
			 * @param active Whether this input field is active.
			 */
			virtual void setActive(bool active);

			/**
			 * Sets whether the content of this input field (text etc.) can be edited.
			 * @param editable Whether the content of this input field can be edited.
			 */
			void setEditable(bool editable);

			/**
			 * Sets that this input field gets focus.
			 * @note This method does not check whether the field is visible and active.
			 */
			void setFocus();
	};
}


#endif
