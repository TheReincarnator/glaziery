/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FIELDSAREA_H
#define __GLAZIERY_FIELDSAREA_H


namespace glaziery
{
	/**
	 * A fields area is a client area that contains fields at
	 * specific (fixed) positions. A window with a fields area
	 * is typically used as a modal dialog (user input).
	 * Field areas may also be used to group fields and display
	 * the whole group as a popup window.
	 */
	class FieldsArea : public ScrollingArea
	{
		friend Field;

		private:

			/**
			 * Whether auto layouting centers the window of this fields area.
			 */
			bool autoLayoutCentering;

			/**
			 * Whether the fields area should automatically layout its fields
			 * whenever they change (fields are added, resized, etc.).
			 * Defaults to false.
			 */
			bool autoLayouting;

			/**
			 * Whether auto layouting resizes the fields area according
			 * to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp column or row. Then, the fields area is resized.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The fields area is not resized in this case.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.
			 * @see setAutoLayouting()
			 */
			bool autoLayoutResizing;

			/**
			 * The default button if the fields area' window is run modally.
			 * Pressing enter in an input field causes the default button to be pressed.
			 */
			Button * defaultButton;

			/**
			 * The default bottom and right margin sizes for newly added fields in pixels.
			 * @see Field::bottomRightMargin
			 */
			Vector defaultFieldBottomRightMargin;

			/**
			 * The default bottom and right padding sizes for newly added fields in pixels.
			 * @see Field::bottomRightPadding
			 */
			Vector defaultFieldBottomRightPadding;

			/**
			 * The default top and left margin sizes for newly added fields in pixels.
			 * @see Field::topLeftMargin
			 */
			Vector defaultFieldTopLeftMargin;

			/**
			 * The default top and left padding sizes for newly added fields in pixels.
			 * @see Field::topLeftPadding
			 */
			Vector defaultFieldTopLeftPadding;

			/**
			 * The fields contained in this fields area.
			 */
			ArrayList<Field> fields;

			/**
			 * The one field that currently has focus.
			 * Initially, no field has focus.
			 * The field with focus receives all events.
			 */
			InputField * focusField;

			/**
			 * The layout data for this fields area. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @see ContainerLayoutData
			 */
			ContainerLayoutData * layoutData;

			/**
			 * The layouter to be used when manually or automatically layouting the area's fields.
			 * Defaults to the GridLayouter.
			 * @see Layouter
			 * @see GridLayouter
			 */
			Layouter * layouter;

			/**
			 * Whether the field area is currently layouting. This is used to prevent endless recursion.
			 */
			bool layouting;

			/**
			 * The radio groups managed by this fields area.
			 * A fields area owns all radio groups its radio boxes.
			 */
			ArrayList<RadioGroup> radioGroups;

		protected:

			/**
			 * Destroys the fields area.
			 */
			virtual ~FieldsArea();

			/**
			 * Focusses a given field if it can be focused (active, visible
			 * and not the unchecked box of a radio group with one box checked).
			 * @param field The field to be checked.
			 * @return Whether the field was focussable.
			 */
			bool focusIfPossible(Field * field);

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
				DECLARE_DYNAMIC(FieldsArea);
			#endif

		public:

			/**
			 * Creates a new fields area.
			 */
			FieldsArea();

			/**
			 * Appends a field at the end of the fields list of this fields area.
			 * The order of the fields also determines the tab stop order.
			 * @param field The field to be added.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendField(Field * field);

			/**
			 * Creates a new radio group.
			 * @note The memory is owned by the fields area.
			 * It is automatically deleted when the fields area is.
			 * @return The radio group.
			 */
			RadioGroup * createRadioGroup();

			/**
			 * Draws the content of the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The implementation must clear the context before rendering,
			 * if it does not render to the entire context.
			 * @note This method is called when the cache is invalid only.
			 * See invalidate() and the CachableComponent's class comment for details.
			 * @see Desktop::getSkin()
			 * @see CachableComponent::invalidate()
			 * @see CachableComponent
			 */
			virtual void drawAreaContent(DrawingContext & context);

			/**
			 * Returns the event target at a given position within the content
			 * that is setup to receive pointer events.<br>
			 *
			 * This is a deep search: The desktop delegates to the found
			 * window or popup component, which delegates to the client area
			 * and so on.
			 * @param position The position relative to the content's upper-left corner.
			 * @return The event receiving event target at the position or
			 * <code>NULL</code>, if there is no target.
			 */
			virtual EventTarget * getContentEventTargetAt(Vector position);

