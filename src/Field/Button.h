/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_BUTTON_H
#define __GLAZIERY_BUTTON_H


#define GLAZIERY_BUTTON(variable, area, groupField, \
	text, image) \
	Button * variable; \
	if ((variable = new Button) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setText(text); \
	if (image != NULL) \
		variable->setImage(image);

#define GLAZIERY_BUTTON_P(variable, area, groupField, px, py, autoResizing, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->moveTo(Vector((px), (py)));

#define GLAZIERY_BUTTON_S(variable, area, groupField, width, height, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_PS(variable, area, groupField, px, py, width, height, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_M(variable, area, groupField, minimumWidth, minimumHeight, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_BUTTON_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_BUTTON_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	text, image) \
	GLAZIERY_BUTTON(variable, area, groupField, text, image) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_I(variable, area, groupField, \
	text, imageId) \
	Button * variable; \
	if ((variable = new Button) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setText(text); \
	variable->setImageId(imageId);

#define GLAZIERY_BUTTON_IP(variable, area, groupField, px, py, autoResizing, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->moveTo(Vector((px), (py)));

#define GLAZIERY_BUTTON_IS(variable, area, groupField, width, height, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_IPS(variable, area, groupField, px, py, width, height, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_IM(variable, area, groupField, minimumWidth, minimumHeight, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_BUTTON_IPM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_BUTTON_ISM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_BUTTON_IPSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	text, imageId) \
	GLAZIERY_BUTTON_I(variable, area, groupField, text, imageId) \
	variable->setAutoResizing(false); \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	class Button : public InputField
	{
		public:

			/**
			 * The listener interface for button events.
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
					 * Called when the button has been pressed, either using the
					 * user interface, or using the press() method.
					 * @param button The button that has been pressed.
					 * @see Button::press()
					 */
					virtual void onPressed(Button * button);
			};

			/**
			 * The listener implementation for simple function calls for onPressed().
			 */
			class PressedFunctionListener : public Listener
			{
				private:

					/**
					 * The function to be invoked, when the button has been pressed.
					 */
					void (*function)(Button * button);

				public:

					/**
					 * Creates a new pressed function listener.
					 * @param function The function to be invoked, when the button has been pressed.
					 */
					PressedFunctionListener(void (*function)(Button * button));

					/**
					 * Called when the button has been pressed, either using the
					 * user interface, or using the press() method.
					 * @param button The button that has been pressed.
					 * @see Button::press()
					 */
					virtual void onPressed(Button * button);
			};

		friend ArrayList<Button>;
		friend HashMap<Button>;
		friend InputField;

		private:

			/**
			 * Whether this button is auto-resizing according to the size of the content. Defaults to true.
			 * @see autoResize()
			 */
			bool autoResizing;

			/**
			 * Whether this button dismisses the dialog (if the button's window is a dialog).
			 * Defaults to false.
			 * @see Dialog::getPressedButton()
			 */
			bool dialogButton;

			/**
			 * The image of the button. May be NULL, displaying no image.
			 */
			Image * image;

			/**
			 * The text (the caption) of this field.
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 */
			String text;

		protected:

			/**
			 * Destroys the new button.
			 */
			virtual ~Button();

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
				DECLARE_DYNAMIC(Button);
			#endif

		public:

			/**
			 * Creates a new button.
			 */
			Button();

			/**
			 * Adds a button listener for the onPressed event and attaches it to a function.
			 * This is a helper to reduce the need for listener classes on a per-field basis.
			 * @param function The function to be invoked, when the button has been pressed.
			 * @return The automatically created and appended listener.
			 */
			PressedFunctionListener * addPressedFunction(void (*function)(Button * button));

			/**
			 * Resizes the button once according to the size of the content.
			 * Useful if auto resizing is disabled.
			 * @see isAutoResizing()
			 * @see setAutoResizing()
			 */
			void autoResize();

			/**
			 * Returns whether the ENTER key is entirely consumed by this input field.
			 * If not, it may press the field area's default button.
			 * Defaults to false.
			 * @return Whether the ENTER key is entirely consumed by this input field.
			 */
			virtual bool consumingEnter();

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
			 * Returns the image of the button. May be NULL, displaying no image.
			 * @return The image of the button or NULL.
			 */
			Image * getImage();

			/**
			 * Returns whether this button is auto-resizing according to the size of the content. Defaults to true.
			 * @return Whether this button is auto-resizing according to the size of the content.
			 * @see autoResize()
			 */
			bool isAutoResizing();

			/**
			 * Returns whether this button dismisses the dialog (if the button's window is a dialog).
			 * Defaults to false.
			 * @return Whether this button is a dialog button.
			 * @see Dialog::getPressedButton()
			 */
			bool isDialogButton();

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
			 * Presses the button, notifying all listeners. Modal buttons also end
			 * the modal loop this way.
			 */
			void press();

			/**
			 * Sets whether this button is auto-resizing according to the size of the content. Defaults to true.
			 * @param autoResizing Whether this button is auto-resizing according to the size of the content.
			 * @see autoResize()
			 */
			void setAutoResizing(bool autoResizing);

			/**
			 * Sets whether to use all-capital characters when drawing the field's text.
			 * Does not change the text itself. Defaults to false.
			 * @param caps Whether to use all-capital characters when drawing the field's text.
			 */
			virtual void setCaps(bool caps);

			/**
			 * Sets whether this button dismisses the dialog (if the button's window is a dialog).
			 * Defaults to false.
			 * @param dialogButton Whether this button is a dialog button.
			 * @see Dialog::getPressedButton()
			 */
			void setDialogButton(bool dialogButton);

			/**
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

			/**
			 * Sets the image of the button. May be NULL, displaying no image.
			 * @param image The image to be set or NULL.
			 */
			void setImage(Image * image);

			/**
			 * Sets the image of the button by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const String & imageId);

			/**
			 * Sets the image of the button by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const char * imageId);

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
