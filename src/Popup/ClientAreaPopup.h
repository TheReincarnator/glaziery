/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CLIENTAREAPOPUP_H
#define __GLAZIERY_CLIENTAREAPOPUP_H


namespace glaziery
{
	/**
	 * The client area popup is a popup window that is a container for a client area,
	 * e.g. a fields area. The client area uses the whole popup window's size.
	 * It is automatically resized together with the area.
	 */
	class ClientAreaPopup : public Popup
	{
		private:

			/**
			 * Whether to destroy the content (child) client area on popup destruction.
			 * Defaults to true.
			 */
			bool autoDestroying;

			/**
			 * The content (child) client area that uses the whole popup window's space.
			 */
			ClientArea * clientArea;

			/**
			 * This method adjusts the child client areas position and size
			 * according to the client area popup's position and size.
			 */
			void adjustArea();

		protected:

			/**
			 * Destroys the client area popup.
			 */
			virtual ~ClientAreaPopup();

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
			 * Notifies the component that the maximum size of one of its children has changed.
			 * The default implementation does nothing.
			 * @param child The child that changed.
			 * @param oldMaximumSize The old maximum size.
			 */
			virtual void onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize);

			/**
			 * Notifies the component that the minimum size of one of its children has changed.
			 * The default implementation does nothing.
			 * @param child The child that changed.
			 * @param oldMaximumSize The old minimum size.
			 */
			virtual void onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize);

			/**
			 * Notifies the component that the maximum size has changed.
			 * The default implementation does nothing.
			 * @param oldMinimumSize The old maximum size.
			 */
			virtual void onMaximumSizeChanged(Vector oldMaximumSize);

			/**
			 * Notifies the component that the minimum size has changed.
			 * The default implementation does nothing.
			 * @param oldMinimumSize The old minimum size.
			 */
			virtual void onMinimumSizeChanged(Vector oldMinimumSize);

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
				DECLARE_DYNAMIC(ClientAreaPopup);
			#endif

		public:

			/**
			 * Creates a new client area popup.
			 */
			ClientAreaPopup();

			/**
			 * Draws the content of the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The implementation must clear the context before rendering,
			 * if it does not render to the entire context.
			 * @note This method is called when the cache is invalid only.
			 * See invalidate() and the CachableComponent's class comment for details.
			 * If the cache never gets valid (because the display depends on some event
			 * that is not pushed into the application or game), call invalidate() from
			 * within the implementation of drawContent().
			 * @see Desktop::getSkin()
			 * @see CachableComponent::invalidate()
			 * @see CachableComponent
			 */
			virtual void drawContent(DrawingContext & context);

			/**
			 * Returns the content (child) client area that uses the whole client area's space.
			 * @return The content (child) client area.
			 */
			ClientArea * getContentArea();

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
			 * Returns whether to destroy the content (child) client area on popup destruction.
			 * Defaults to true.
			 * @return Whether to destroy the client area on popup destruction.
			 */
			bool isAutoDestroying();

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
			 * Sets whether to destroy the content (child) client area on popup destruction.
			 * Defaults to true.
			 * @param autoDestroying Whether to destroy the client area on popup destruction.
			 */
			void setAutoDestroying(bool autoDestroying);

			/**
			 * Sets the content (child) client area that uses the whole client area's space.
			 * @param clientArea The new content (child) client area.
			 * Any previous client area is automatically deleted.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void setContentArea(ClientArea * clientArea);
	};
}


#endif
