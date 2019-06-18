/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SPLITTINGAREA_H
#define __GLAZIERY_SPLITTINGAREA_H


namespace glaziery
{
	/**
	 * A splitting area is a client area that consists of a horizontal row
	 * or a vertical column of other client areas. This is useful to split
	 * the windows client area size into several resizable sub areas.
	 * The sub areas of a splitting area are called panes.
	 * Since splitting areas are client areas themselves, they may be
	 * nested, so arbitrary structures of splitting can be created.
	 * In each splitting area, only one area has focus.
	 */
	class SplittingArea : public ClientArea
	{
		public:

			enum Direction
			{
				/**
				 * The panes are oriented horizontally next to each other.
				 * The first pane is the left-most one.
				 * @see direction
				 */
				HORIZONTAL,

				/**
				 * The panes are oriented vertically below each other.
				 * The first pane is the top-most one.
				 * @see direction
				 */
				VERTICAL
			};

		private:

			/**
			 * Whether all sub client areas (panes) queue up as a horizontal
			 * row (HORIZONTAL) or as a vertical column (VERTICAL).
			 * @see Direction
			 */
			Direction direction;

			/**
			 * The sub client area (pane) that currently has focus.
			 * The focus pane e.g. receives all key events of the splitting area.
			 */
			ClientArea * focusPane;

			/**
			 * The sub client areas (panes) of this splitting area.
			 * The panes are aligned horizontally or vertically,
			 * according to the direction property.
			 * @see direction
			 */
			ArrayList<ClientArea> panes;

			/**
			 * Whether the user may resize the panes to each other,
			 * using the mouse e.g. on the border between the panes.
			 * More precisely, the skin defines the look and feel to do this.
			 * This property only defines whether it is possible.
			 */
			bool panesResizable;

			/**
			 * This method adjusts the position and size of all sub client areas
			 * (panes) according to the splitting area's position, size,
			 * and properties (e.g. skin splitter width).
			 */
			void adjustClientAreas();

			/**
			 * Resizes the splitting area to meet requirements for a new pane.
			 * @param pane The pane that has been added.
			 */
			void resizeForNewPane(ClientArea * pane);

		protected:

			/**
			 * Destroys the splitting area.
			 */
			virtual ~SplittingArea();

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
				DECLARE_DYNAMIC(SplittingArea);
			#endif

		public:

			/**
			 * Creates a new splitting area.
			 * @param direction Whether all sub client areas (panes)
			 * queue up as a horizontal row (HORIZONTAL)
			 * or as a vertical column (VERTICAL).
			 * @see Direction
			 */
			SplittingArea(Direction direction);

			/**
			 * Adds a sub client area (pane) at the end of the list of panes.
			 * End means at the very right or very bottom.
			 * @param pane The pane to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendPane(ClientArea * pane);

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
			virtual void drawContent(DrawingContext & context);

			/**
			 * Returns whether all sub client areas (panes) queue up as a
			 * horizontal row (HORIZONTAL) or as a vertical column (VERTICAL).
			 * @return How all panes queue up.
			 * @see Direction
			 */
			Direction getDirection();

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
			 * Returns the child component that has focus within this parent.
			 * @return The child component that has focus within this parent
			 * or NULL, if this component has no children or if focusing is
			 * not subject of this component.
			 */
			virtual Component * getFocusChild();

			/**
			 * Returns the sub client area (pane) that currently has focus.
			 * The focus pane e.g. receives all key events of the splitting area.
			 * @return The current focus pane.
			 */
			virtual ClientArea * getFocusPane();

			/**
			 * Returns the sub client areas (panes) of this splitting area.
			 * The panes are aligned horizontally or vertically,
			 * according to the direction property.
			 * @return The sub client areas (panes) of this splitting area.
			 * @see getDirection()
			 */
			virtual const ArrayList<ClientArea> & getPanes();

			/**
			 * Adds a sub client area (pane) before a given position.
			 * @param position The 0-indexed position to add the pane at.
			 * 0 is the left-most or top-most pane.
			 * @param pane The pane to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertPane(int position, ClientArea * pane);

			/**
			 * Returns whether the user may resize the panes to each other,
			 * using the mouse e.g. on the border between the panes.
			 * More precisely, the skin defines the look and feel to do this.
			 * This property only defines whether it is possible.
			 * @return Whether the user may resize the panes to each other.
			 */
			bool isPanesResizable();

