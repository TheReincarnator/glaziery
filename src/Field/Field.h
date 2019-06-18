/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FIELD_H
#define __GLAZIERY_FIELD_H


namespace glaziery
{
	/**
	 * A field is an element of a field area that allows arbitrary positioning
	 * on the area and e.g. serves as container for user input, can have focus,
	 * can be focused using the tab key, and so on. Examples for fields are
	 * text input fields, check boxes, labels, buttons, progress bars, etc.
	 * @warning If you implement a class that derives from Field,
	 * remember to call the window's invalidate() method in every method
	 * that changes something concerning the graphical representation
	 * (in any skin, if applicable).
	 */
	class Field : public Component
	{
		public:

			/**
			 * The way check box or radio box attachments should work.
			 * Attaching a field to a check box or radio box makes
			 * the field active depending on the box'es check state.
			 * @see attachTo()
			 */
			enum AttachmentType
			{
				/**
				 * The field is active if and only if at least one
				 * attached check box or radio box is checked.
				 */
				ACTIVE_IF_ANY_CHECKED,

				/**
				 * The field is active if and only if at least one
				 * attached check box or radio box is not checked.
				 */
				ACTIVE_IF_ANY_UNCHECKED,

				/**
				 * The field is active if and only if all
				 * attached check box or radio boxes are checked.
				 */
				ACTIVE_IF_ALL_CHECKED,

				/**
				 * The field is active if and only if all
				 * attached check box or radio boxes are not checked.
				 */
				ACTIVE_IF_ALL_UNCHECKED,
			};

			/**
			 * The listener interface for field events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public Component::Listener
			{
				friend ArrayList<Listener>;

				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					/**
					 * Called when the text of a field has been changed.
					 * @param field The field that received the event.
					 * @see onTextChanging()
					 */
					virtual void onTextChanged(Field * field);

					/**
					 * Called when the text of a text element is about to be changed.
					 * The listener may cancel this event by vetoing it.
					 * @param field The field that received the event.
					 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
					 * @see VetoMode
					 * @see onTextChanged()
					 */
					virtual VetoMode onTextChanging(Field * field);
			};

		friend ArrayList<Field>;
		friend AttachmentCheckBoxListener;
		friend AttachmentRadioBoxListener;
		friend FieldsArea;
		friend GroupField;

		private:

			/**
			 * For input fields: Whether this input field can get focus and therefore
			 * can be edited or selected. Not active fields are usually grayed out.
			 * For non-input fields: Whether this field should be displayed normally.
			 * Like input fields, not active fields are usually grayed out.
			 */
			bool active;

			/**
			 * The way check box or radio box attachments should work.
			 * Attaching a field to a check box or radio box makes
			 * the field active depending on the box'es check state.
			 * @see attachTo()
			 */
			AttachmentType attachmentType;

			/**
			 * If this field is attached to a check box or radio box,
			 * whether this field should get focus when it gets active.
			 * Defaults to false.
			 * @see attachTo()
			 */
			bool attachmentFocussing;

			/**
			 * Whether to use all-capital characters when drawing the field's text.
			 * Does not change the text itself. Defaults to false.
			 */
			bool caps;

			/**
			 * The listeners for check boxes or radio boxes this field is currently attached to.
			 * Depending on the attachment type, the active state is set if any resp. all
			 * of the input fields are checked resp. unchecked.
			 */
			ArrayList<Listener> attachmentCheckListeners;

			/**
			 * The layout data for this field. Defaults to a FieldLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @see FieldLayoutData
			 */
			FieldLayoutData * fieldLayoutData;

			/**
			 * The font to be used for texts included in the field.
			 */
			Font * font;

			/**
			 * The group field this field is contained in, or NULL, if the field is top-level.
			 * Defaults to NULL.
			 * @note Group fields are used for layouting purposes,
			 * as they declare a bounding box for the layout process.
			 * @see GroupField
			 */
			GroupField * groupField;

			/**
			 * Whether click operations on this field behave like clicking the title bar,
			 * if supported by the skin.
			 * Defaults to false for input fields and some special fields, true for regular fields.
			 */
			bool likeTitle;

			/**
			 * The deferred text.
			 * @see Desktop::executeDeferrals()
			 */
			String textDeferred;

			InputField * getAttachedCheckable(int no);

			/**
			 * Updates the active state when an attached check box or radio box'es check state changes.
			 */
			void setActiveFromAttachments();

		protected:

			/**
			 * Creates a new field.
			 */
			Field();

			/**
			 * Destroys the field.
			 */
			virtual ~Field();

			/**
			 * Adds a hot key for this field. Note that the Field class provides a method
			 * to set a field's text. This method also calls addHotKeys(), so in this case,
			 * you don't need to.
			 * @param hotKey The hot key character to be set.
			 * @see setText()
			 */
			void addHotKey(char hotKey);