			/**
			 * The default button if the fields area' window is run modally.
			 * Pressing enter in an input field causes the default button to be pressed.
			 * @return defaultButton The default.
			 */
			Button * getDefaultButton();

			/**
			 * Returns the default bottom and right margin sizes for newly added fields in pixels.
			 * @return The default bottom and right margin sizes.
			 * @see Field::getBottomRightMargin()
			 */
			Vector getDefaultFieldBottomRightMargin();

			/**
			 * Returns the default bottom and right padding sizes for newly added fields in pixels.
			 * @return The default bottom and right padding sizes.
			 * @see Field::getBottomRightPadding()
			 */
			Vector getDefaultFieldBottomRightPadding();

			/**
			 * Returns the default top and left margin sizes for newly added fields in pixels.
			 * @return The default top and left margin sizes.
			 * @see Field::getTopLeftMargin()
			 */
			Vector getDefaultFieldTopLeftMargin();

			/**
			 * Returns the default top and left padding sizes for newly added fields in pixels.
			 * @return The default top and left padding sizes.
			 * @see Field::getTopLeftPadding()
			 */
			Vector getDefaultFieldTopLeftPadding();

			/**
			 * Returns the fields contained in this fields area.
			 * @return The fields contained in this fields area.
			 */
			const ArrayList<Field> & getFields();

			/**
			 * Returns the child component that has focus within this parent.
			 * @return The child component that has focus within this parent
			 * or NULL, if this component has no children or if focusing is
			 * not subject of this component.
			 */
			virtual Component * getFocusChild();

			/**
			 * Returns the one field that currently has focus.
			 * Initially, no field has focus.
			 * The field with focus receives all events.
			 * @return The one field that currently has focus.
			 */
			InputField * getFocusField();

			/**
			 * Returns the layout data for this fields area. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @return The layout data for this fields area.
			 * @see ContainerLayoutData
			 */
			ContainerLayoutData * getLayoutData();

			/**
			 * Returns the layouter to be used when manually or automatically layouting the area's fields.
			 * Defaults to the GridLayouter.
			 * @return The layouter to be used.
			 * @see Layouter
			 * @see GridLayouter
			 */
			Layouter * getLayouter();

			/**
			 * Inserts a field at a given position of the fields list of this fields area.
			 * The order of the fields also determines the tab stop order.
			 * @param position The position to insert the field at.
			 * @param field The field to be inserted.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertField(int position, Field * field);

			/**
			 * Returns whether auto layouting centers the window of this fields area.
			 * @return Whether auto layouting centers the window of this fields area.
			 */
			bool isAutoLayoutCentering();

			/**
			 * Returns the fields area should automatically layout its fields
			 * whenever they change (fields are added, resized, etc.).
			 * Defaults to false.
			 * @return Whether the fields area should automatically layout.
			 */
			bool isAutoLayouting();

			/**
			 * Returns whether auto layouting resizes the fields area according
			 * to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp column or row. Then, the fields area is resized.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The fields area is not resized in this case.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.
			 * @return Whether auto layouting resizes the fields area according
			 * @see setAutoLayouting()
			 */
			bool isAutoLayoutResizing();

			/**
			 * Layouts all fields in this fields area according to the layout data
			 * of the area and the fields. You can call this method after having added
			 * all fields manually, or set the fields area to be automatically layouting.
			 * In both cases, first add and setup all fields, then call the layouting methods.
			 * @param layoutResizing Whether layouting resizes the fields area according
			 * to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp column or row. Then, the fields area is resized.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The fields area is not resized in this case.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * Defaults to true.
			 * @param layoutCentering Whether auto layouting resizes the fields area according
			 * @see setAutoLayouting()
			 */
			void layout(bool layoutResizing = true, bool layoutCentering = true);

			/**
			 * Layouts all fields in this fields area if automatic layouting is turned on,
			 * using the automatic layout resizing property as well.
			 * @see layout()
			 * @see setAutomaticLayouting()
			 */
			void layoutIfAutomatic();

			/**
			 * Handles the event that the back TAB key is pressed on the event target.
			 * On Windows systems, this is SHIFT+TAB.
			 * Back tabs step the focus from field to field in backward order.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onBackTab(bool secondary);

			/**
			 * Notifies all component listeners about the destruction
			 * of the component, but does not delete it nor adds it to
			 * the list of disposable components.
			 * @note Override this method if you require pre-deletion operations.
			 * The default implementation notifies listeners only.
			 */
			virtual void onDestroying();

