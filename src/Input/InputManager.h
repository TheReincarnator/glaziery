/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTMANAGER_H
#define __GLAZIERY_INPUTMANAGER_H


namespace glaziery
{
	/**
	 * The input manager responsible for all input devices
	 * and their event handlings during the application. After having
	 * created is object and set up some events, handle must
	 * be called in regular intervals, then providing input
	 * information by calling GetEventQuantity.
	 * @note Normally, you do not need to override the handle method.
	 * But if you experience difficulties with the default
	 * behavior, feel free to do.
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class InputManager : public CObject, public Configurable
	#else
		class InputManager : public Configurable
	#endif
	{
		friend JoystickHandler;
		friend KeyboardHandler;
		friend KeyEvent;
		friend MouseHandler;

		// Configurable properties
		CF_INT_DECL(WheelLines)

		private:

			/**
			 * The currently maximum number of events managed (e.g. for the eventQuantities array).
			 */
			int eventCount;

			/**
			 * The event quantities array.
			 */
			long * eventQuantities;

			/**
			 * The global attachments set. Scene attachments are managed by the Scene.
			 * @see Scene
			 */
			InputAttachmentSet globalAttachments;

			/**
			 * Returns the joystick input handler of the platform adapter.
			 * @return The joystick input handler.
			 */
			JoystickHandler * getJoystickHandler();

			/**
			 * Returns the keyboard input handler of the platform adapter.
			 * @return The keyboard input handler.
			 */
			KeyboardHandler * getKeyboardHandler();

			/**
			 * Returns the mouse input handler of the platform adapter.
			 * @return The mouse input handler.
			 */
			MouseHandler * getMouseHandler();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(InputManager);
			#endif

		public:

			/**
			 * This attachment type represents 'axis movement',
			 * which means to increase the quantity by factor each step in the positive
			 * direction and to decrease by factor each step in the negative direction.
			 */
			static const int ATTACHMENT_AXISMOVEMENT;

			/**
			 * This attachment type represents 'axis position',
			 * which means to set the quantity to the axis position
			 * multiplied with factor.
			 */
			static const int ATTACHMENT_AXISPOSITION;

			/**
			 * This attachment type represents 'key hold time',
			 * which means to increase the quantity by factor each milli second.
			 */
			static const int ATTACHMENT_KEYHOLD;

			/**
			 * This attachment type represents 'key press',
			 * which means to increase the quantity by factor each key press.
			 */
			static const int ATTACHMENT_KEYPRESS;

			/**
			 * This attachment type represents 'key press and release',
			 * which means to increase the quantity by factor each key press or release.
			 */
			static const int ATTACHMENT_KEYPRESSRELEASE;

			/**
			 * This attachment type represents 'key press with repetition',
			 * which means to increase the quantity by factor each key press,
			 * and (if held) after KeyboardRepeatDelay each KeyboardRepeatRate
			 * milli seconds again by factor.
			 */
			static const int ATTACHMENT_KEYPRESSREPEAT;

			/**
			 * This attachment type represents 'key release',
			 * which means to increase the quantity by factor each key release.
			 */
			static const int ATTACHMENT_KEYRELEASE;

			/**
			 * This attachment type represents 'no attachment'.
			 */
			static const int ATTACHMENT_NONE;

			/**
			 * This key code represents the first joystick button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_JOYSTICK_BUTTON1;

			/**
			 * This key code represents the second joystick button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_JOYSTICK_BUTTON2;

			/**
			 * This key code represents the third joystick button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_JOYSTICK_BUTTON3;

			/**
			 * This key code represents the forth joystick button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_JOYSTICK_BUTTON4;

			/**
			 * This key code represents the left mouse button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_MOUSE_LEFT;

			/**
			 * This key code represents the middle mouse button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_MOUSE_MIDDLE;

			/**
			 * This key code represents the right mouse button and is
			 * used e.g. for attachKey and getLastKey.
			 */
			static const int KEY_MOUSE_RIGHT;

			/**
			 * No key has been pressed since the last call to resetLastKey().
			 * This value may be returned by the getLastKey method.
			 */
			static const int KEY_NONE;

			/**
			 * Creates a new input manager.
			 */
			InputManager();

			/**
			 * Destroys the input manager.
			 */
			virtual ~InputManager();

