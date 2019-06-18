/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WINDOW_H
#define __GLAZIERY_WINDOW_H


namespace glaziery
{
	/**
	 * A window is a cachable component with capabilities that enable
	 * the user to handle it like a window in the Microsoft Windows(tm) GUI.
	 * This includes a title bar, resizability, maximize widget, etc.
	 * Windows consist of a frame and a client area. The client area
	 * is the rectangular region that remains when leaving out the frame area.
	 * The client area is a separate component. It is set into the window.
	 * @warning If you implement a class that derives from Window,
	 * remember to call invalidate() in every method that changes something
	 * concerning its graphical representation (in any skin, if applicable).
	 * @see ClientArea
	 * @see invalidate()
	 */
	class Window : public CachableComponent
	{
		public:

			/**
			 * The base listener interface for all window events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public Component::Listener
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
					 * Called when the window has been closed
					 * (using the close widget or close() method).
					 * @param window The window that received the event.
					 * @see onClosing()
					 * @see Window::close()
					 */
					virtual void onClosed(Window * window);

					/**
					 * Called when the window is about to be closed
					 * (using the close widget or close() method).
					 * The listener may cancel this event by vetoing it.
					 * @param window The window that received the event.
					 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
					 * @see VetoMode
					 * @see onClosed()
					 * @see Window::close()
					 */
					virtual VetoMode onClosing(Window * window);

					/**
					 * Called when the window has been maximized
					 * (using the maximize widget or maximize() method).
					 * @param window The window that received the event.
					 * @see onRestored()
					 */
					virtual void onMaximized(Window * window);

					/**
					 * Called when the window has been restored from maximize
					 * (using the maximize widget or restore() method).
					 * @param window The window that received the event.
					 * @see onMaximized()
					 */
					virtual void onRestored(Window * window);

					/**
					 * Called when the window status bar text or the statused flag has been changed.
					 * @param window The window that received the event.
					 */
					virtual void onStatusChanged(Window * window);

					/**
					 * Called when the window title text or the titled flag has been changed.
					 * @param window The window that received the event.
					 */
					virtual void onTitleChanged(Window * window);
			};

		private:

		friend ArrayList<Window>;
		friend Desktop;

		private:

			/**
			 * Whether the consumes and executes events, e.g. by changing
			 * properties and/or notifying listeners. If set to false, events
			 * "fall through" this window to the next one below, resp. finally
			 * to the Desktop.
			 */
			bool active;

			/**
			 * Whether the window has already been added to the Desktop.
			 */
			bool added;

			/**
			 * Whether cancel events close the window. Defaults to true.
			 * Has effect only when closable is set, too.
			 */
			bool cancelClosing;

			/**
			 * The client area of the window. The client area is the rectangular
			 * region that remains when leaving out the frame area.
			 * The client area is a separate component and must be set into the window.
			 */
			ClientArea * clientArea;

			/**
			 * Whether the user can close the window
			 * using a special close widget.
			 * The actual UI is defined by the skin.
			 */
			bool closable;

			/**
			 * Whether this window is already on the way to be destroyed,
			 * i.e. the destroy effect is already issued.
			 */
			bool destroying;

			/**
			 * Whether this window consumes key and pointer events, i.e. it has a higher priority.
			 * Defaults to false for regular windows, and to true for modal windows, such as dialogs.
			 */
			bool eventConsuming;

			/**
			 * Whether the last onPress focused the window.
			 * This is handy e.g. in field elements to determine whether the click should edit the element.
			 */
			bool lastPressFocused;

			/**
			 * Whether the user can maximize/restore the window
			 * using a special maximize widget.
			 * The actual UI is defined by the skin.
			 */
			bool maximizable;

			/**
			 * Whether the window is currently maximized.
			 * In this state, the regularSize and regularPosition
			 * carry the original properties of the window, before
			 * it has been maximized.
			 * @see maximize()
			 * @see restore()
			 */
			bool maximized;

			/**
			 * The menu of this window. May be NULL, if the window has no menu.
			 */
			Menu * menu;

			/**
			 * Whether the user can move the window
			 * e.g. dragging the title bar.
			 * The actual UI is defined by the skin.
			 */
			bool movable;

			/**
			 * Whether the pointer is shown if the desktop is in automatic pointer mode
			 * and this window has focus. Defaults to true.
			 * @see Desktop::showPointerAutomatically()
			 * @see Desktop::showPointerAlways()
			 * @see Desktop::hidePointer()
			 */
			bool pointerShown;

			/**
			 * The position of the window, before it has been maximized.
			 * The value of this attribute is valid only when it is maximized.
			 * When restoring the window, this value is used to move the window.
			 * @see maximized
			 * @see position
			 * @see maximize()
			 * @see restore()
			 */
			Vector regularPosition;

