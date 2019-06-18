/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_COMPONENT_H
#define __GLAZIERY_COMPONENT_H


namespace glaziery
{
	/**
	 * Component is the super class for all visible windows and elements.
	 * It has a position and size, listens to user and other events
	 * and provides other listeners to register to the events received.
	 */
	class Component : public EventTarget
	{
		public:

			/**
			 * The base listener class for all component events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public EventTarget::Listener
			{
				friend Component;

				public:

					/**
					 * Called when the component is about to be destroyed (its memory
					 * is released). This method is called before the component is deleted,
					 * but its parent container references may have been released already.
					 * @param component The component that received the event.
					 */
					virtual void onDestroying(Component * component);

					/**
					 * Called when the component has just newly got the focus.
					 * The component with focus e.g. receives the key events.
					 * @param component The component that received the event.
					 * @param byParent If the parent got focus and this is propagated to the child.
					 * @see onLostFocus()
					 */
					virtual void onGotFocus(Component * component, bool byParent);

					/**
					 * Called when the component has been hidden.
					 * @param component The component that received the event.
					 * @see onShown()
					 */
					virtual void onHidden(Component * component);

					/**
					 * Called when the component has just lost the focus.
					 * The component with focus e.g. receives the key events.
					 * @param component The component that received the event, i.e.
					 * the component that lost the focus, not the now focused one.
					 * To see which component has the focus now, use Desktop::getFocusComponent().
					 * @see Desktop::getFocusComponent()
					 * @see onGotFocus()
					 */
					virtual void onLostFocus(Component * component);

					/**
					 * Called when the component has been moved.
					 * @param component The component that received the event.
					 * @param oldPosition The old position of the component before the move.
					 */
					virtual void onMoved(Component * component, Vector oldPosition);

					/**
					 * Called when the component has been resized.
					 * @param component The component that received the event.
					 * @param oldSize The old size of the component before the resizing.
					 */
					virtual void onResized(Component * component, Vector oldSize);

					/**
					 * Called when the component has been shown.
					 * @param component The component that received the event.
					 * @see onHidden()
					 */
					virtual void onShown(Component * component);
			};

		friend ArrayList<Component>;
		friend ComponentEffect;
		friend Desktop;
		friend GroupField;
		friend HashMap<Component>;
		friend Window;

		private:

			/**
			 * Whether the component has being destroyed by the destroy() method.
			 * Is has been placed into the list of disposable components and
			 * will be destroyed when the Desktop::draw() method is called next time
			 * (the next frame).
			 */
			bool disposed;

			/**
			 * All component effects currently running for this component.
			 */
			ArrayList<ComponentEffect> effects;

			/**
			 * The maximum size of the component. Resize operation are limited by this value.
			 */
			Vector maximumSize;

			/**
			 * The minimum size of the component. Resize operation are limited by this value.
			 */
			Vector minimumSize;

			/**
			 * The parent component, usually the container (e.g. the window
			 * of a client area).
			 */
			Component * parent;

			/**
			 * The position of the component relative to its parent.
			 */
			Vector position;

			/**
			 * The size of the component.
			 */
			Vector size;

			/**
			 * Whether the component is visible.
			 */
			bool visible;

			/**
			 * The deferred visible.
			 * @see Desktop::executeDeferrals()
			 */
			bool visibleDeferred;

			/**
			 * The widgets of this component. Pointer events are first passed to
			 * the widgets. Only if none matches, it is handled by the component.
			 */
			ArrayList<Widget> widgets;

			/**
			 * Adds a component effect running for this component.
			 * @param effect The component effect to be added.
			 */
			void addEffect(ComponentEffect * effect);

			/**
			 * Removes (forgets) a component effect running for this component.
			 * @param effect The component effect to be removed.
			 */
			void removeEffect(ComponentEffect * effect);

		protected:

			/**
			 * Destroys the component.
			 */
			virtual ~Component();

			/**
			 * Destroys a component, if it is a child of this component.
			 * If not (which is illegal), an exception is thrown.
			 * @param child The child to be destroyed.
			 */
			void deleteChild(Component * child);

			/**
			 * Executes all deferred operations in this event target.
			 * Override, but super-call if you provide any deferred operations.
			 * @see Desktop::executeDeferrals()
			 */
			virtual void executeDeferrals();

