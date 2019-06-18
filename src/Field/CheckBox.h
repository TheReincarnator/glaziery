/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CHECKBOX_H
#define __GLAZIERY_CHECKBOX_H


#define GLAZIERY_CHECKBOX(variable, area, groupField, \
	text, state) \
	CheckBox * variable; \
	if ((variable = new CheckBox) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setText(text); \
	variable->setState(CheckBox::##state);

#define GLAZIERY_CHECKBOX_P(variable, area, groupField, px, py, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->moveTo(Vector((px), (py)));

#define GLAZIERY_CHECKBOX_S(variable, area, groupField, width, height, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_CHECKBOX_PS(variable, area, groupField, px, py, width, height, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_CHECKBOX_M(variable, area, groupField, minimumWidth, minimumHeight, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_CHECKBOX_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_CHECKBOX_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_CHECKBOX_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	text, state) \
	GLAZIERY_CHECKBOX(variable, area, groupField, text, state) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	class CheckBox : public InputField
	{
		public:

			/**
			 * The possible states of a check box.
			 */
			enum State
			{
				/**
				 * The check box is not checked.
				 */
				UNCHECKED,

				/**
				 * The check box is checked.
				 */
				CHECKED,

				/**
				 * The check box is in tri-state (third, grayed state).
				 * This state is only possible if the check box supports tri-state.
				 */
				TRI_STATE
			};

			/**
			 * The listener interface for check box events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public InputField::Listener
			{
				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					/**
					 * Called when the check box'es state has been changed,
					 * either using the user interface or using the setChecked()
					 * or setState() methods.
					 * @param checkBox The check box that has been changed.
					 * @see onStateChanging()
					 * @see CheckBox::setChecked()
					 * @see CheckBox::setState()
					 */
					virtual void onStateChanged(CheckBox * checkBox);

					/**
					 * Called when the check box'es state is about to be changed,
					 * either using the user interface or using the setChecked()
					 * or setState() methods.
					 * @param checkBox The check box that is about to be changed.
					 * @param newState The new state the check box should receive.
					 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
					 * @see VetoMode
					 * @see onStateChanged()
					 * @see CheckBox::setChecked()
					 * @see CheckBox::setState()
					 */
					virtual VetoMode onStateChanging(CheckBox * checkBox, State newState);
			};

		private:

			/**
			 * The current state of the check box, UNCHECKED, CHECKED, or TRI_STATE.
			 * @see State
			 */
			State state;

			/**
			 * The text (the caption) of this field.
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the field.
			 */
			String text;

			/**
			 * Whether this check box supports the tri-state (third, grayed state).
			 */
			bool triStateSupported;

		protected:

			/**
			 * Destroys the check box.
			 */
			virtual ~CheckBox();

			/**
			 * Returns the text string attribute, if the field supports text editing, otherwise NULL.
			 * Defaults to NULL, override in derivatives that support text editing.
			 * @note getTextAttribute() returns NULL, the field does not support text editing.
			 * setText does nothing, getText returns an empty string in this case.
			 * @return The text string attribute or NULL.
			 * @see getText()
			 * @see setText()
			 */
			virtual String * getTextAttribute();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(CheckBox);
			#endif

		public:

			/**
			 * Creates a new check box.
			 */
			CheckBox();

			/**
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Returns the current state of the check box, UNCHECKED, CHECKED, or TRI_STATE.
			 * @return The current state of the check box.
			 * @see State
			 * @see isChecked()
			 */
			State getState();

			/**
			 * Returns whether this check box is checked (the state is CHECKED).
			 * TRI_STATE is considered not checked. Use getState() for a more detailed state check.
			 * @return Whether this check box is checked (the state is CHECKED).
			 * @see getState()
			 */
			bool isChecked();

			/**
			 * Returns whether this check box supports the tri-state (third, grayed state).
			 * @return Whether this check box supports the tri-state.
			 */
			bool isTriStateSupported();

			/**
			 * Handles the event that a regular character has been entered
			 * on the event target.
			 * This is used for text input fields to enter text.
			 * @param character The character pressed, using ISO-8859-1 encoding.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCharacter(char character, bool option1, bool option2);

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
			 * Handles the event that the input field has been activated by its hot key.
			 * The hot key dispatching has already been performed.
			 * @param direct Whether the pressed hot key is the one of this field.
			 * If false, a preceeding non-input field's hot key (e.g. a label) matched.
			 */
			virtual void onMatchedHotKey(bool direct);

			/**
			 * Sets this check box'es state to checked or unchecked.
			 * First, all listeners are informed using onStateChanging(). They may
			 * veto the state change. Then, all listeners are informed using onStateChanged().
			 * @param checked Whether to set the state to checked.
			 * @return Whether the state change was performed. Any veto results in false.
			 * @see Listener::onStateChanging()
			 * @see Listener::onStateChanged()
			 */
			bool setChecked(bool checked);

			/**
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

			/**
			 * Sets this check box'es state to checked, unchecked, or tri-state.
			 * First, all listeners are informed using onStateChanging(). They may
			 * veto the state change. Then, all listeners are informed using onStateChanged().
			 * @param state The new state for the check box, CHECKED, UNCHECKED, or TRI_STATE.
			 * @return Whether the state change was performed. Any veto results in false.
			 * @see State
			 * @see Listener::onStateChanging()
			 * @see Listener::onStateChanged()
			 */
			bool setState(State state);

			/**
			 * Sets the text (the caption) of this field, if the field supports it.
			 * For multi line text fields, the string is split at new line characters ('\n').
			 * If supported by the field implementation, the text may contain a hot key character
			 * (preceded by an ampersand character). Pressing the hot key will set the focus to the field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 * @param text The text (the caption) of this field.
			 * @note This method uses the attribute returned by the getTextAttribute() method.
			 * If getTextAttribute() returns NULL, the field does not support text editing.
			 * setText does nothing, getText returns an empty string in this case.
			 * @see getTextAttribute()
			 */
			virtual void setText(const String & text);

			/**
			 * Sets whether this check box supports the tri-state (third, grayed state).
			 * @param triStateSupported Whether this check box supports the tri-state.
			 */
			void setTriStateSupported(bool triStateSupported);

			/**
			 * Toggles the state of the check box (regarding the triStateSupported property).
			 */
			void toggle();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