			/**
			 * The size of the window, before it has been maximized.
			 * The value of this attribute is valid only when it is maximized.
			 * When restoring the window, this value is used to resize the window.
			 * @see maximized
			 * @see size
			 * @see maximize()
			 * @see restore()
			 */
			Vector regularSize;

			/**
			 * Whether the user can resize the window
			 * e.g. dragging the frame border.
			 * The actual UI is defined by the skin.
			 */
			bool resizable;

			/**
			 * The window status bar text, displayed if the statused attribute is set.
			 * @see statused
			 */
			String status;

			/**
			 * Whether the window is set to display the status bar. Defaults to false.
			 * @see title
			 */
			bool statused;

			/**
			 * Whether the window should stay on top of all non-top windows.
			 */
			bool stayOnTop;

			/**
			 * The window title, displayed if the titled attribute is set.
			 * @see titled
			 */
			String title;

			/**
			 * Whether the window is set to display the window title. Defaults to true.
			 * @see title
			 */
			bool titled;

			/**
			 * The deferred window title.
			 * @see Desktop::executeDeferrals()
			 */
			String titleDeferred;

			/**
			 * If this window has a client area, this method adjusts its
			 * position and size according to the window's position, size,
			 * and properties (e.g. menu and titled).
			 */
			void adjustClientArea();

			/**
			 * Sets that this window has already been added to the desktop.
			 */
			void setAdded();

		protected:

			/**
			 * Destroys the window.
			 */
			virtual ~Window();

			/**
			 * Executes all deferred operations in this event target.
			 * Override, but super-call if you provide any deferred operations.
			 * @see Desktop::executeDeferrals()
			 */
			virtual void executeDeferrals();

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
			 * @note If the window is currently maximized, then this method
			 * has no immediate effect. However, calling restore() any later
			 * will restore the window with the new size.
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
				DECLARE_DYNAMIC(Window);
			#endif

		public:

			/**
			 * Creates a new window.
			 */
			Window();

			/**
			 * Brings this window to the foreground, in front of all other windows.
			 * The window automatically gets focus then.
			 * @param focus Whether this window should initially get focus. Defaults to true.
			 */
			void bringToFront(bool focus = true);

			/**
			 * Gracefully tries to close the window, removing it from the Glaziery system.
			 * This method should be used for regular window closes with user interaction.
			 * First, all window listeners are notified using onClosing().
			 * They may veto the window close. If noone does, they are again notified
			 * about the close having performed, using onClosed(). The window is then
			 * destroyed by adding it to the list of disposable components, which finally
			 * destroys it on next call of Desktop::draw().
			 * @note The component is maybe added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @note Use Desktop::removeWindow() to remove the window ungracefully,
			 * without user interaction and close notification.
			 * @return Whether the close was performed. Any veto results in false.
			 * @see Listener::onClosing()
			 * @see Listener::onClosed()
			 */
			bool close();

			/**
			 * Creates a menu for this window and sets it as the window's menu.
			 * If a menu already exists, the old one is automatically destroyed.
			 * @return The new menu for this window.
			 * @see getMenu()
			 */
			Menu * createMenu();

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
			 * Returns the client area of the window. The client area is the
			 * rectangular region that remains when leaving out the frame area.
			 * The client area is a separate component and must be set into the window.
			 * @return The client area.
			 */
			ClientArea * getClientArea();

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
			 * Returns the menu of this window. This method does not create
			 * one, so if the window does not have any menu, NULL is returned.
			 * @return The menu of this window or NULL, if the window has no menu.
			 * @see createMenu()
			 */
			Menu * getMenu();

			/**
			 * Returns the modulation color used when actually rendering the cachable component.
			 * @return The modulation color for rendering.
			 */
			virtual unsigned long getModulationColor();

			/**
			 * If the window is maximized, this method returns the position of the
			 * window, before it has been maximized.
			 * If it is restored, it returns its current position.
			 * @return The window's regular position.
			 * @see resize()
			 * @see maximize()
			 * @see restore()
			 */
			Vector getRegularPosition();

			/**
			 * If the window is maximized, this method returns the size of the
			 * window, before it has been maximized.
			 * If it is restored, it returns its current size.
			 * @return The window's regular size.
			 * @see move()
			 * @see maximize()
			 * @see restore()
			 */
			Vector getRegularSize();

			/**
			 * Returns the window status bar text, displayed if the statused attribute is set.
			 * @return The window status bar text.
			 * @see isStatused
			 * @see setStatused
			 */
			const String & getStatus();

			/**
			 * Returns the window title, displayed if the titled attribute is set.
			 * @return The window title.
			 * @see isTitled
			 * @see setTitled
			 */
			const String & getTitle();