			/**
			 * Moves a related component (child, parent, ...) within its parent.
			 * No veto is possible, and the parent is not notified.
			 * @param relatedComponent The related component to be moved.
			 * @param position The new position relative to its parent.
			 * @note Use this method only from within components that have to move
			 * related components. This method delegates to the moveInternal() method.
			 * @see moveInternal()
			 * @see moveTo()
			 */
			void moveComponent(Component * relatedComponent, Vector position);

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
			 * Resizes a related component (child, parent, ...).
			 * No veto is possible, and the parent is not notified.
			 * @param relatedComponent The related component to be resized.
			 * @param size The new size.
			 * @note Use this method only from within components that have to move
			 * related components. This method delegates to the moveInternal() method.
			 * @see resize()
			 * @see resizeInternal()
			 */
			void resizeComponent(Component * relatedComponent, Vector size);

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

			/**
			 * Sets the parent of a related child component when adding the child to this component.
			 * You cannot change a parent once it has been set, except if you unset the parent first,
			 * which is not recommended for regular cases.
			 * @param child The child to set the parent to this component.
			 * @note Do not use this method in application (or game) code.
			 * Use this method in the parent component when implementing
			 * a method that adds a child (e.g. appendField in FieldsArea).
			 * @see unsetComponentParent()
			 * @see getParent()
			 */
			void setComponentParent(Component * child);

			/**
			 * Sets the maximum size of a component. Resize operation are limited by this value.
			 * If the current size is bigger than the maximum size, the component is resized automatically.
			 * @param component The component to set the maximum size of.
			 * @param maximumSize The maximum size of the component.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 */
			void setComponentMaximumSize(Component * component, Vector maximumSize, bool notifyParent);

			/**
			 * Sets the minimum size of a component. Resize operation are limited by this value.
			 * If the current size is smaller than the maximum size, the component is resized automatically.
			 * @param component The component to set the minimum size of.
			 * @param minimumSize The minimum size of the component.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 */
			void setComponentMinimumSize(Component * component, Vector minimumSize, bool notifyParent);

			/**
			 * Sets that the component is about to be destroyed. Usually, don't call this method directly.
			 * Use destroy() instead, which automatically deletes the object during the next frame.
			 */
			void setDisposed();

			/**
			 * Sets the maximum size of the component. Resize operation are limited by this value.
			 * If the current size is bigger than the maximum size, the component is resized automatically.
			 * @param maximumSize The maximum size of the component.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 */
			void setMaximumSizeInternal(Vector maximumSize, bool notifyParent);

			/**
			 * Sets the minimum size of the component. Resize operation are limited by this value.
			 * If the current size is smaller than the maximum size, the component is resized automatically.
			 * @param minimumSize The minimum size of the component.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 */
			void setMinimumSizeInternal(Vector minimumSize, bool notifyParent);