			/**
			 * Moves a splitter between two sub client areas (panes), resizing
			 * both adjacent areas. This is helpful because it prevents the
			 * splitting area from being resized itself, so no superfluous events
			 * and other handling is raised.
			 * @param splitter The number of the splitter to be resized.
			 * 0 stands for the splitter between the first and the second pane,
			 * 1 stands for the one between the second and the third pane, and so on.
			 * @param delta The number of pixels to move the splitter.
			 * Positive values move the splitter to the right or to the bottom,
			 * according to the direction property.
			 * @see getDirection()
			 */
			void moveSplitter(int splitter, int delta);

			/**
			 * Handles the event that the back TAB key is pressed on the event target.
			 * On Windows systems, this is SHIFT+TAB.
			 * Back tabs step the focus from pane to pane in backward order.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onBackTab(bool secondary);

			/**
			 * Performs operations necessary when a child component
			 * has been moved within this parent. E.g. move the parent,
			 * too, if the child uses all the area of the parent, or
			 * ignore the event if the child can move freely.
			 * @param child The child component that moved.
			 * @param oldPosition The position of the child before the move.
			 */
			virtual void onChildMoved(Component * child, Vector oldPosition);

			/**
			 * Performs operations necessary when a child component
			 * has been resized within this parent. E.g. resize the parent,
			 * too, if the child uses all the area of the parent, or
			 * ignore the event if the child can resize freely.
			 * @param child The child component that resized.
			 * @param oldSize The size of the child before the resize.
			 */
			virtual void onChildResized(Component * child, Vector oldSize);

			/**
			 * Notifies all component listeners about the destruction
			 * of the component, but does not delete it nor adds it to
			 * the list of disposable components.
			 * @note Override this method if you require pre-deletion operations.
			 * The default implementation notifies listeners only.
			 */
			virtual void onDestroying();

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
			 * Handles the event that the TAB key is pressed on the event target.
			 * Tabs step the focus from pane to pane.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onTab(bool secondary);

			/**
			 * Adds a sub client area (pane) at the beginning of the list of panes.
			 * Beginning means at the very left or very top.
			 * @param pane The pane to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependPane(ClientArea * pane);

			/**
			 * Invalidates and releases this component's internal caches (e.g. drawing caches).
			 * This method should be when it is unlikely that the cache will be needed
			 * in the near future, to release system resources (e.g. when the component gets invisible).
			 */
			virtual void releaseCache();

			/**
			 * Removes a pane by its position and destroys it.
			 * @note The component is added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @param position The zero-indexed position of the pane to be destroyed.
			 */
			void removePane(int position);

			/**
			 * Sets the sub client area (pane) that shall get focus.
			 * The focus pane e.g. receives all key events of the splitting area then.
			 * @param focusPane The new focus pane.
			 */
			virtual void setFocusPane(ClientArea * focusPane);

			/**
			 * Sets the focus pane to the next available and valid pane.
			 * The method rolls around the end of the panes list.
			 * @return Whether the method rolled around the end of the panes list.
			 * @note The invocation is first propagated to the focused child area.
			 * Only if the focus pane rolls around the end of the panes list,
			 * this area steps to the next pane. If the child area does not support
			 * focusing, this area steps to the next pane, too.
			 */
			bool setFocusToNextPane();

			/**
			 * Sets the focus pane to the previous available and valid pane.
			 * The method rolls around the beginning of the panes list.
			 * @return Whether the method rolled around the beginning of the panes list.
			 * @note The invocation is first propagated to the focused child area.
			 * Only if the focus pane rolls around the beginning of the panes list,
			 * this area steps to the previous pane. If the child area does not support
			 * focusing, this area steps to the previous pane, too.
			 */
			bool setFocusToPreviousPane();

			/**
			 * Sets whether the user may resize the panes to each other,
			 * using the mouse e.g. on the border between the panes.
			 * More precisely, the skin defines the look and feel to do this.
			 * This property only defines whether it is possible.
			 * @param panesResizable Whether the user may resize the panes.
			 */
			void setPanesResizable(bool panesResizable);
	};
}


#endif