			/**
			 * Returns whether the consumes and executes events, e.g. by changing
			 * properties and/or notifying listeners. If set to false, events
			 * "fall through" this window to the next one below, resp. finally
			 * to the Desktop.
			 * @reutrn Whether the consumes and executes events.
			 */
			bool isActive();

			/**
			 * Returns whether cancel events close the window. Defaults to true.
			 * Has effect only when closable is set, too.
			 * @return Whether cancel events close the window.
			 */
			bool isCancelClosing();

			/**
			 * Returns whether the user can close the window
			 * using a special close widget.
			 * The actual UI is defined by the skin.
			 * @return Whether the user can close the window.
			 */
			bool isClosable();

			/**
			 * Returns whether this window is already on the way to be destroyed,
			 * i.e. the destroy effect is already issued.
			 * @return Whether this window is already on the way to be destroyed.
			 */
			bool isDestroying();

			/**
			 * Returns whether this window consumes key and pointer events, i.e. it has a higher priority.
			 * Defaults to false for regular windows, and to true for modal windows, such as dialogs.
			 * @return Whether this window is event consuming.
			 */
			bool isEventConsuming();

			/**
			 * Return whether the last onPress focused the window.
			 * This is handy e.g. in field elements to determine whether the click should edit the element.
			 * @return Whether the last onPress focused the window.
			 */
			bool isLastPressFocused();

			/**
			 * Returns whether the user can maximize/restore the window
			 * using a special maximize widget.
			 * The actual UI is defined by the skin.
			 * @return Whether the user can maximize/restore the window.
			 */
			bool isMaximizable();

			/**
			 * Returns whether the window is currently maximized.
			 * @return Whether the window is currently maximized.
			 * @see maximize()
			 * @see restore()
			 */
			bool isMaximized();

			/**
			 * Returns whether the user can move the window
			 * e.g. dragging the title bar.
			 * The actual UI is defined by the skin.
			 * @return Whether the user can move the window.
			 */
			bool isMovable();

			/**
			 * Returns whether the pointer is shown if the desktop is in automatic pointer mode
			 * and this window has focus. Defaults to true.
			 * @return Whether the pointer is shown.
			 * @see Desktop::showPointerAutomatically()
			 * @see Desktop::showPointerAlways()
			 * @see Desktop::hidePointer()
			 */
			bool isPointerShown();

			/**
			 * Returns whether the user can resize the window
			 * e.g. dragging the frame border.
			 * The actual UI is defined by the skin.
			 * @return Whether the user can resize the window.
			 */
			bool isResizable();

			/**
			 * Returns whether the window is set to display the status bar. Defaults to false.
			 * @return Whether the window is set to display the status bar.
			 * @see getStatus()
			 * @see setStatus()
			 */
			bool isStatused();

			/**
			 * Returns whether the window should stay on top of all non-top windows.
			 * @return Whether the window should stay on top.
			 */
			bool isStayOnTop();

			/**
			 * Returns whether the window is set to display the window title. Defaults to true.
			 * @return Whether the window is set to display the window title.
			 * @see getTitle()
			 * @see setTitle()
			 */
			bool isTitled();

			/**
			 * Maximizes the window to full desktop size.
			 * The original position and size are memorized and will be restored
			 * when calling restore(). This method does the following:
			 * First, the position and size of the window is set to full desktop.
			 * Then, the maximize flag is set, so calling isMaximized() will then
			 * return true. Finally, all listeners' onMoved(), onResized(), and
			 * onMaximized() methods are called (in that order).
			 * @see restore()
			 * @see isMaximized()
			 * @see Component::Listener::onMoved()
			 * @see Component::Listener::onResized()
			 * @see Component::Listener::onMaximized()
			 */
			void maximize();

			/**
			 * Moves the window. This calls the listeners' onMoved() method.
			 * @note If the window is currently maximized, then this method
			 * has no immediate effect. However, calling restore() any later
			 * will restore the window at the new position.
			 * @param position The new position relative to its parent.
			 * @return Whether the window was actually moved.
			 * @see restore()
			 * @see maximize()
			 */
			virtual bool moveTo(Vector position);

			/**
			 * Handles the cancel event on the event target.
			 * On Windows systems, this is the ESC key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCancel();

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
			 * Handles the event that the close widget has been clicked.
			 * The default implementation calls close().
			 * @see close()
			 */
			virtual void onCloseWidgetClicked();

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
			 * Invalidates and releases this component's internal caches (e.g. drawing caches).
			 * This method should be when it is unlikely that the cache will be needed
			 * in the near future, to release system resources (e.g. when the component gets invisible).
			 */
			virtual void releaseCache();

			/**
			 * Removes the menu of this window, if it exists.
			 * The menu object is automatically destroyed.
			 */
			void removeMenu();

