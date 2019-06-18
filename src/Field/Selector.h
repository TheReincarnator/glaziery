/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SELECTOR_H
#define __GLAZIERY_SELECTOR_H


#define GLAZIERY_SELECTOR(variable, area, groupField) \
	Selector * variable; \
	if ((variable = new Selector) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable);

#define GLAZIERY_SELECTOR_P(variable, area, groupField, px, py) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_SELECTOR_S(variable, area, groupField, width, height) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SELECTOR_PS(variable, area, groupField, px, py, width, height) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SELECTOR_M(variable, area, groupField, minimumWidth, minimumHeight) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SELECTOR_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SELECTOR_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SELECTOR_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_SELECTOR(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SELECTOR_TEXTOPTION(optionVariable, textElementVariable, selector, text, selected) \
	Selector::Option * optionVariable; \
	if ((optionVariable = new Selector::Option) == NULL) \
		throw EOUTOFMEMORY; \
	selector->appendOption(optionVariable); \
	TextElement * textElementVariable; \
	if ((textElementVariable = new TextElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(textElementVariable); \
	textElementVariable->setText(text); \
	if (selected) \
		selector->setSelection(optionVariable);

#define GLAZIERY_SELECTOR_IMAGEOPTION(optionVariable, imageElementVariable, selector, image, selected) \
	Selector::Option * optionVariable; \
	if ((optionVariable = new Selector::Option) == NULL) \
		throw EOUTOFMEMORY; \
	selector->appendOption(optionVariable); \
	ImageElement * imageElementVariable; \
	if ((imageElementVariable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(imageElementVariable); \
	imageElementVariable->setImageId(imageId); \
	if (selected) \
		selector->setSelection(optionVariable);

#define GLAZIERY_SELECTOR_IMAGETEXTOPTION(optionVariable, imageElementVariable, textElementVariable, \
	selector, image, text, selected) \
	Selector::Option * optionVariable; \
	if ((optionVariable = new Selector::Option) == NULL) \
		throw EOUTOFMEMORY; \
	selector->appendOption(optionVariable); \
	ImageElement * imageElementVariable; \
	if ((imageElementVariable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(imageElementVariable); \
	imageElementVariable->setImage(image); \
	TextElement * textElementVariable; \
	if ((textElementVariable = new TextElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(textElementVariable); \
	textElementVariable->setText(text); \
	if (selected) \
		selector->setSelection(optionVariable);

#define GLAZIERY_SELECTOR_IMAGEOPTION_I(optionVariable, imageElementVariable, selector, imageId, selected) \
	Selector::Option * optionVariable; \
	if ((optionVariable = new Selector::Option) == NULL) \
		throw EOUTOFMEMORY; \
	selector->appendOption(optionVariable); \
	ImageElement * imageElementVariable; \
	if ((imageElementVariable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(imageElementVariable); \
	imageElementVariable->setImageId(imageId); \
	if (selected) \
		selector->setSelection(optionVariable);

#define GLAZIERY_SELECTOR_IMAGETEXTOPTION_I(optionVariable, imageElementVariable, textElementVariable, \
	selector, imageId, text, selected) \
	Selector::Option * optionVariable; \
	if ((optionVariable = new Selector::Option) == NULL) \
		throw EOUTOFMEMORY; \
	selector->appendOption(optionVariable); \
	ImageElement * imageElementVariable; \
	if ((imageElementVariable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(imageElementVariable); \
	imageElementVariable->setImageId(imageId); \
	TextElement * textElementVariable; \
	if ((textElementVariable = new TextElement) == NULL) \
		throw EOUTOFMEMORY; \
	optionVariable->appendElement(textElementVariable); \
	textElementVariable->setText(text); \
	if (selected) \
		selector->setSelection(optionVariable);


namespace glaziery
{
	/**
	 * A selector is a field that allows the user to choose from a given set
	 * of possible options.
	 */
	class Selector : public InputField
	{
		public:

			/**
			 * The listener interface for selector events.
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

					virtual void onSelectionChanged(Selector * selector, int oldSelectionNo);
					virtual VetoMode onSelectionChanging(Selector * selector, int newSelectionNo);
			};

			class Option : public ListField::Cell
			{
				friend ArrayList<Option>;
				friend Selector;

				private:

					/**
					 * The selector this option belongs to.
					 */
					Selector * selector;

					/**
					 * Sets the selector this option belongs to.
					 * An exception is thrown, if the property has already been set.
					 * @param selector The selector this option belongs to.
					 */
					 void setSelector(Selector * selector);

				public:

					/**
					 * Creates a new option.
					 */
					Option();

					/**
					 * Appends a field element at the end of the cell.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void appendElement(FieldElement * element);

					/**
					 * Returns the field elements of this cell
					 * @return The content elements of this cell.
					 */
					const ArrayList<FieldElement> & getElements();

					/**
					 * Returns the selector this option belongs to.
					 * @return The selector this option belongs to.
					 */
					Selector * getSelector();

					/**
					 * Inserts a field element at a given position.
					 * @param position the 0-indexed column position to insert the element at.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void insertElement(int position, FieldElement * element);

					/**
					 * Returns whether this option is selected in its selector.
					 * @return Whether this option is selected.
					 */
					bool isSelected();

					/**
					 * Prepends a field element at the beginning of the cell.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void prependElement(FieldElement * element);

					/**
					 * Removes a field element at a given position.
					 * @param position the 0-indexed column position of the element to be removed.
					 */
					void removeElement(int position);

					/**
					 * Selects this option in its selector and unselects any previously selected option.
					 */
					void select();
			};

		private:

			class ListFieldRowsFieldListener : public ListField::RowsField::Listener
			{
				private:

					/**
					 * The selector to create the list field rows field listener for.
					 */
					Selector * selector;

					/**
					 * Writes back the selection from the popup field to the selector.
					 * @param target The target that received the event.
					 */
					void commitSelection(EventTarget * target);

				public:

					/**
					 * Creates a new list field rows field listener.
					 * @param selector The selector to create the list field rows field listener for.
					 */
					ListFieldRowsFieldListener(Selector * selector);

					/**
					 * Called when the component is clicked (usually using the left mouse button),
					 * called by the scrolling field with content-transformed coordinates.
					 * A click is raised after the mouse has been pressed and
					 * released, and no dragging operation has started.
					 * @param component The component that received the event.
					 * @param position The position where the click occurred
					 * relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onPress()
					 * @see onRelease()
					 */
					virtual void onContentClick(Component * component, Vector position,
						bool option1, bool option2);

					/**
					 * Called when the ENTER key is pressed on an event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onEnter(EventTarget * EventTarget, bool option1, bool option2);
			};

			class PopupListener : public Component::Listener
			{
				private:

					/**
					 * The selector to create the popup listener for.
					 */
					Selector * selector;

				public:

					/**
					 * Creates a new popup listener.
					 * @param selector The selector to create the list field listener for.
					 */
					PopupListener(Selector * selector);

					/**
					 * Called when the component is about to be destroyed (its memory
					 * is released). This method is called before the component is deleted,
					 * but its parent container references may have been released already.
					 * @param component The component that received the event.
					 */
					virtual void onDestroying(Component * component);
			};

		friend ListFieldRowsFieldListener;
		friend PopupListener;

		private:

			/**
			 * Whether this selector is auto-resizing its height according to the height of the content.
			 * Defaults to true.
			 * @see autoResize()
			 */
			bool autoResizing;

			ArrayList<Option> options;

			/**
			 * The popup list field that displays the options of the selector.
			 * It has no parent, it is passed to a popup window when the selector is clicked.
			 */
			ListField popupField;

			/**
			 * The maximum height (in pixels) of the popup window that displays
			 * the options of the selector. Defaults to 128.
			 */
			int popupHeight;

			/**
			 * The popup listener, if a popup is open, otherwise NULL.
			 */
			PopupListener * popupListener;

			/**
			 * The 0-indexed position of the selected option.
			 */
			int selectionNo;

		protected:

			virtual ~Selector();

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
			 * Handles the event that the END key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToEnd(bool option1, bool option2);

			/**
			 * Handles the event that the START (HOME, POS1, ...) key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToStart(bool option1, bool option2);

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

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Selector);
			#endif

		public:

			/**
			 * This value indicates that there is no selection.
			 */
			static const int Selector::NONE;

			Selector();

			void appendOption(Option * option);

			/**
			 * Resizes once the selector's height according to the height of the content.
			 * Useful if auto resizing is disabled.
			 * @see isAutoResizing()
			 * @see setAutoResizing()
			 */
			void autoResize();

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

			const ArrayList<Option> & getOptions();

			/**
			 * Returns the popup list field that displays the options of the selector.
			 * It has no parent, it is passed to a popup window when the selector is clicked.
			 * @note You normally do not need to operate on this field.
			 * E.g. it is useful for visual settings made by the skin's setup method.
			 * @return The popup list field that displays the options of the selector.
			 */
			ListField * getPopupField();

			/**
			 * Returns the maximum height (in pixels) of the popup window that displays
			 * the options of the selector. Defaults to 128.
			 * @return The maximum height (in pixels) of the popup window.
			 */
			int getPopupHeight();

			/**
			 * Returns the currently selected option or NULL, if no option is selected.
			 * @return The currently selected option or NULL.
			 */
			Option * getSelection();

			/**
			 * Returns the 0-indexed position of the currently selected option or NONE,
			 * if no option is selected.
			 * @return The 0-indexed position of the currently selected option or NONE.
			 */
			int getSelectionNo();

			void insertOption(int position, Option * option);

			/**
			 * Returns whether this selector's height is auto-resizing its height according
			 * to the height of the content. Defaults to true.
			 * @return Whether this selector's height is auto-resizing its height.
			 * @see autoResize()
			 */
			bool isAutoResizing();

			/**
			 * Notifies all component listeners about the destruction
			 * of the component, but does not delete it nor adds it to
			 * the list of disposable components.
			 */
			virtual void onDestroying();

			/**
			 * Handles the event that the event target is pressed down
			 * (usually using the left mouse button).
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onHold()
			 * @see onRelease()
			 */
			virtual void onPress(Vector position, bool option1, bool option2);

			void prependOption(Option * option);

			void removeOption(int position);

			void removeOptions();

			/**
			 * Sets whether this selector's height is auto-resizing its height according
			 * to the height of the content. Defaults to true.
			 * @param autoResizing Whether this selector's height is auto-resizing its height.
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
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

			/**
			 * Sets the maximum height (in pixels) of the popup window that displays
			 * the options of the selector. Defaults to 128.
			 * @param popupHeight The maximum height (in pixels) of the popup window.
			 */
			void setPopupHeight(int popupHeight);

			void setSelection(Option * selection);

			void setSelection(int selectionNo);

			void setSelectionByModel(void * model);

			/**
			 * If a selector popup is open, it is destroyed.
			 * Otherwise a new popup window is created, using the popup field,
			 * to choose the selected option.
			 */
			void togglePopup();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * For tutorials: Creates a pointer effect that moves the pointer to the selector,
			 * clicks it, clicks an option of the list, and waits until the effect has ended.
			 * @param selectionNo The 0-indexed number of the option to be selected.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialSelect(int selectionNo, long time = -1);

			/**
			 * Validates whether an option has been chosen.
			 * If not, shows a balloon popup with an error message.
			 * @param nonNullModelRequired If set to true, the model must be non-NULL, too.
			 * This is useful if you have a "please select" option with NULL model.
			 * @return Whether the field is valid.
			 */
			bool validateRequired(bool nonNullModelRequired = false);
	};
}


#endif
