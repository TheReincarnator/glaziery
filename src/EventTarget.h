/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_EVENTTARGET_H
#define __GLAZIERY_EVENTTARGET_H


namespace glaziery
{
	/**
	 * A event target is an object that can be the target of pointer events.
	 * E.g. you can click on it. Pointer targets are components, widgets, etc.
	 * @see Component
	 * @see Window
	 * @see Widget
	 */
	class EventTarget : public Listenable
	{
		public:

			/**
			 * The base listener class for all event target events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * event target anymore. So you should not hold any references to them.
			 */
			class Listener : public Listenable::Listener
			{
				friend EventTarget;

				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					/**
					 * Called when any key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * This method is called in addition to another key method,
					 * such as onCharacter or onMoveDown. The onAnyKey method is called first.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onAnyKey(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the BACKSPACE key (backward delete)
					 * is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 */
					virtual void onBackSpace(EventTarget * target);

					/**
					 * Called when the back TAB key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * On Windows systems, the back TAB key is SHIFT+TAB.
					 * Back tabs step the focus from field to field in backward order.
					 * @param target The target that received the event.
					 * @param secondary Whether the secondary tabbing is used.
					 * E.g., on Windows systems, this is done by holding the control key.
					 */
					virtual void onBackTab(EventTarget * target, bool secondary);

					/**
					 * Called when a regular character has been entered
					 * on the event target, the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * This is used for text input fields to enter text.
					 * @param target The target that received the event.
					 * @param character The character pressed, using ISO-8859-1 encoding.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onCharacter(EventTarget * target, char character, bool option1, bool option2);

					/**
					 * Called when the target is clicked (usually using the left mouse button).
					 * A click is raised after the mouse has been pressed and
					 * released, and no dragging operation has started.
					 * @param target The target that received the event.
					 * @param position The position where the click occurred
					 * relative to the upper-left corner of the target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onPress()
					 * @see onRelease()
					 */
					virtual void onClick(EventTarget * target, Vector position,
						bool option1, bool option2);

					/**
					 * Called when the target is context-clicked
					 * (usually using the right mouse button).
					 * @param target The target that received the event.
					 * @param position The position where the context click occurred
					 * relative to the upper-left corner of the target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onContextClick(EventTarget * target, Vector position,
						bool option1, bool option2);

					/**
					 * Called when the event target should perform a clipboard copy.
					 * @param target The target that received the event.
					 */
					virtual void onCopy(EventTarget * target);

					/**
					 * Called when the event target should perform a clipboard cut.
					 * @param target The target that received the event.
					 */
					virtual void onCut(EventTarget * target);

					/**
					 * Called when the DELETE key (forward delete)
					 * is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 */
					virtual void onDelete(EventTarget * target);

					/**
					 * Called when the target is double-clicked (usually using the left mouse button).
					 * A double click is raised when the mouse is pressed, released, and pressed
					 * again within a short time. In this case, the entire event sequence is:
					 * onPress, onRelease, onClick, onPress, onDoubleClick, onRelease, onClick,
					 * so the double-click is raised in response to the second press.
					 * @param target The target that received the event.
					 * @param position The position where the double click occurred
					 * relative to the upper-left corner of the target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onClick()
					 */
					virtual void onDoubleClick(EventTarget * target, Vector position,
						bool option1, bool option2);

					/**
					 * Called when a dragging operation successfully ended.
					 * This is when a dragging operation is in progress and the primary pointer
					 * button has been released.
					 * @param draggedObject The event target that has been dragged.
					 * @param dropTarget The event target where the object has been dropped.
					 * @param relativePosition The relative position within the drop target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onDropped(EventTarget * draggedObject, EventTarget * dropTarget,
						Vector relativePosition, bool option1, bool option2);

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
					virtual void onEnter(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when a hot key has been pressed on an event target.
					 * Hot keys represent special characters e.g. to easily access fields
					 * (indicated by underlined characters).
					 * @param target The target that received the event.
					 * @param character The hot key character pressed.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onHotKey(EventTarget * target, char character, bool option1, bool option2);

					/**
					 * Called when the pointer is hovered over the event target for a while.
					 * @param target The target that received the event.
					 */
					virtual void onHover(EventTarget * target);

					/**
					 * Called when a planned key stroke has been pressed
					 * on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * Key strokes are usually received by key stroke fields.
					 * When the desktop is in key stroke mode, the next key pressed
					 * should be sent to this method.
					 * This is useful e.g. to define keyboard layouts for games.
					 * @param target The target that received the event.
					 * @param keyCode The key stroke key code.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see Desktop::isKeyStrokeMode()
					 */
					virtual void onKeyStroke(EventTarget * target, int keyCode, bool option1, bool option2);

					/**
					 * Called when the DOWN key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveDown(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the LEFT key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveLeft(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the RIGHT key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveRight(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the END key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveToEnd(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the START key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveToStart(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the UP key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveUp(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the PAGE DOWN key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onPageDown(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the PAGE UP key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onPageUp(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the event target should perform a clipboard paste.
					 * @param target The target that received the event.
					 */
					virtual void onPaste(EventTarget * target);