			/**
			 * Restores the window to the position and size before the last call
			 * to maximize(), except the moveTo() and/or resize() methods have been
			 * called after maximize(). In that case, the new position and/or size
			 * are restored.
			 * This method does the following: First, the position and size of the
			 * window are restored. Then, the maximize flag is reset, so calling
			 * isMaximized() will then return false. Finally, all listeners'
			 * onMoved(), onResized(), and onRestored() methods are called
			 * (in that order).
			 * @see maximize()
			 * @see moveTo()
			 * @see resize()
			 * @see isMaximized()
			 * @see Component::Listener::onMoved()
			 * @see Component::Listener::onResized()
			 * @see Component::Listener::onRestored()
			 */
			void restore();

			/**
			 * Sends this window to the background, behind all other windows.
			 * If any other window exists, the window automatically loses focus.
			 */
			void sendToBack();

			/**
			 * Sets whether the consumes and executes events, e.g. by changing
			 * properties and/or notifying listeners. If set to false, events
			 * "fall through" this window to the next one below, resp. finally
			 * to the Desktop.
			 * @param active Whether the consumes and executes events.
			 */
			void setActive(bool active);

			/**
			 * Sets whether cancel events close the window. Defaults to true.
			 * Has effect only when closable is set, too.
			 * @param cancelClosing Whether cancel events close the window.
			 */
			void setCancelClosing(bool cancelClosing);

			/**
			 * Sets the client area of the window. The client area is the
			 * rectangular region that remains when leaving out the frame area.
			 * The client area is a separate component and must be set into the window.
			 * @param clientArea The new client area.
			 * Any previous client area is automatically deleted.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void setClientArea(ClientArea * clientArea);

			/**
			 * Sets whether the user can close the window
			 * using a special close widget.
			 * The actual UI is defined by the skin.
			 * @param closable Whether the user can close the window.
			 */
			void setClosable(bool closable);

			/**
			 * Sets whether this window consumes key and pointer events, i.e. it has a higher priority.
			 * Defaults to false for regular windows, and to true for modal windows, such as dialogs.
			 * @param eventConsuming Whether this window is event consuming.
			 */
			void setEventConsuming(bool eventConsuming);

			/**
			 * Sets that this window gets focus.
			 * @note This method does not check whether the window is visible and active.
			 */
			virtual void setFocus();

			/**
			 * Sets whether the user can maximize/restore the window
			 * using a special maximize widget.
			 * The actual UI is defined by the skin.
			 * @param maximizable Whether the user can maximize/restore the window.
			 */
			void setMaximizable(bool maximizable);

			/**
			 * Sets whether the user can move the window
			 * e.g. dragging the title bar.
			 * The actual UI is defined by the skin.
			 * @param movable Whether the user can move the window.
			 */
			void setMovable(bool movable);

			/**
			 * Sets whether the pointer is shown if the desktop is in automatic pointer mode
			 * and this window has focus. Defaults to true.
			 * @param pointerShown Whether the pointer is shown.
			 * @see Desktop::showPointerAutomatically()
			 * @see Desktop::showPointerAlways()
			 * @see Desktop::hidePointer()
			 */
			void setPointerShown(bool pointerShown);

			/**
			 * Sets whether the user can resize the window
			 * e.g. dragging the frame border.
			 * The actual UI is defined by the skin.
			 * @param resizable Whether the user can resize the window.
			 */
			void setResizable(bool resizable);

			/**
			 * Sets the window status bar text, displayed if the statused attribute is set.
			 * @param status The new window status bar text.
			 * @see isStatused
			 * @see setStatused
			 */
			void setStatus(const String & status);

			/**
			 * Sets whether the window should stay on top of all non-top windows.
			 * @param stayOnTop Whether the window should stay on top.
			 */
			void setStayOnTop(bool stayOnTop);

			/**
			 * Sets whether the window is set to display the status bar. Defaults to false.
			 * @param statused Whether the window is set to display the status bar.
			 * @see getStatus()
			 * @see setStatus()
			 */
			void setStatused(bool statused);

			/**
			 * Sets the window title, displayed if the titled attribute is set.
			 * @param title The new window title.
			 * @see isTitled
			 * @see setTitled
			 */
			void setTitle(const String & title);

			/**
			 * Defers the setTitle operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param title The new window title.
			 * @see Desktop::addDeferredObject()
			 * @see Desktop::executeDeferrals()
			 * @see EventTarget::executeDeferrals()
			 */
			void setTitleDeferred(const String & title);

			/**
			 * Sets whether the window is set to display the window title. Defaults to true.
			 * @param titled Whether the window is set to display the window title.
			 * @see getTitle()
			 * @see setTitle()
			 */
			void setTitled(bool titled);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * If this window currently has focus, passes the focus to the top-most visible window,
			 * that does not match this window.
			 * @note The window is automatically unfocused, when it is destroyed.
			 * @see destroy()
			 */
			void unfocus();
	};
}


#endif
