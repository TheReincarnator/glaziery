/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_RADIOBOX_H
#define __GLAZIERY_RADIOBOX_H


#define GLAZIERY_RADIOBOX(variable, area, groupField, \
	text, checked, radioGroup) \
	RadioBox * variable; \
	if ((variable = new RadioBox) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	radioGroup->addRadioBox(variable); \
	variable->setText(text); \
	variable->setChecked(checked);

#define GLAZIERY_RADIOBOX_P(variable, area, groupField, px, py, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_RADIOBOX_S(variable, area, groupField, width, height, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_RADIOBOX_PS(variable, area, groupField, px, py, width, height, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_RADIOBOX_M(variable, area, groupField, minimumWidth, minimumHeight, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_RADIOBOX_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_RADIOBOX_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_RADIOBOX_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	text, checked, radioGroup) \
	GLAZIERY_RADIOBOX(variable, area, groupField, text, checked, radioGroup) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	class RadioBox : public InputField
	{
		public:

			/**
			 * The listener interface for radio box events.
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
					 * Called when the radio box'es state has been changed,
					 * either using the user interface or using the setChecked()
					 * or setState() methods.
					 * @param radioBox The radio box that has been changed.
					 * @see onStateChanging()
					 * @see RadioBox::setChecked()
					 * @see RadioBox::setState()
					 */
					virtual void onStateChanged(RadioBox * radioBox);
					// TODO: vetoable onStateChanging(RadioBox * radioBox, bool newState)
			};

		friend ArrayList<RadioBox>;
		friend RadioGroup;

		private:

			bool checked;

			String text;

			RadioGroup * radioGroup;

		protected:

			virtual ~RadioBox();

			int getPositionInGroup();

			String * getTextAttribute();

			void selectNextRadioBox();

			void selectPreviousRadioBox();

			void setRadioGroup(RadioGroup * radioGroup);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(RadioBox);
			#endif

		public:

			/**
			 * Creates a new radio box.
			 */
			RadioBox();

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

			RadioGroup * getRadioGroup();

			bool isChecked();

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
			 * Handles the event that the DOWN key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveDown(bool option1, bool option2);

			/**
			 * Handles the event that the LEFT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveLeft(bool option1, bool option2);

			/**
			 * Handles the event that the RIGHT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveRight(bool option1, bool option2);

			/**
			 * Handles the event that the UP key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveUp(bool option1, bool option2);

			void setChecked(bool checked);

			/**
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

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
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