			/**
			 * Executes all deferred operations in this event target.
			 * Override, but super-call if you provide any deferred operations.
			 * @see Desktop::executeDeferrals()
			 */
			virtual void executeDeferrals();

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

			/**
			 * Moves the component within its parent and handles listeners.
			 * This method is called internally by the move overload,
			 * and by parents moveing a child.
			 * @param position The new position relative to its parent.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 * @return Whether the component was actually moved.
			 * @see moveTo()
			 * @see onChildMoved()
			 * @see onChildResized()
			 */
			virtual bool moveInternal(Vector position, bool notifyParent);

			/**
			 * Notifies all field listeners that the text of the field has changed.
			 */
			void notifyListenersTextChanged();

			/**
			 * Notifies all field listeners that the text of the field is about to change.
			 * @param Whether any listener has vetoed.
			 */
			bool notifyListenersTextChanging();

			/**
			 * Removes a hot key from this field. Use this method when implementing
			 * a field that has a hot key text, e.g. a check box, and call it when
			 * deleting the object. Note that the Field class provides a method to set a
			 * field's text. This method also calls removeHotKey(), so in this case,
			 * you don't need to.
			 * @param hotKey The hot key character to be removed.
			 * It is safe to provide 0: In this case, the method does nothing.
			 */
			void removeHotKey(unsigned char hotKey);

