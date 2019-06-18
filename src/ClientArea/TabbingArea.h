/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TABBINGAREA_H
#define __GLAZIERY_TABBINGAREA_H


namespace glaziery
{
	/**
	 * A tabbing area is a client area that consists of a set of contained
	 * client areas (panes), exactly one of which visible at any time.
	 * To choose the visible (top) pane, the tabbing area has named tabs,
	 * one for each pane. The tabs indicate which pane is visible.
	 */
	class TabbingArea : public ClientArea
	{
		private:

			/**
			 * The sub client area (pane) that currently has focus.
			 * The focus pane e.g. receives all key events of the tabbing area.
			 */
			ClientArea * focusPane;

			/**
			 * The tab names for the sub client areas (panes) of this tabbing area.
			 */
			ArrayList<String> paneNames;

			/**
			 * The sub client areas (panes) of this tabbing area.
			 */
			ArrayList<ClientArea> panes;

			/**
			 * This method adjusts the position and size of all sub client areas
			 * (panes) according to the tabbing area's position and, size,
			 * and properties (e.g. tab area height).
			 */
			void adjustClientAreas();

		protected:

			/**
			 * Destroys the tabbing area.
			 */
			virtual ~TabbingArea();

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
				DECLARE_DYNAMIC(TabbingArea);
			#endif

		public:

			/**
			 * Creates a new tabbing area.
			 */
			TabbingArea();

			/**
			 * Adds a sub client area (pane) at the end of the list of panes.
			 * @param pane The pane to be added.
			 * @param name The name for the pane (usually displayed in the tab).
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the area.
			 * To print the ampersand character itself, use a double ampersand.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendPane(ClientArea * pane, const String & name);

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
			 * The focus pane e.g. receives all key events of the tabbing area.
			 * @return The current focus pane.
			 */
			virtual ClientArea * getFocusPane();

			/**
			 * Returns the name of a sub client area (pane).
			 * @param position The zero-indexed position of the pane to return the name of.
			 * @return The name of the pane.
			 */
			const String & getPaneName(int position);

			/**
			 * Returns the sub client areas (panes) of this tabbing area.
			 * @return The sub client areas (panes) of this tabbing area.
			 */
			virtual const ArrayList<ClientArea> & getPanes();

			/**
			 * Adds a sub client area (pane) before a given position.
			 * @param position The 0-indexed position to add the pane at.
			 * @param pane The pane to be added.
			 * @param name The name for the pane (usually displayed in the tab).
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the area.
			 * To print the ampersand character itself, use a double ampersand.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertPane(int position, ClientArea * pane, const String & name);

			/**
			 * Returns whether a given child component is visible (not hidden).
			 * This is true if and only if the child is visible itself, and this
			 * parent component does not define it to be hidden (e.g. not selected
			 * tabbing child client areas).
			 * @param child The child component. The component must be a child of this parent.
			 * @return Whether the component is visible.
			 * @note The default implementation returns whether the child is visible itself.
			 */
			virtual bool isChildVisible(Component * child);

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
			 * @param pane The pane to be added.
			 * @param name The name for the pane (usually displayed in the tab).
			 * The text may contain a hot key character (preceded by an ampersand character).
			 * Pressing the hot key will set the focus to the area.
			 * To print the ampersand character itself, use a double ampersand.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependPane(ClientArea * pane, const String & name);

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
			 * The focus pane e.g. receives all key events of the tabbing area then.
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
			 * Changes the name of a sub client area (pane).
			 * @param position The zero-indexed position of the pane to be renamed.
			 * @param name The new name of the pane.
			 * An ampersand character indicates a hot key,
			 * a double ampersand print the ampersand character itself.
			 */
			void setPaneName(int position, const String & name);
	};
}


#endif