					/**
					 * Called when the target is pressed down
					 * (usually using the left mouse button).
					 * @param target The target that received the event.
					 * @param position The position where the click occurred
					 * relative to the upper-left corner of the target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onRelease()
					 */
					virtual void onPress(EventTarget * target, Vector position, bool option1, bool option2);

					/**
					 * Called when the ointer target press is released.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onPress()
					 */
					virtual void onRelease(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the user requested to select all
					 * (e.g. by pressing Ctrl+A on Windows systems),
					 * when the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 */
					virtual void onSelectAll(EventTarget * target);

					/**
					 * Called when the TAB key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * Tabs step the focus from field to field.
					 * @param target The target that received the event.
					 * @param secondary Whether the secondary tabbing is used.
					 * E.g., on Windows systems, this is done by holding the control key.
					 */
					virtual void onTab(EventTarget * target, bool secondary);

					/**
					 * Called when a pointer wheel (mouse, joystick, etc.)
					 * is changed while the mouse is over this event target.
					 * @param target The target that received the event.
					 * @param delta The relative wheel movement.
					 */
					virtual void onWheel(EventTarget * target, int delta);
				};

		friend ArrayList<EventTarget>;
		friend Desktop;

		private:

			/**
			 * The tooltip, a text that appears in a popup when the pointer is hovered over the event target.
			 */
			String tooltip;

		protected:

			/**
			 * Destroys the event target.
			 */
			virtual ~EventTarget();

			/**
			 * Executes all deferred operations in this event target.
			 * Override, but super-call if you provide any deferred operations.
			 */
			virtual void executeDeferrals();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(EventTarget);
			#endif

		public:

			/**
			 * Returns the origin for relative position values.
			 * E.g. for components, this is their absolute position,
			 * and for widgets, it is the absolute position of their component.
			 */
			virtual Vector getOrigin() = NULL;

			/**
			 * Returns the parent component, usually the container
			 * (e.g. the window for the client area).
			 * @return The parent component.
			 */
			virtual Component * getParent() = NULL;

			/**
			 * Returns the tooltip, a text that appears in a popup when the pointer is hovered over the event target.
			 * @return The tooltip.
			 */
			const String & getTooltip();

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
			 * is pressed on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onBackSpace();

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
			 * Handles the cancel event on the event target.
			 * On Windows systems, this is the ESC key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCancel();

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
			 * Handles the event that the event target is clicked
			 * (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
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
			 * is pressed on the event target.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onDelete();

			/**
			 * Handles the event that the target is double-clicked (usually using the left mouse button).
			 * A double click is raised when the mouse is pressed, released, and pressed
			 * again within a short time. In this case, the entire event sequence is:
			 * onPress, onRelease, onClick, onPress, onDoubleClick, onRelease, onClick,
			 * so the double-click is raised in response to the second press.
			 * @param position The position where the double click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onDoubleClick(Vector position, bool option1, bool option2);

			/**
			 * Handles the event that the pointer is moved during a dragging operation.
			 * @param startPosition The absolute position where the dragging started.
			 * @param lastPosition The absolute position before the last move step.
			 * @param currentPosition The new absolute position.
			 */
			virtual void onDragMove(const Vector & startPosition,
				const Vector & lastPosition, const Vector & currentPosition);

			/**
			 * Handles the event that a dragging operation started.
			 * This is when the component was pressed using the primary pointer
			 * button and then the pointer has been moved a little.
			 * @param startPosition The absolute position where the dragging started.
			 * @return The dragging surrogate to be drawn next to the pointer
			 * while the dragging operation is running. This surrogate is a representative
			 * of the dragged object (event target, list field row, etc.).
			 * NULL indicates that dragging is not supported, which is the default.
			 * The DraggingSurrogate class itself can be used to support dragging
			 * without drawing a different pointer etc.
			 */
			virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

			/**
			 * Handles the event that the EDIT key is pressed on the event target.
			 * On Windows systems, this is the F2 key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onEdit();

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
			 * Handles the event that the pointer is hovered over the event target for a while.
			 */
			virtual void onHover();

			/**
			 * Handles the event that a planned key stroke has been pressed
			 * on the event target.
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

			/**
			 * Handles the event that the event target press is released.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onHold()
			 * @see onPress()
			 */
			virtual void onRelease(bool option1, bool option2);

			/**
			 * Handles the event that the user requested to select all
			 * (e.g. by pressing Ctrl+A on Windows systems).
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onSelectAll();

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
			 * Handles the event that a pointer wheel (mouse, joystick, etc.)
			 * is changed while the mouse is over this event target.
			 * @param delta The relative wheel movement.
			 */
			virtual void onWheel(int delta);

			/**
			 * Sets the tooltip, a text that appears in a popup when the pointer is hovered over the event target.
			 * @param tooltip The tooltip.
			 */
			void setTooltip(const String & tooltip);
	};
}


#endif