			/**
			 * Unsets the parent of a related child component in the rare cases of removing
			 * the child from this component again.
			 * @param child The child to unset the parent of.
			 * @note Do not use this method in application (or game) code.
			 * Use this method in the parent component when implementing
			 * a method that removes a child (e.g. PopupWindow).
			 * @see setComponentParent()
			 * @see getParent()
			 */
			void unsetComponentParent(Component * child);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Component);
			#endif

		public:

			/**
			 * Creates a new component.
			 */
			Component();

			/**
			 * Adds a component widget at the end of the widgets of this component.
			 * Appended widgets are considered last.
			 * @param widget The widget to be added.
			 * @note Widgets may only be added to one component at a time.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendWidget(Widget * widget);

			/**
			 * Cancels all effects currently running for this component.
			 * This method is useful before added new effects.
			 */
			void cancelEffects();

			/**
			 * Moves the component to be located in the center of its parent.
			 */
			void center();

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
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context) = NULL;

			/**
			 * Returns the absolute (Desktop relative) position of this component.
			 * @return The absolute position.
			 */
			Vector getAbsolutePosition();

			/**
			 * Returns the component effects currently running for this component.
			 * @return The component effects.
			 */
			const ArrayList<ComponentEffect> & getEffects();

			/**
			 * Returns the origin of children of this component.
			 * Typically it is the absolute position of this component.
			 * However, components with scrolling child views (e.g. the ScrollingArea), have a different origin.
			 * @return The children origin.
			 */
			virtual Vector getChildrenOrigin();

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
			 * Returns the maximum size of the component. Resize operation are limited by this value.
			 * @return The maximum size of the component.
			 */
			Vector getMaximumSize();

			/**
			 * Returns the minimum size of the component. Resize operation are limited by this value.
			 * @return The minimum size of the component.
			 */
			Vector getMinimumSize();

			/**
			 * Returns the origin for relative position values.
			 * E.g. for components, this is their absolute position,
			 * and for widgets, it is the absolute position of their component.
			 */
			virtual Vector getOrigin();

			/**
			 * Returns the parent component, usually the container
			 * (e.g. the window of a client area).
			 * @return The parent component.
			 */
			virtual Component * getParent();

			/**
			 * Returns the position of the component relative to its parent.
			 * @return The position of the component relative to its parent.
			 */
			Vector getPosition();

			/**
			 * Returns the size of the component.
			 * @return The size of the component.
			 */
			Vector getSize();

			/**
			 * Returns all currently registered component widgets.
			 * @return The component widgets.
			 */
			const ArrayList<Widget> & getWidgets();

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
			 * Defers the hide operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @see Desktop::deferOperation()
			 */
			void hideDeferred();

			/**
			 * Invalidates this component's internal caches (e.g. drawing caches).
			 * This method should be called whenever its state changes.
			 * The invalidation is propagated to the parent, if any.
			 */
			void invalidate();

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
			 * Returnw whether the component has being destroyed by the destroy() method.
			 * Is has been placed into the list of disposable components and
			 * will be destroyed when the Desktop::draw() method is called next time
			 * (the next frame).
			 * @return Whether the component is disposed of.
			 */
			bool isDisposed();

			/**
			 * Returns whether the component is visible (not hidden).
			 * @return Whether the component is visible.
			 */
			bool isVisible();

			/**
			 * Returns whether the component and all its ancestors are visible (not hidden).
			 * @return Whether the component and all its ancestors are visible.
			 */
			bool isVisibleIncludingAncestors();

			/**
			 * Moves the component relatively. This calls the listeners' onMoved() method.
			 * @param delta The translation relative to the current position.
			 * @return Whether the component was actually moved.
			 */
			bool moveRelative(Vector delta);

			/**
			 * Moves the component absolutely within its parent. This calls the listeners' onMoved() method.
			 * @param position The new position relative to its parent.
			 * @return Whether the component was actually moved.
			 */
			virtual bool moveTo(Vector position);

			/**
			 * Handles the event that any key event occurred.
			 * This method is called in addition to another key method,
			 * such as onCharacter or onMoveDown. The onAnyKey method is called first.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onAnyKey(bool option1, bool option2);

			/**
			 * Handles the event that the BACKSPACE key (backward delete)
			 * is pressed on the component.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onBackSpace();

			/**
			 * Handles the event that the back TAB key is pressed on the component.
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
			 * Handles the cancel event on the component.
			 * On Windows systems, this is the ESC key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCancel();

			/**
			 * Handles the event that a regular character has been entered
			 * on the component.
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
			 * Handles the event that the event target is context-clicked
			 * (usually using the right mouse button).
			 * @param position The position where the context click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContextClick(Vector position, bool option1, bool option2);

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
			 * is pressed on the component.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onDelete();

			/**
			 * Notifies all component listeners about the destruction
			 * of the component, but does not delete it nor adds it to
			 * the list of disposable components.
			 * @note Override this method if you require pre-deletion operations.
			 * The default implementation notifies listeners only.
			 */
			virtual void onDestroying();

			/**
			 * Handles the event that the EDIT key is pressed on the component.
			 * On Windows systems, this is the F2 key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onEdit();

			/**
			 * Handles the event that the ENTER key is pressed on the component.
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
			 * Handles the event that a hot key has been pressed on the component.
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
			 * Handles the event that a planned key stroke has been pressed
			 * on the component.
			 * Key strokes are usually received by key stroke fields.
			 * When the desktop is in key stroke mode, the next key pressed
			 * should be sent to this method.
			 * This is useful e.g. to define keyboard layouts for games.
			 * @param keyCode The key stroke key code.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 * @see Desktop::isKeyStrokeMode()
			 */
			virtual bool onKeyStroke(int keyCode, bool option1, bool option2);

			/**
			 * First notifies the listeners of the deepest grand-child of a focus loss,
			 * then its parent, and so on, until this component is reached,
			 * which is then notified (calling the listeners' onLostFocus() method).
			 * @see Listener::onLostFocus()
			 */
			virtual void onLostFocus();

			/**
			 * Handles the event that the DOWN key is pressed on the component.
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
			 * Handles the event that the LEFT key is pressed on the component.
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
			 * Handles the event that the RIGHT key is pressed on the component.
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
			 * Handles the event that the END key is pressed on the component.
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
			 * Handles the event that the START (HOME, POS1, ...) key is pressed on the component.
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
			 * Handles the event that the UP key is pressed on the component.
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
			 * Handles the event that the TAB key is pressed on the component.
			 * Tabs step the focus from field to field.
			 * @param secondary Whether the secondary tabbing is used.
			 * E.g., on Windows systems, this is done by holding the control key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onTab(bool secondary);

			/**
			 * Adds a component widget at the beginning of the widgets of this component.
			 * Prepended widgets are considered first.
			 * @param widget The widget to be added.
			 * @note Widgets may only be added to one component at a time.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependWidget(Widget * widget);

			/**
			 * Removes a widget from the list of component widgets and then deletes it.
			 * @param widget The widget to be removed and deleted.
			 */
			void removeWidget(Widget * widget);

			/**
			 * Resizes the component. This calls the listeners' onResized() method.
			 * @param size The new size.
			 * @return Whether the component was actually resized.
			 * @note Calling this method on fields or field areas relayouts the fields area,
			 * if automatic layouting is enabled, so consider turning it off while doing major changes
			 * to the fields of such fields area.
			 */
			bool resize(Vector size);

			/**
			 * Resizes the component to the maximum size.
			 * @return Whether the component was actually resized.
			 * @note Calling this method on fields or field areas relayouts the fields area,
			 * if automatic layouting is enabled, so consider turning it off while doing major changes
			 * to the fields of such fields area.
			 */
			bool resizeToMaximum();

			/**
			 * Resizes the component to the minimum size.
			 * @return Whether the component was actually resized.
			 * @note Calling this method on fields or field areas relayouts the fields area,
			 * if automatic layouting is enabled, so consider turning it off while doing major changes
			 * to the fields of such fields area.
			 */
			bool resizeToMinimum();

			/**
			 * Sets the maximum size of the component. Resize operation are limited by this value.
			 * If the current size is bigger than the maximum size, the component is resized automatically.
			 * @param maximumSize The maximum size of the component.
			 */
			void setMaximumSize(Vector maximumSize);

			/**
			 * Sets the minimum size of the component. Resize operation are limited by this value.
			 * If the current size is smaller than the maximum size, the component is resized automatically.
			 * @param minimumSize The minimum size of the component.
			 */
			void setMinimumSize(Vector minimumSize);

			/**
			 * Sets the skin specific settings and other skin data.
			 * The skin data is managed by the skin. If you want to implement a skin
			 * independent application (or game), do not use this object outside the skin.
			 * However, if you know which skin is in use and you require to read or write
			 * special property, you may cast this object to the respective type.
			 * This object may be NULL, if the skin does not need to store any data.
			 * @param skinData The skin specific settings and other skin data.
			 * @note If the object is not null when the component is destroyed,
			 * then the skin data is automatically destroyed.
			 * @note Any previously set skin data is automatically destroyed.
			 */
			virtual void setSkinData(SkinData * skinData);

			/**
			 * Shows or hides the component (makes it visible or invisible).
			 * @note This is a convenience method, it delegates to show() or hide().
			 * @param visible The new visibility.
			 * @see show()
			 * @see hide()
			 */
			void setVisible(bool visible);

			/**
			 * Defers the setVisible operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @note This is a convenience method, it delegates to showDeferred() or hideDeferred().
			 * @param visible The new visibility.
			 * @see Desktop::deferOperation()
			 * @see showDeferred()
			 * @see hideDeferred()
			 */
			void setVisibleDeferred(bool visible);

			/**
			 * Shows the component (makes it visible again).
			 */
			virtual void show();

			/**
			 * Shows a balloon popup for this field, containing a given text.
			 * @param text The text to show.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(const String & text);

			/**
			 * Defers the show operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @see Desktop::deferOperation()
			 */
			void showDeferred();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * For tutorials: Creates a pointer effect that moves the pointer to this component
			 * and clicks the component, and waits until the effect has ended.
			 * @param buttonEffect The button effect (see PointerEffect::ButtonEffect) to be added
			 * (clicking, double-clicking, right-clicking the component). Only BUTTONEFFECT_LEFTCLICK,
			 * BUTTONEFFECT_DOUBLECLICK, and BUTTONEFFECT_RIGHTCLICK is allowed. To drag this component,
			 * use tutorialDragDropTo. Defaults to BUTTONEFFECT_LEFTCLICK;
			 * @param option1 Whether option1 (e.g. Shift) is held during the button effect. Defaults to false.
			 * @param option2 Whether option2 (e.g. Ctrl) is held during the button effect. Defaults to false.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialClick(PointerEffect::ButtonEffect buttonEffect = PointerEffect::BUTTONEFFECT_LEFTCLICK,
				bool option1 = false, bool option2 = false, long time = -1);

			/**
			 * For tutorials: Creates a pointer effect that drags the component to another location and drops there,
			 * and waits until the effect has ended.
			 * @param position The absolute position to drag-drop to.
			 * @param option1 Whether option1 (e.g. Shift) is held during the button effect. Defaults to false.
			 * @param option2 Whether option2 (e.g. Ctrl) is held during the button effect. Defaults to false.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialDragDropTo(Vector position, bool option1 = false, bool option2 = false, long time = -1);

			/**
			 * For tutorials: Creates a pointer effect that moves the pointer to this component
			 * and waits until the effect has ended.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialMoveTo(long time = -1);
	};
}


#endif