			/**
			 * Resizes the component and handles listeners.
			 * This method is called internally by the resize overload,
			 * and by parents resizing a child.
			 * @param size The new size.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 * @return Whether the component was actually resized.
			 * @see resize()
			 * @see onChildMoved()
			 * @see onChildResized()
			 */
			virtual bool resizeInternal(Vector size, bool notifyParent);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Field);
			#endif

		public:

			/**
			 * Attaches the active state of this field to the checked state of a given input field,
			 * for instance, a check box or a radio box.
			 * In the simplest way, this field is set active if and only
			 * if the attached check box or radio box is checked.
			 * But depending on the attachment type, the active state is set if any resp. all
			 * of the input fields are checked resp. not checked.
			 * Additionally, if attachment focussing is set to true, if this field gets active,
			 * it also gets focus.
			 * @param checkable The check box or radio box to listen to.
			 * @see attachmentType
			 * @see attachmentFocussing
			 */
			void attachTo(InputField * checkable);

			/**
			 * Removes the component from the Glaziery system to destroy it.
			 * First performs operations before the component is destroyed,
			 * then adds it to the Desktop's list of disposables.
			 * @note The component is added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @note This is an ungraceful removal, without user interaction
			 * and close notifications, but with destroy noficiation.
			 * E.g. for windows, use Window::close() to close normally.
			 */
			virtual void destroy();

			/**
			 * Detaches this field from a previously attached-to input field (check box or radio box).
			 * @param checkable The check box or radio box to stop listening to.
			 * @see attachTo()
			 */
			void detachFrom(InputField * checkable);

			/**
			 * Escapes all hot key characters ('&') in a string.
			 * Setting the result as field text (Field::setText()) shows the ampersand instead of declaring a hot key.
			 * @param text The original text.
			 * @return The escaped text.
			 * @see setText()
			 */
			static String escapeHotKeys(const String & text);

			/**
			 * The way check box or radio box attachments should work.
			 * Attaching a field to a check box or radio box makes
			 * the field active depending on the box'es check state.
			 * @see attachTo()
			 */
			AttachmentType getAttachmentType();

			/**
			 * Returns the client area of the field. The client area is the
			 * rectangular region that remains when leaving out the frame area.
			 * The client area is a separate component and must be set into the window.
			 * @return The client area.
			 */
			ClientArea * getClientArea();

			/**
			 * Returns the font to be used for texts included in the field.
			 * @return The font to be used.
			 */
			Font * getFont();

			/**
			 * Returns the fields area of the field, if the field is contained by one.
			 * If the field is contained by another type of client area, the method returns NLL.
			 * @return The fields area or NULL.
			 */
			FieldsArea * getFieldsArea();

			/**
			 * Returns the layout data for this field. Defaults to a FieldLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @return The layout data for this field.
			 * @see FieldLayoutData
			 */
			FieldLayoutData * getFieldLayoutData();

			/**
			 * Returns the group field this field is contained in, or NULL, if the field is top-level.
			 * Defaults to NULL.
			 * @note Group fields are used for layouting purposes,
			 * as they declare a bounding box for the layout process.
			 * @return The group field this field is contained in, or NULL.
			 * @see GroupField
			 */
			GroupField * getGroupField();

			/**
			 * Returns the position relative to the absolute client-area coordinates.
			 * @return The client-area relative position.
			 */
			Vector getPositionOnClientArea();

			/**
			 * Returns the position relative to the absolute window coordinates.
			 * @return The window relative position.
			 */
			Vector getPositionOnWindow();

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
			 * Returns the window this component is directly or indirectly contained in.
			 * @return The window or NULL, if this component is not yet attached.
			 */
			Window * getWindow();

			/**
			 * Returns whether this component has focus within its parent,
			 * and all ancestor's have focus within their respective parent.
			 * Only one component in the entire desktop has focus at
			 * any time (except that the parent of a focus component
			 * also has focus, of course).
			 * @return Whether this component has focus.
			 */
			virtual bool hasFocus();

			/**
			 * Hides the component (makes it invisible, but still existent).
			 */
			virtual void hide();

			/**
			 * Returns whether the field accepts onPress events to focus it. Defaults to true.
			 * @return Whether the field accepts onPress events to focus it.
			 */
			virtual bool isAcceptingPressesToFocus();

			/**
			 * For input fields: Returns whether this input field can get focus and therefore
			 * can be edited or selected. Not active fields are usually grayed out.
			 * For non-input fields: Returns whether this field should be displayed normally.
			 * Like input fields, not active fields are usually grayed out.
			 * @return Whether this field is active.
			 */
			bool isActive();

			/**
			 * Whether this field is active, and its group field is active, if any, and so on.
          * @return Whether this field and all its container group fields are active.
			 * @see isActive()
			 */
			bool isActiveIncludingAncestors();

			/**
			 * If this field is attached to a check box or radio box,
			 * whether this field should get focus when it gets active.
			 * Defaults to false.
			 * @see attachTo()
			 */
			bool isAttachmentFocussing();

			/**
			 * Returns whether to use all-capital characters when drawing the field's text.
			 * Does not change the text itself. Defaults to false.
			 * @return Whether to use all-capital characters when drawing the field's text.
			 */
			bool isCaps();

			/**
			 * Returns whether click operations on this field behave like clicking the title bar,
			 * if supported by the skin.
			 * Defaults to false for input fields and some special fields, true for regular fields.
			 * @param position The position within the field to return the information for.
			 * @return Whether this field is like the title bar.
			 */
			virtual bool isLikeTitle(Vector position);

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
			 * Handles the event this an element of this field has changed.
			 * By default, invalidates the field.
			 * @param element The element that changed.
			 */
			virtual void onElementChanged(FieldElement * element);

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
			 * Handles the event that the event target is pressed down
			 * (usually using the left mouse button).
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onRelease()
			 */
			virtual void onPress(Vector position, bool option1, bool option2);

			/**
			 * Handles the event that a pointer wheel (mouse, joystick, etc.)
			 * is changed while the mouse is over this event target.
			 * @param delta The relative wheel movement.
			 */
			virtual void onWheel(int delta);

			/**
			 * For input fields: Sets whether this input field can get focus and therefore
			 * can be edited or selected. Not active fields are usually grayed out.
			 * For non-input fields: Sets whether this field should be displayed normally.
			 * Like input fields, not active fields are usually grayed out.
			 * @param active Whether this field is active.
			 */
			virtual void setActive(bool active);

			/**
			 * The way check box or radio box attachments should work.
			 * Attaching a field to a check box or radio box makes
			 * the field active depending on the box'es check state.
			 * @see attachTo()
			 */
			void setAttachmentType(AttachmentType attachmentType);

			/**
			 * If this field is attached to a check box or radio box,
			 * whether this field should get focus when it gets active.
			 * Defaults to false.
			 * @see attachTo()
			 */
			void setAttachmentFocussing(bool attachmentFocussing);

			/**
			 * Sets whether to use all-capital characters when drawing the field's text.
			 * Does not change the text itself. Defaults to false.
			 * @param caps Whether to use all-capital characters when drawing the field's text.
			 */
			virtual void setCaps(bool caps);

			/**
			 * Sets the layout data for this field. Defaults to a FieldLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @param fieldLayoutData The layout data for this field.
			 * @note Calling this method does not relayouts the fields area, if automatic layouting
			 * is enabled, so you have to call the layout method manually, in this case.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 * @see FieldLayoutData
			 */
			void setFieldLayoutData(FieldLayoutData * fieldLayoutData);

			/**
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

			/**
			 * Sets the font to be used for texts included in the field, by its ID.
			 * @param font The ID of the font to be used.
			 */
			void setFontId(const String & fontId);

			/**
			 * Sets the font to be used for texts included in the field, by its ID.
			 * @param font The ID of the font to be used.
			 */
			void setFontId(const char * fontId);

			/**
			 * Sets whether click operations on this field behave like clicking the title bar,
			 * if supported by the skin.
			 * Defaults to false for input fields and some special fields, true for regular fields.
			 * @param likeTitle Whether this field is like the title bar.
			 */
			void setLikeTitle(bool likeTitle);

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
			 * Defers the setText operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param text The text (the caption) of this field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 * @see Desktop::deferOperation()
			 */
			void setTextDeferred(const String & text);

			/**
			 * Shows the component (makes it visible again).
			 */
			virtual void show();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
