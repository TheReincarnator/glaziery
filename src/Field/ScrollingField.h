/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SCROLLINGFIELD_H
#define __GLAZIERY_SCROLLINGFIELD_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class ScrollingField : public InputField
	{
		public:

			/**
			 * The base listener interface for scrolling field events.
			 * This listener additionally enables listening to events that are specially
			 * transformed for scrolling (e.g. pointer operations).
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public Field::Listener
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
					 * Called when the component is context-clicked
					 * (usually using the right mouse button),
					 * called by the scrolling field with content-transformed coordinates.
					 * @param component The component that received the event.
					 * @param position The position where the context click occurred
					 * relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onContentContextClick(Component * component, Vector position,
						bool option1, bool option2);

					/**
					 * Called when the component is double-clicked (usually using the left mouse button).
					 * called by the scrolling field with content-transformed coordinates.
					 * A double click is raised when the mouse is pressed, released, and pressed
					 * again within a short time. In this case, the entire event sequence is:
					 * onContentPress, onContentRelease, onContentClick, onContentPress,
					 * onContentDoubleClick, onContentRelease, onContentClick,
					 * so the double-click is raised in response to the second press.
					 * @param component The component that received the event.
					 * @param position The position where the double click occurred
					 * relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onClick()
					 */
					virtual void onContentDoubleClick(Component * component, Vector position,
						bool option1, bool option2);

					/**
					 * Called when, after the event target is pressed down on the content
					 * (usually using the left mouse button), the pointer is not moved,
					 * but the button has not yet been released. onContentHold() is called in the time
					 * between onContentPress() and onContentRelease(). Glaziery provides the time between
					 * the last call of onContentHold() and this one.
					 * @param component The component that received the event.
					 * @param position The current position of the pointer within the content,
					 * relative to the upper-left corner of the content.
					 * @param holdTime The hold time between the last call of onContentHold() and this one,
					 * plus the hold time not consumed in the last call of onContentHold(). The time's
					 * unit is milli seconds.
					 * @see onContentPress()
					 * @see onContentRelease()
					 */
					virtual void onContentHold(Component * component, Vector position, long holdTime);

					/**
					 * Called when the component is pressed down
					 * (usually using the left mouse button),
					 * called by the scrolling field with content-transformed coordinates.
					 * @param component The component that received the event.
					 * @param position The position where the click occurred
					 * relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onRelease()
					 */
					virtual void onContentPress(Component * component, Vector position, bool option1, bool option2);

					/**
					 * Called when the event target press is released,
					 * called by the scrolling field with content-transformed coordinates.
					 * @param component The component that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onPress()
					 */
					virtual void onContentRelease(Component * component, bool option1, bool option2);

					/**
					 * Called when the scroll position now is set.
					 * @param field The field that received the event.
					 * @param previousPosition The previous scrolling position now.
					 */
					virtual void onScrollPositionNowSet(ScrollingField * field, Vector previousPosition);
			};

		private:

			/**
			 * The size of the border to be displayed around the content view.
			 * (0, 0) does not display any border at all, (1, 1) displays a simple border.
			 * Defaults to (1, 1), suitable for most field areas.
			 * The SingleFieldArea and the ListField's RowsField automatically
			 * set this property to (0, 0).
			 */
			Vector borderSize;

			/**
			 * Whether the constrainScrollPosition method is running.
			 */
			bool constrainingScrollPosition;

			/**
			 * The size of the padding space between the scrollers (if any) and the content view.
			 * If the respective scroller exists, this padding is used at the bottom-right corner.
			 * Defaults to (1, 1). The SingleFieldArea automatically set this property to (2, 2),
			 * and the ListField's RowsField sets it to (0, 0).
			 */
			Vector innerPaddingSize;

			/**
			 * Whether we are currently in the scrollToNow method, to prevent endless recursion.
			 */
			bool inScrollToNow;

			/**
			 * The size of the padding space between the border (if any) and the content view.
			 * This padding is used at the top-left corner, and if the respective scroller does not exist,
			 * it is also used at the bottom-right corner.
			 * Defaults to (1, 1). The SingleFieldArea automatically set this property to (2, 2),
			 * and the ListField's RowsField sets it to (0, 0).
			 */
			Vector outerPaddingSize;

			/**
			 * Whether the last onPress was within the content.
			 */
			bool pressWithinContent;

			/**
			 * The height (in pixels) of the horizontal scroller
			 * resp. the width of the vertical scroller.
			 */
			int scrollerBreadth;

			/**
			 * The number of pixels to scroll when an arrow is pressed by the user.
			 * Defaults to (20, 20).
			 */
			Vector scrollingUnitSize;

			/**
			 * The current scroll position. This is the position of the content
			 * to be displayed in the upper-left corner of the scrolling field.
			 */
			Vector scrollPositionNow;

		protected:

			/**
			 * The calculated size of the content, as if there was no scrolling field
			 * and the field must display its entire content at once.
			 * The scrolling field automatically adjusts the scrollers according to
			 * this value.
			 * @note The value is updated by the updateContentSize() method.
			 * It should be called when the content changes in a way that might change
			 * the its size.
			 * @see updateContentSize()
			 * @see getContentSize()
			 */
			Vector contentSize;

			/**
			 * Creates a new scrolling field.
			 */
			ScrollingField();

			/**
			 * Destroys the scrolling field.
			 */
			virtual ~ScrollingField();

			/**
			 * Ensures that the scroll position is within its valid range.
			 * This method is called automatically when invalidating.
			 */
			void constrainScrollPosition();

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
			virtual void drawContent(DrawingContext & context) = NULL;

			/**
			 * For a given position within the field, returns the position relative
			 * to the content of the field.
			 * @param positionWithinField The position within the field.
			 * @return The position relative to the content of the field.
			 */
			Vector getPositionWithinContent(Vector positionWithinField);

			/**
			 * Returns the sizes of the scrollers, regarding whether they are turned on.
			 * The Skin's method getScrollingFieldScrollerSizes() does not regard this.
			 * @return The sizes of the scrollers, regarding whether they are turned on.
			 */
			Vector getScrollerSizes();

			/**
			 * Returns whether a position is within the content area of the scrolling field,
			 * i.e. not within a scroller.
			 * @param position The position relative to the upper-left corner of the field.
			 * @return Whether the position is within the content area.
			 */
			bool isWithinContent(Vector position);

			/**
			 * Handles the event that a the scroll position now value has changed.
			 * @param previousPosition The old scroll position now value.
			 */
			virtual void onScrollPositionNowChanged(Vector previousPosition);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ScrollingField);
			#endif

		public:

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
			 * Adjusts the scroll position so that a given point within the content
			 * is visible. The method tries to do as least scrolling as possible.
			 */
			virtual void ensureVisible(Vector point);

			/**
			 * Adjusts the scroll position so that a given rectangle within the content
			 * is entirely visible. The method tries to do as least scrolling as possible.
			 * If the rectangle is larger than the content view of the scrolling field,
			 * only the top-left part of the rectangle will be visible.
			 */
			virtual void ensureVisible(Vector topLeft, Vector bottomRight);

			/**
			 * Adjusts the scroll position so that a given point within the content
			 * is visible now, bypassing the skin.
			 * The method tries to do as least scrolling as possible.
			 * @note Use this method in the skin to set the position directly.
			 */
			virtual void ensureVisibleNow(Vector point);

			/**
			 * Adjusts the scroll position so that a given rectangle within the content
			 * is entirely visible now, bypassing the skin.
			 * The method tries to do as least scrolling as possible.
			 * If the rectangle is larger than the content view of the scrolling field,
			 * only the top-left part of the rectangle will be visible.
			 * @note Use this method in the skin to set the position directly.
			 */
			virtual void ensureVisibleNow(Vector topLeft, Vector bottomRight);

			/**
			 * The size of the border to be displayed around the content view.
			 * (0, 0) does not display any border at all, (1, 1) displays a simple border.
			 * Defaults to (1, 1), suitable for most field areas.
			 * The SingleFieldArea and the ListField's RowsField automatically
			 * set this property to (0, 0).
			 * @return The size of the border or (0, 0), if the border is disabled.
			 */
			Vector getBorderSize();

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
			 * Returns the size of the content, as if there was no scrolling field
			 * and the field must display its entire content at once.
			 * The scrolling field automatically adjusts the scrollers according to
			 * this value.
			 * @note The value is updated by the updateContentSize() method.
			 * It should be called when the content changes in a way that might change
			 * the its size.
			 * @return The size of the content.
			 * @see updateContentSize()
			 */
			Vector getContentSize();

			/**
			 * Returns the size of the content view, i.e. the scrolling field's size
			 * without the scrolling field border and scrollers. The content view size
			 * is the visible area of the content.
			 * @return The size of the content view.
			 */
			Vector getContentViewSize();

			/**
			 * Returns the size of the padding space at the bottom-right corner of the content view.
			 * If a scroller exists, this is the inner padding space, otherwise, it is the outer padding space.
			 * Defaults to (1, 1).
			 * @return The size of the effective inner padding space.
			 * @see getInnerPaddingSize()
			 * @see getOuterPaddingSize()
			 */
			Vector getEffectiveInnerPaddingSize();

			/**
			 * Returns the event target at a given position that is setup to
			 * receive pointer events. For popup components, this is always true,
			 * for windows, this is true if and only if the pointer event mode is
			 * active or inactive, but bypassing windows are treated as not
			 * existing (transparent for events), and for widgets, this depends on
			 * the widget's hit test (isHitAt()).<br>
			 *
			 * This is a deep search: The desktop delegates to the found
			 * window or popup component, which delegates to the client area
			 * and so on.
			 * @param position The position relative to the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @return The event receiving event target at the position or
			 * <code>NULL</code>, if there is no target.
			 */
			virtual EventTarget * getEventTargetAt(Vector position);

			/**
			 * Returns the size of the padding space between the scrollers (if any) and the content view.
			 * If the respective scroller exists, this padding is used at the bottom-right corner.
			 * Defaults to (1, 1).
			 * @return The size of the inner padding space.
			 * @see getEffectiveInnerPaddingSize()
			 */
			Vector getInnerPaddingSize();

			/**
			 * Returns the maximum value for the scroll position.
			 * If the content view size is bigger than the content size
			 * for one of the axises or both, then the respective value is 0.
			 * @return The maximum value for the scroll position.
			 */
			Vector getMaximumScrollPosition();

			/**
			 * The size of the padding space between the border (if any) and the content view.
			 * This padding is used at the top-left corner, and if the respective scroller does not exist,
			 * it is also used at the bottom-right corner.
			 * Defaults to (1, 1).
			 * @return The size of the outer padding space.
			 * @see getEffectiveInnerPaddingSize()
			 */
			Vector getOuterPaddingSize();

			/**
			 * Returns the height (in pixels) of the horizontal scroller
			 * resp. the width of the vertical scroller.
			 * @return The new breadth in pixels.
			 */
			int getScrollerBreadth();

			/**
			 * Returns the number of pixels to scroll when an arrow is pressed by the user.
			 * Defaults to (20, 20).
			 * @return The number of pixels to scroll.
			 */
			Vector getScrollingUnitSize();

			/**
			 * Returns the current scroll position the way the skin defines. The scroll position
			 * is the position of the content to be displayed in the upper-left corner of the
			 * scrolling field. The method automatically constraints the position within the valid range.
			 * @return The current scroll position.
			 */
			Vector getScrollPosition();

			/**
			 * Returns the current scroll position now, bypassing the skin. The scroll position
			 * is the position of the content to be displayed in the upper-left corner of the
			 * scrolling field. The method automatically constraints the position within the valid range.
			 * @note Use this method in the skin to get the position directly.
			 * @return The current scroll position.
			 */
			Vector getScrollPositionNow();

			/**
			 * Invalidates a part of this component's internal caches (e.g. drawing caches).
			 * This method should be called whenever its state changes.
			 * The invalidation is propagated to the parent, if any.
			 * @param position The top-left position of the area to be invalidated,
			 * relative to this component.
			 * @param size The size of the area to be invalidated.
			 */
			virtual void invalidateArea(Vector position, Vector size);

			/**
			 * Returns whether the scrolling field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the scrolling field displays a horizontal scroller.
			 */
			virtual bool isHorizontallyScrollable() = NULL;

			/**
			 * Returns whether the scrolling field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the scrolling field displays a horizontal scroller.
			 */
			virtual bool isVerticallyScrollable() = NULL;

			/**
			 * Handles the event that the event target is clicked
			 * (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the component.
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
			 * Called by the scrolling field to notify the content when the field
			 * is clicked (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position within the content where the click
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onContentPress()
			 * @see onContentRelease()
			 */
			virtual void onContentClick(Vector position, bool option1, bool option2);

			/**
			 * Called by the scrolling field to notify the content when the field
			 * is context-clicked (usually using the right mouse button).
			 * @param position The position within the content where the context click
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContentContextClick(Vector position, bool option1, bool option2);

			/**
			 * Called by the scrolling field to notify the content when the field
			 * is double-clicked (usually using the left mouse button).
			 * @param position The position within the content where the double click
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContentDoubleClick(Vector position, bool option1, bool option2);

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
			 * Called by the scrolling field to notify the content when the field
			 * press is released.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onContentPress()
			 */
			virtual void onContentRelease(bool option1, bool option2);

			/**
			 * Handles the event that the event target is context-clicked
			 * (usually using the right mouse button).
			 * @param position The position where the context click occurred
			 * relative to the upper-left corner of the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContextClick(Vector position, bool option1, bool option2);

			/**
			 * Handles the event that the target is double-clicked (usually using the left mouse button).
			 * A double click is raised when the mouse is pressed, released, and pressed
			 * again within a short time. In this case, the entire event sequence is:
			 * onPress, onRelease, onClick, onPress, onDoubleClick, onRelease, onClick,
			 * so the double-click is raised in response to the second press.
			 * @param position The position where the double click occurred
			 * relative to the upper-left corner of the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onDoubleClick(Vector position, bool option1, bool option2);

			/**
			 * Handles the event that, after the event target is pressed down
			 * (usually using the left mouse button), the pointer is not moved,
			 * but the button has not yet been released. onHold() is called in the time
			 * between onPress() and onRelease(). Glaziery provides the time between
			 * the last call of onHold() and this one. It expects that the onHold
			 * implementation issues in its return value the hold time not consumed.
			 * If 0 is returned, all the time has been consumed and the holdTime reflects
			 * only the time between two consecutive onHold() calls each time.
			 * If a positive value is returned, this value is added to the next holdTime value.
			 * This helps to implement operations that have to be performed after
			 * some time has elapsed, while not losing remaining milli seconds.
			 * The following code snippet illustrates this operation:
			 * <code><pre>
			 * long MyClass::onHold(Vector position, long holdTime)
			 * {
			 * 	while (holdTime >= 100)
			 * 	{
			 * 		performSomeOperation();
			 * 		holdTime -= 100;
			 * 	}
			 * 	return holdTime;
			 * }
			 * </pre></code>
			 * @param position The current position of the pointer relative to
			 * the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param holdTime The hold time between the last call of onHold() and this one,
			 * plus the hold time not consumed in the last call of onHold(). The time's
			 * unit is milli seconds.
			 * @return The hold time this method has not consumed in milli seconds.
			 * @see onPress()
			 * @see onRelease()
			 */
			virtual long onHold(Vector position, long holdTime);

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
			 * Handles the event that the event target press is released.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onPress()
			 */
			virtual void onRelease(bool option1, bool option2);

			/**
			 * Handles the event that a pointer wheel (mouse, joystick, etc.)
			 * is changed while the mouse is over this event target.
			 * @param delta The relative wheel movement.
			 */
			virtual void onWheel(int delta);

			/**
			 * Translates the current scroll position by a given delta (adds it to the
			 * current value. The scroll position is the position of the content
			 * to be displayed in the upper-left corner of the scrolling field.
			 * The method automatically constraints the position within the valid range.
			 * @param delta The delta to be scrolled.
			 */
			void scroll(Vector delta);

			/**
			 * Sets the current scroll position the way the skin defines. The scroll position
			 * is the position of the content to be displayed in the upper-left corner of the
			 * scrolling field. The method automatically constraints the position within the valid range.
			 * @param scrollPosition The new scroll position.
			 */
			void scrollTo(Vector scrollPosition);

			/**
			 * Sets the current scroll position now, bypassing the skin. The scroll position
			 * is the position of the content to be displayed in the upper-left corner of the
			 * scrolling field. The method automatically constraints the position within the valid range.
			 * @note Use this method in the skin to set the position directly.
			 * @param scrollPosition The new scroll position.
			 */
			void scrollToNow(Vector scrollPosition);

			/**
			 * The size of the border to be displayed around the content view.
			 * (0, 0) does not display any border at all, (1, 1) displays a simple border.
			 * Defaults to (1, 1), suitable for most field areas.
			 * The SingleFieldArea and the ListField's RowsField automatically
			 * set this property to (0, 0).
			 * @param borderSize The size of the border or (0, 0), to disable the botder.
			 */
			void setBorderSize(Vector borderSize);

			/**
			 * The size of the padding space between the scrollers (if any) and the content view.
			 * If the respective scroller exists, this padding is used at the bottom-right corner.
			 * Defaults to (1, 1).
			 * @param innerPaddingSize The size of the inner padding space.
			 * @see getEffectiveInnerPaddingSize()
			 */
			void setInnerPaddingSize(Vector innerPaddingSize);

			/**
			 * The size of the padding space between the border (if any) and the content view.
			 * This padding is used at the top-left corner, and if the respective scroller does not exist,
			 * it is also used at the bottom-right corner.
			 * Defaults to (1, 1).
			 * @param outerPaddingSize The size of the outer padding space.
			 * @see getEffectiveInnerPaddingSize()
			 */
			void setOuterPaddingSize(Vector outerPaddingSize);

			/**
			 * Sets the height (in pixels) of the horizontal scroller
			 * resp. the width of the vertical scroller.
			 * @param scrollerBreadth The new breadth in pixels.
			 */
			void setScrollerBreadth(int scrollerBreadth);

			/**
			 * Sets the number of pixels to scroll when an arrow is pressed by the user.
			 * Defaults to (20, 20).
			 * @param scrollingUnitSize The number of pixels to scroll.
			 */
			void setScrollingUnitSize(Vector scrollingUnitSize);

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
			virtual void updateContentSize() = NULL;
	};
}


#endif