			/**
			 * Sets, that x movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick x movement.
			 * @param factor The amount to increase the x event quantity for each x step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachJoystickXMoveGlobal(int eventNumber, long factor);

			/**
			 * Sets, that the x position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick x position.
			 * @param factor The multiplier for the x position. This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachJoystickXPositionGlobal(int eventNumber, long factor);

			/**
			 * Sets, that y movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick y movement.
			 * @param factor The amount to increase the y event quantity for each y step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachJoystickYMoveGlobal(int eventNumber, long factor);

			/**
			 * Sets, that the y position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick y position.
			 * @param factor The multiplier for the y position. This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachJoystickYPositionGlobal(int eventNumber, long factor);

			/**
			 * Sets, that a keyboard key shall raise a certain event if pressed, released
			 * or held. To raise an event means to increase its quantity by a certain amount.
			 * @param eventNumber The number of the event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @param type The type of key action to raise the event. Possible values are:
			 *
			 * <ul>
			 * <li>GFKC_HOLD: The event is raised while the key is held by factor for each
			 * milli second.</li>
			 * <li>GFKC_PRESS: The event is raised once when the key is pressed.</li>
			 * <li>GFKC_PRESSREPEAT: The event is raised when the key is pressed, and when held
			 * according to keyboard repetition (KeyboardRepeatDelay, KeyboardRepeatRate).</li>
			 * <li>GFKC_RELEASE: The event is raised once when the key is released.</li>
			 * <li>GFKC_PRESSRELEASE: The event is raised once when the key is pressed
			 * and once when released.</li>
			 * </ul>
			 *
			 * @param factor The amount to increase the event quantity in case of an event.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachKeyOrButtonGlobal(int eventNumber, int keyCode, int type, long factor);

			/**
			 * Sets, that the X movements of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachMouseXMoveGlobal(int eventNumber, long factor);

			/**
			 * Sets, that the Y movements of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachMouseYMoveGlobal(int eventNumber, long factor);

			/**
			 * Sets, that the mouse wheel movement shall increase a certain event quantity.
			 * @param eventNumber The number of the event for the mouse wheel movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in all Scenes.
			 * If you wish to set an attachment in one Scene only, use the Scene's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void attachMouseWheelGlobal(int eventNumber, long factor);

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Removes all event attachments.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachAllGlobal();

			/**
			 * Removes all attachments of an event.
			 * @param eventNumber The number of the event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachGlobal(int eventNumber);

			/**
			 * Removes any attachment of the joystick X axis to an event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachJoystickXAxisGlobal();

			/**
			 * Removes any attachment of the joystick Y axis to an event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachJoystickYAxisGlobal();

			/**
			 * Removes any attachment of a key to an event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachKeyOrButtonGlobal(int keyCode);

			/**
			 * Removes any attachments of the mouse wheel to an event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachMouseWheelGlobal();

			/**
			 * Removes any attachment of the mouse X axis to an event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachMouseXMoveGlobal();

			/**
			 * Removes any attachment of the mouse Y axis to an event.
			 * @note This method removes attachments that are applied in all Scenes.
			 * If you wish to remove an attachment made to one Scene only, use the Scene's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see Scene
			 */
			void detachMouseYMoveGlobal();

			/**
			 * Checks whether the event count (for the event quantity array) is big enough
			 * for a requested event number. If not, the event count is increased and
			 * the quantities array is reallocated, saving the previous values into the new array.
			 * @param requestedNumber The requested event number.
			 */
			void ensureEventCount(int requestedNumber);

			/**
			 * Returns the key code of the attachment of a given event number.
			 * @param eventNumber The number of the event.
			 * @param positive Whether to search for attachments with positive factor.
			 * Defaults to true. May be combined with negative.
			 * @param negative Whether to search for attachments with positive factor.
			 * Defaults to true. May be combined with positive.
			 * @return The key code of the attachment found, or -1 if no attachment
			 * or more than one attachment was found.
			 * The key may include joystick and mouse buttons, specified by the key code constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 */
			int getAttachmentKeyOrButton(int eventNumber, bool positive = true, bool negative = true);

			/**
			 * Returns the joystick X axis attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveJoystickXAxisAttachment();

			/**
			 * Returns the joystick Y axis attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveJoystickYAxisAttachment();

			/**
			 * Returns the key or button attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @param keyCode The key code to return the attachment of. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveKeyOrButtonAttachment(int keyCode);

			/**
			 * Returns the mouse wheel attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveMouseWheelAttachment();

			/**
			 * Returns the mouse Y axis attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveMouseXAxisAttachment();

			/**
			 * Returns the mouse X axis attachment of the current scene, if any,
			 * otherwise the global attachment.
			 * @return The effective attachment.
			 */
			InputAttachment & getEffectiveMouseYAxisAttachment();