			/**
			 * Handles the event that a hot key has been pressed on the event target.
			 * Hot keys represent special characters e.g. to easily access fields
			 * (indicated by underlined characters).
			 * @param character The hot key character pressed.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onHotKey(char character, bool option1, bool option2);

			/**
			 * Handles the event that the TAB key is pressed on the event target.
			 * Tabs step the focus from field to field.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onTab(bool secondary);

			/**
			 * Prepends a field at the beginning of the fields list of this fields area.
			 * The order of the fields also determines the tab stop order.
			 * @param field The field to be added.
			 * @note Calling this method on fields relayouts the fields area,
			 * if automatic layouting is enabled, so consider turning it off while doing major changes
			 * to the fields of such fields area.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependField(Field * field);

			/**
			 * Removes a field from the fields area and deletes it.
			 * @note The component is added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @return field The field to be deleted.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 */
			void removeField(Field * field);

			/**
			 * Sets whether the fields area should automatically layout its fields
			 * whenever they change (fields are added, resized, etc.).
			 * Defaults to false.
			 * @param autoLayouting Whether the fields area should automatically layout.
			 * Setting the auto layouting to true also manually layouts once.
			 * @param autoLayoutResizing Whether auto layouting resizes the fields area according
			 * to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp. column or row. Then, the fields area is resized.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The fields area is not resized in this case.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * Defaults to true.
			 * @param autoLayoutCentering Whether auto layouting centers the window of this fields area.
			 * @see layout()
			 */
			void setAutoLayouting(bool autoLayouting, bool autoLayoutResizing = true,
				bool autoLayoutCentering = true);

			/**
			 * Sets the default button if the fields area' window is run modally.
			 * Pressing enter in an input field causes the default button to be pressed.
			 * @param defaultButton The default button.
			 */
			void setDefaultButton(Button * defaultButton);

			/**
			 * Sets the default bottom and right margin sizes for newly added fields in pixels.
			 * @param defaultFieldBottomRightMargin The default bottom and right margin sizes.
			 * @see Field::setBottomRightMargin()
			 */
			void setDefaultFieldBottomRightMargin(Vector defaultFieldBottomRightMargin);

			/**
			 * Sets the default bottom and right padding sizes for newly added fields in pixels.
			 * @param defaultFieldBottomRightPadding The default bottom and right padding sizes.
			 * @see Field::setBottomRightPadding()
			 */
			void setDefaultFieldBottomRightPadding(Vector defaultFieldBottomRightPadding);

			/**
			 * Sets the default top and left margin sizes for newly added fields in pixels.
			 * @param defaultFieldTopLeftMargin The default top and left margin sizes.
			 * @see Field::setTopLeftMargin()
			 */
			void setDefaultFieldTopLeftMargin(Vector defaultFieldTopLeftMargin);

			/**
			 * Sets the default top and left padding sizes for newly added fields in pixels.
			 * @param defaultFieldTopLeftPadding The default top and left padding sizes.
			 * @see Field::setTopLeftPadding()
			 */
			void setDefaultFieldTopLeftPadding(Vector defaultFieldTopLeftPadding);

			/**
			 * Sets the one field that has focus.
			 * The field with focus receives all events.
			 * @note This method does not check whether the field is visible and active.
			 * @param focusField The field that gets focus.
			 */
			void setFocusField(InputField * focusField);

			/**
			 * Sets the focus field to the first visible and active field.
			 */
			void setFocusToFirstField();

			/**
			 * Sets the focus field to the last visible and active field.
			 */
			void setFocusToLastField();

			/**
			 * Sets the focus field to the next visible and active field.
			 * The method rolls around the end of the fields list.
			 * @return Whether the focus stepping rolled around.
			 */
			bool setFocusToNextField();

			/**
			 * Sets the focus field to the previous visible and active field.
			 * The method rolls around the beginning of the fields list.
			 * @return Whether the focus stepping rolled around.
			 */
			bool setFocusToPreviousField();

			/**
			 * Sets the layout data for this fields area. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @param layoutData The layout data for this fields area.
			 * @note Calling this method does not relayouts the fields area, if automatic layouting
			 * is enabled, so you have to call the layout method manually, in this case.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 * @see ContainerLayoutData
			 */
			void setLayoutData(ContainerLayoutData * layoutData);

			/**
			 * Sets the layouter to be used when manually or automatically layouting the area's fields.
			 * Any previously set layouter is automatically deleted.
			 * @param layouter The layouter to be used.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 * @see Layouter
			 * @see GridLayouter
			 */
			void setLayouter(Layouter * layouter);
	};
}


#endif
