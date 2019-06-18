/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TEXTFIELD_H
#define __GLAZIERY_TEXTFIELD_H


#define GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	TextField * variable; \
	if ((variable = new TextField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setMultiLine(multiLine);

#define GLAZIERY_TEXTFIELD_P(variable, area, groupField, px, py, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_TEXTFIELD_S(variable, area, groupField, width, height, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TEXTFIELD_PS(variable, area, groupField, px, py, width, height, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TEXTFIELD_M(variable, area, groupField, minimumWidth, minimumHeight, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_TEXTFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_TEXTFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TEXTFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, multiLine) \
	GLAZIERY_TEXTFIELD(variable, area, groupField, multiLine) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	/**
	 * A text field is an input field that enables the user to enter some text.
	 * The text field can be setup to support a single line or multi line texts.
	 */
	class TextField : public ScrollingField
	{
		protected:

			/**
			 * The column of the cursor, i.e. the character position within the row.
			 */
			int cursorColumn;

			/**
			 * The row of the cursor, i.e. the line number.
			 */
			int cursorRow;

			/**
			 * Cursor last x pixel position for veritical movement.
			 */
			int cursorX;

			/**
			 * The text content of the field. For single line text fields, the line array
			 * contains only one element. For multi line text fields, the line array
			 * contains the lines of the field.
			 */
			ArrayList<String> lines;

			/**
			 * The number of pixels of space below each line. Defaults to 0.
			 */
			int lineSpacingBottom;

			/**
			 * The number of pixels of space above each line. Defaults to 0.
			 */
			int lineSpacingTop;

			/**
			 * Whether to display all characters entered in the field e.g. as asterisks,
			 * providing a password entering text field.
			 */
			bool masked;

			/**
			 * Whether the text field supports the enter key,
			 * producing multiple lines within the field. Defaults to false.
			 * If multi line is disabled, the enter key is treated normally,
			 * e.g. pressing the default button.
			 */
			bool multiLine;

			/**
			 * The column of the selection start, i.e. the character position within the row.
			 * If a selection exists, it lasts from the selection start to the cursor position.
			 * If no selection exists, the selection start equals the cursor position.
			 */
			int selectionStartColumn;

			/**
			 * The row of the selection start, i.e. the line number where the selection starts.
			 * If a selection exists, it lasts from the selection start to the cursor position.
			 * If no selection exists, the selection start equals the cursor position.
			 */
			int selectionStartRow;

			virtual ~TextField();

			/**
			 * Checks that the cursor and selection lie within the text lines.
			 * Calculates the new cursor x pixel position according to the cursor position.
			 */
			void constrainCursor();

			/**
			 * Draws the content of the scrolling field to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation. This context is already translated
			 * for the current scroll position and scroll field size, so you should
			 * draw as if there is no scrolling field, and use the field size you also
			 * return in getContentSize().
			 * @see Desktop::getSkin()
			 */
			virtual void drawContent(DrawingContext & context);

			/**
			 * Returns whether the cursor is currently on a word start.
			 * @return Whether the cursor is currently on a word start.
			 */
			bool isOnWordStart();

			/**
			 * Handles a context menu click for 'cut'.
			 * @param item The selected item.
			 */
			static void onContextMenuCut(Menu::Item * item);

			/**
			 * Handles a context menu click for 'copy'.
			 * @param item The selected item.
			 */
			static void onContextMenuCopy(Menu::Item * item);

			/**
			 * Handles a context menu click for 'copy'.
			 * @param item The selected item.
			 */
			static void onContextMenuDelete(Menu::Item * item);

			/**
			 * Handles a context menu click for 'paste'.
			 * @param item The selected item.
			 */
			static void onContextMenuPaste(Menu::Item * item);

			/**
			 * Handles a context menu click for 'select all'.
			 * @param item The selected item.
			 */
			static void onContextMenuSelectAll(Menu::Item * item);

			/**
			 * TODO
			 */
			bool moveDownInternal(bool option1, bool option2, int count);

			/**
			 * TODO
			 */
			bool moveUpInternal(bool option1, bool option2, int count);

			/**
			 * Sets the cursor column from the pixel position in cursorX.
			 */
			void setCursorColumnFromX();

			/**
			 * Splits a given text into lines, creating new String objects for each line,
			 * and adding the lines to a given ArrayList. If multiline is false,
			 * adds the entire text as a cloned String to the ArrayList.
			 * @param lines The ArrayList to add the String clones to.
			 * @param text The text to be split.
			 */
			void splitTextIntoLines(ArrayList<String> & lines, const String & text);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(TextField);
			#endif

		public:

			/**
			 * Creates a new text field.
			 */
			TextField();

			/**
			 * Appends a line of text at the end of this multi-line text field.
			 * If the field is not multi-line, the call is ignored.
			 * @param line The line to be appended.
			 */
			void appendLine(const String & line);

			/**
			 * Deletes all the lines in this text field.
			 */
			virtual void clear();

			/**
			 * Returns whether the ENTER key is entirely consumed by this input field.
			 * If not, it may press the field area's default button.
			 * Defaults to false.
			 * @return Whether the ENTER key is entirely consumed by this input field.
			 */
			virtual bool consumingEnter();

			/**
			 * Adds items for a context-clock menu. If you leave the menu empty, none appears.
			 * This is the default. Override this method to define your own menu items.
			 * Super-call to include the default menu items of the overridden parent.
			 * @param menu The context menu.
			 * @param clickPosition The click position relative to the top-left corner of the component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return The new context menu to show, or NULL if no menu shall exists.
			 */
			virtual void addContextMenuItems(Menu * menu, Vector clickPosition, bool option1, bool option2);

			/**
			 * If a selection exists, the content is deleted and no selection persists.
			 * If not, nothing happens.
			 * @return Whether the operation succeeded. false indicates a veto.
			 */
			virtual bool deleteSelection();

			/**
			 * Ensures that the cursor is entirely visible by setting the scroll position.
			 * If the cursor cannot be shown entirely, then the top-left corner is ensured to be shown.
			 */
			void ensureCursorVisible();

			/**
			 * Returns the column of the cursor, i.e. the character position within the row.
			 * @return The column of the cursor.
			 */
			int getCursorColumn();

			/**
			 * Returns the row of the cursor, i.e. the line number.
			 * @return The row of the cursor.
			 */
			int getCursorRow();

			/**
			 * Returns the text content of the field as a line array.
			 * For single line text fields, the array contains only one element.
			 * For multi line text fields, the array contains the lines of the field.
			 * @return The text content of the field as a line array.
			 */
			const ArrayList<String> & getLines();

			/**
			 * Returns the number of pixels of space below each line. Defaults to 0.
			 * @return The number of pixels of space below each line.
			 */
			int getLineSpacingBottom();

			/**
			 * Returns the number of pixels of space above each line. Defaults to 0.
			 * @return The number of pixels of space above each line.
			 */
			int getLineSpacingTop();

			/**
			 * Returns the number of lines entirely visible in the content view.
			 * This is also the amount of lines to move when moving per page.
			 * @return The number of lines per page.
			 */
			int getPageLineCount();

			/**
			 * Returns the column of the selection start, i.e. the character position within the row.
			 * If a selection exists, it lasts from the selection start to the cursor position.
			 * If no selection exists, the selection start equals the cursor position.
			 * @return The column of the selection start.
			 */
			int getSelectionStartColumn();

			/**
			 * Returns the row of the selection start, i.e. the line number where the selection starts.
			 * If a selection exists, it lasts from the selection start to the cursor position.
			 * If no selection exists, the selection start equals the cursor position.
			 * @return The row of the selection start.
			 */
			int getSelectionStartRow();

			/**
			 * Returns the selected text of the field as a joined string.
			 * For multi line text fields, the lines are joined using the new line character ('\n').
			 * @return The selected text of the field as a joined string.
			 */
			String getSelectionText();

			/**
			 * Returns the text (the caption) of this field, if the field supports it.
			 * For multi line text fields, the string is joined using new line characters ('\n').
			 * If supported by the field implementation, the text may contain a hot key character
			 * (preceded by an ampersand character). Pressing the hot key will set the focus to the field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 * @return The text (the caption) of this field.
			 * @note This method uses the attribute returned by the getTextAttribute() method.
			 * If getTextAttribute() returns NULL, the field does not support text editing.
			 * setText does nothing, getText returns an empty string in this case.
			 * @see getTextAttribute()
			 */
			virtual String getText();

			/**
			 * Returns the double value of the trimmed text of this field.
			 * If the text cannot be converted to a double, the method returns 0.0.
			 * @return The double value of this field.
			 */
			double getTextAsDouble();

			/**
			 * Returns the float value of the trimmed text of this field.
			 * If the text cannot be converted to a float, the method returns 0.0.
			 * @return The float value of this field.
			 */
			float getTextAsFloat();

			/**
			 * Returns the int value of the trimmed text of this field.
			 * If the text cannot be converted to an int, the method returns 0.
			 * @return The int value of this field.
			 */
			int getTextAsInt();

			/**
			 * Returns the long value of the trimmed text of this field.
			 * If the text cannot be converted to a long, the method returns 0.
			 * @return The long value of this field.
			 */
			long getTextAsLong();

			/**
			 * Returns the trimmed text of this field.
			 * For multi line text fields, the string is joined using new line characters ('\n').
			 * Trimming removes spaces, tabs, and newlines at the beginning of the (first) line
			 * and it removes spaces, tabs, and newlines at the end of the (last) line.
			 * @return The trimmed text of this field.
			 */
			String getTextTrimmed();

			/**
			 * Returns whether the text field currently has a selection,
			 * i.e. the cursor position and the selection start differ.
			 * @return Whether the text field currently has a selection.
			 */
			bool hasSelection();

			/**
			 * Insert a line of text at a given position of this multi-line text field.
			 * If the field is not multi-line, the call is ignored.
			 * @param rowNo The 0-indexed position of the new line.
			 * @param line The line to be inserted.
			 */
			virtual void insertLine(int rowNo, const String & line);

			/**
			 * Returns whether the text field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the text field displays a horizontal scroller.
			 */
			virtual bool isHorizontallyScrollable();

			/**
			 * Returns whether to display all characters entered in the field e.g. as asterisks,
			 * providing a password entering text field.
			 * @return Whether this field is masked.
			 */
			bool isMasked();

			/**
			 * Returns whether the text field supports the enter key,
			 * producing multiple lines within the field. Defaults to false.
			 * @return Whether the text field supports the enter key.
			 */
			bool isMultiLine();

			/**
			 * Returns whether the text field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the text field displays a horizontal scroller.
			 */
			virtual bool isVerticallyScrollable();

			/**
			 * Handles the event that the BACKSPACE key (backward delete)
			 * is pressed on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onBackSpace();

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
			 * Handles the event that, after the event target is pressed down on the content
			 * (usually using the left mouse button), the pointer is not moved,
			 * but the button has not yet been released. onContentHold() is called in the time
			 * between onContentPress() and onContentRelease(). Glaziery provides the time between
			 * the last call of onContentHold() and this one. It expects that the onHold
			 * implementation issues in its return value the hold time not consumed.
			 * If 0 is returned, all the time has been consumed and the holdTime reflects
			 * only the time between two consecutive onContentHold() calls each time.
			 * If a positive value is returned, this value is added to the next holdTime value.
			 * This helps to implement operations that have to be performed after
			 * some time has elapsed, while not losing remaining milli seconds.
			 * The following code snippet illustrates this operation:
			 * <code><pre>
			 * long MyClass::onContentHold(Vector position, long holdTime)
			 * {
			 * 	while (holdTime >= 100)
			 * 	{
			 * 		performSomeOperation();
			 * 		holdTime -= 100;
			 * 	}
			 * 	return holdTime;
			 * }
			 * </pre></code>
			 * @param position The current position of the pointer within the content,
			 * relative to the upper-left corner of the content.
			 * @param holdTime The hold time between the last call of onContentHold() and this one,
			 * plus the hold time not consumed in the last call of onContentHold(). The time's
			 * unit is milli seconds.
			 * @return The hold time this method has not consumed in milli seconds.
			 * @see onContentPress()
			 * @see onContentRelease()
			 */
			virtual long onContentHold(Vector position, long holdTime);

			/**
			 * Called by the scrolling field to notify the content when the field
			 * is pressed down (usually using the left mouse button).
			 * @param position The position within the content where the press
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onContentPress()
			 * @see onContentRelease()
			 */
			virtual void onContentPress(Vector position, bool option1, bool option2);

			/**
			 * Handles a clipboard copy event on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCopy();

			/**
			 * Handles a clipboard cut event on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCut();

			/**
			 * Handles the event that the DELETE key (forward delete)
			 * is pressed on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onDelete();

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
			 * First notifies the listeners of this component of a focus gain,
			 * then its focused child, and so on (calling the listeners'
			 * onGotFocus() method).
			 * @param byParent If the parent got focus and this is propagated to the child.
			 * @see Listener::onGotFocus()
			 */
			virtual void onGotFocus(bool byParent);

			/**
			 * Handles the event that a character has been pressed and should be treated as hot key.
			 * Hot keys represent special characters e.g. to easily access fields
			 * (indicated by underlined characters).
			 * @param character The hot key character pressed.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the field has consumed the event.
			 */
			virtual bool onHotKeyByCharacter(char character, bool option1, bool option2);

			/**
			 * First notifies the listeners of the deepest grand-child of a focus loss,
			 * then its parent, and so on, until this component is reached,
			 * which is then notified (calling the listeners' onLostFocus() method).
			 * @see Listener::onLostFocus()
			 */
			virtual void onLostFocus();

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

			/**
			 * Handles the event that the PAGE DOWN key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageDown(bool option1, bool option2);

			/**
			 * Handles the event that the PAGE UP key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageUp(bool option1, bool option2);

			/**
			 * Handles a clipboard paste event on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPaste();

			/**
			 * Handles the event that the user requested to select all
			 * (e.g. by pressing Ctrl+A on Windows systems).
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onSelectAll();

			/**
			 * Prepends a line of text at the beginning of this multi-line text field.
			 * If the field is not multi-line, the call is ignored.
			 * @param line The line to be prepended.
			 */
			void prependLine(const String & line);

			/**
			 * Removes a line of text from this multi-line text field.
			 * If the field is not multi-line, the call is ignored.
			 * @param rowNo The 0-indexed position of the line.
			 */
			virtual void removeLine(int rowNo);

			/**
			 * Selects the entire text, setting the selection start and cursor position.
			 * @see getCursorColumn()
			 * @see getCursorRow()
			 * @see getSelectionStartColumn()
			 * @see getSelectionStartRow()
			 */
			void selectAll();

			/**
			 * Selects a number of characters starting at a given position,
			 * if needed spanning over several rows. The cursor is set to the
			 * end of the selection.
			 * @param startRow The row of the selection start, i.e. the line number.
			 * @param startColumn The column of the selection start,
			 * i.e. the character position within the row.
			 * @param characters The number of characters to select. May be 0.
			 */
			void selectCharacters(int startRow, int startColumn, int characters);

			/**
			 * Selects a number of entire rows starting at a given row
			 * The cursor is set to the end of the selection.
			 * @param startRow The row of the selection start, i.e. the line number.
			 * @param rows The number of rows to select. May be 0.
			 */
			void selectRows(int startRow, int rows);

			/**
			 * Sets the cursor position and unselects an existing selection.
			 * @param row The row of the cursor, i.e. the line number.
			 * @param column The column of the cursor, i.e. the character position within the row.
			 */
			void setCursorPosition(int row, int column);

			/**
			 * Sets the text content of the field as a line array.
			 * For single line text fields, the array may contain only one element.
			 * For multi line text fields, the array contains the lines of the field.
			 * @param lines The text content of the field as a line array.
			 */
			virtual void setLines(const ArrayList<String> & lines);

			/**
			 * Sets the number of pixels of space below each line. Defaults to 0.
			 * @param lineSpacingBottom The number of pixels of space below each line.
			 */
			void setLineSpacingBottom(int lineSpacingBottom);

			/**
			 * Sets the number of pixels of space above each line. Defaults to 0.
			 * @param lineSpacingTop The number of pixels of space above each line.
			 */
			void setLineSpacingTop(int lineSpacingTop);

			/**
			 * Sets whether to display all characters entered in the field e.g. as asterisks,
			 * providing a password entering text field.
			 * @param masked Whether this field is masked.
			 */
			void setMasked(bool masked);

			/**
			 * Sets whether the text field supports the enter key,
			 * producing multiple lines within the field. Defaults to false.
			 * @param multiLine Whether the text field supports the enter key.
			 */
			virtual void setMultiLine(bool multiLine);

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
			 * Shows a balloon popup for this field, containing a given text.
			 * @param text The text to show.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(const String & text);

			/**
			 * Shows a balloon popup for this text field, containing a given text.
			 * The popup points at a given character position.
			 * @param row The 0-indexed row number.
			 * @param column The 0-indexed column number.
			 * @param text The text to show.
			 * @param includeSpacing Include the top and bottom spacing to point to.
			 * false points the popup to the text line, only.
			 * @param positionCursor Whether to set the cursor position to the balloon position, too.
			 * Defaults to false.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(int row, int column, const String & text,
				bool includeSpacing, bool positionCursor = false);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * Calculates the size of the content, as if there was no scrolling field
			 * and the field must display its entire content at once, and puts the
			 * value into the contentSize attributes.
			 * The scrolling field automatically adjusts the scrollers according to
			 * this value.
			 * @note This method should be called when the content changes in a way
			 * that might change the its size.
			 * @see contentSize
			 */
			virtual void updateContentSize();

			/**
			 * Validates whether the non-empty text has the same text as another one.
			 * This is handy for password verification fields.
			 * If the field is not valid the method shows a balloon popup with an error message.
			 * @note The text is only checked if it the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @param other The other text field.
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateEqual(TextField * other);

			/**
			 * Validates whether the non-empty text is a valid float value.
			 * This includes int values, as well.
			 * If the field is not valid the method shows a balloon popup with an error message.
			 * @note The text is only checked if it the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateFloat();

			/**
			 * Validates whether the non-empty text is a valid float value
			 * within a given value range. This includes int values, as well.
			 * If the field is not valid the method shows a balloon popup with an error message.
			 * @param from The minimum valid float value.
			 * @param to The maximum valid float value.
			 * @note The text is only checked if it the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateFloatRange(double from, double to);

			/**
			 * Validates whether the non-empty text is a valid int value.
			 * If the field is not valid the method shows a balloon popup with an error message.
			 * @note The text is only checked if it the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateInt();

			/**
			 * Validates whether the non-empty text is a valid int value
			 * within a given value range.
			 * If the field is not valid the method shows a balloon popup with an error message.
			 * @param from The minimum valid int value.
			 * @param to The maximum valid int value.
			 * @note The text is only checked if it the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateIntRange(long from, long to);

			/**
			 * Validates whether this field does not exceed a given length.
			 * If it does, shows a balloon popup with an error message.
			 * @param maxLength The maximum valid length. Newlines count as one.
			 * @param trim Whether to trim spaces, tabs, and newlines at the beginning
			 * of the (first) line, as well as at the end of the (last) line,
			 * when counting the number of characters in this field.
			 * @return Whether the field is valid.
			 */
			bool validateMaxLength(int maxLength, bool trim = false);

			/**
			 * Validates whether this non-empty field has at least a given number of characters.
			 * If not, shows a balloon popup with an error message.
			 * @param minLength The minimum length of the string to be valid.
			 * @param trim Whether to trim spaces, tabs, and newlines at the beginning
			 * of the (first) line, as well as at the end of the (last) line,
			 * when counting the number of characters in this field.
			 * @note The minimum length is only checked if the field is not empty.
			 * To test empty text fields, use validateRequired()
			 * @return Whether the field is valid.
			 * @see validateRequired()
			 */
			bool validateMinLength(int minLength, bool trim = false);

			/**
			 * Validates whether this field has been filled out.
			 * If not, shows a balloon popup with an error message.
			 * @return Whether the field is valid.
			 */
			bool validateRequired(bool trim = false);
	};
}


#endif