			/**
			 * Returns the number of events managed (e.g. for the EventQuantities array).
			 * @return The number of events managed.
			 */
			int getEventCount();

			/**
			 * Returns the event quantity of an event. If this is no absolute
			 * quantity (like joystick position), it is also reset.
			 * @param eventNumber The number of the event.
			 * @return The event quantity.
			 */
			long getEventQuantity(int eventNumber);

			/**
			 * Returns the global joystick X axis attachment.
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalJoystickXAxisAttachment();

			/**
			 * Returns the global joystick Y axis attachment.
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalJoystickYAxisAttachment();

			/**
			 * Returns the global key or button attachment.
			 * @param keyCode The key code to return the attachment of. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalKeyOrButtonAttachment(int keyCode);

			/**
			 * Returns the global mouse wheel attachment.
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalMouseWheelAttachment();

			/**
			 * Returns the global mouse Y axis attachment.
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalMouseXAxisAttachment();

			/**
			 * Returns the global mouse X axis attachment.
			 * @return The global attachment.
			 */
			InputAttachment & getGlobalMouseYAxisAttachment();

			/**
			 * Returns the keyboard repeat delay.
			 * @return The keyboard repeat delay.
			 */
			int getKeyboardRepeatDelay();

			/**
			 * Returns the keyboard repeat rate.
			 * @return The keyboard repeat rate.
			 */
			int getKeyboardRepeatRate();

			/**
			 * Increases the event quantity of an event.
			 * @param eventNumber The number of the event to increase the quantity of.
			 * @param quantity The quantity to be added.
			 * @note This method is called by the input handlers.
			 * Do not call it from client code.
			 */
			void increaseEventQuantity(int eventNumber, long quantity);
			/**
			 * Returns whether the joystick handler is enabled (processes inputs). Defaults to true.
			 * @return Whether the joystick handler is enabled (processes inputs).
			 */
			bool isJoystickEnabled();

			/**
			 * Returns whether the keyboard handler is enabled (processes inputs). Defaults to true.
			 * @return Whether the keyboard handler is enabled (processes inputs).
			 */
			bool isKeyboardEnabled();

			/**
			 * Returns whether the mouse handler is enabled (processes inputs). Defaults to true.
			 * @return Whether the mouse handler is enabled (processes inputs).
			 */
			bool isMouseEnabled();

			/**
			 * Returns the event quantity of an event.
			 * Unlike getEventQuantity, it is not reset.
			 * @param eventNumber The number of the event.
			 * @return The event quantity.
			 */
			long peekEventQuantity(int eventNumber);

			/**
			 * Resets all event quantities. This does not detach events,
			 * but enables you to reset all key presses when switching
			 * a game phase, for example.
			 */
			void resetEventQuantities();

			/**
			 * Resets an event quantity. This does not detach the event.
			 * @param eventNumber The number of the event.
			 */
			void resetEventQuantity(int eventNumber);

			/**
			 * Resets all information about held keys, buttons, etc.
			 * Delegates to the handlers.
			 */
			void resetHelds();

			/**
			 * Sets whether the joystick handler is enabled (processes inputs). Defaults to true.
			 * @param joystickEnabled Whether the joystick handler is enabled (processes inputs).
			 */
			void setJoystickEnabled(bool joystickEnabled);

			/**
			 * Sets whether the keyboard handler is enabled (processes inputs). Defaults to true.
			 * @param keyboardEnabled Whether the keyboard handler is enabled (processes inputs).
			 */
			void setKeyboardEnabled(bool keyboardEnabled);

			/**
			 * Sets the keyboard repeat delay.
			 * @param keyboardRepeatDelay The new keyboard repeat delay.
			 */
			void setKeyboardRepeatDelay(int keyboardRepeatDelay);

			/**
			 * Sets the keyboard repeat rate.
			 * @param keyboardRepeatRate The new keyboard repeat rate.
			 */
			void setKeyboardRepeatRate(int keyboardRepeatRate);

			/**
			 * Sets whether the mouse handler is enabled (processes inputs). Defaults to true.
			 * @param mouseEnabled Whether the mouse handler is enabled (processes inputs).
			 */
			void setMouseEnabled(bool mouseEnabled);

			/**
			 * Called each frame to perform event update operations.
			 */
			void update();
	};
}


#endif
