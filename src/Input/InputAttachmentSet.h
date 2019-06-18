/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTATTACHMENTSET_H
#define __GLAZIERY_INPUTATTACHMENTSET_H


namespace glaziery
{
	/**
	 * A set of keyboard, mouse, and joystick attachment valid for a given scene.
	 * The global attachments are also formed as a special attachment set.
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class InputAttachmentSet : public CObject
	#else
		class InputAttachmentSet
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(InputAttachmentSet);
			#endif

		public:

			/**
			 * The event attachment for the x axis of the joystick.
			 */
			InputAttachment joystickXAxisAttachment;

			/**
			 * The event attachment for the y axis of the joystick.
			 */
			InputAttachment joystickYAxisAttachment;

			/**
			 * The event attachments for the keys (keyboard, mouse and joystick).
			 */
			InputAttachment keyAttachments[GL_KEYCODECOUNT];

			/**
			 * The event attachment for the mouse wheel.
			 */
			InputAttachment mouseWheelAttachment;

			/**
			 * The event attachment for x axis of the mouse.
			 */
			InputAttachment mouseXAxisAttachment;

			/**
			 * The event attachment for y axis of the mouse.
			 */
			InputAttachment mouseYAxisAttachment;

			/**
			 * Sets, that x movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick x movement.
			 * @param factor The amount to increase the x event quantity for each x step.
			 * This number may be negative.
			 */
			void attachJoystickXMove(int eventNumber, long factor);

			/**
			 * Sets, that the x position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick x position.
			 * @param factor The multiplier for the x position. This number may be negative.
			 */
			void attachJoystickXPosition(int eventNumber, long factor);

			/**
			 * Sets, that y movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick y movement.
			 * @param factor The amount to increase the y event quantity for each y step.
			 * This number may be negative.
			 */
			void attachJoystickYMove(int eventNumber, long factor);

			/**
			 * Sets, that the y position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick y position.
			 * @param factor The multiplier for the y position. This number may be negative.
			 */
			void attachJoystickYPosition(int eventNumber, long factor);

			/**
			 * Sets, that a keyboard key shall raise a certain event if pressed, released
			 * or held. To raise an event means to increase its quantity by a certain amount.
			 * @param eventNumber The number of the event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. InputManager::joyButton1 or InputManager::leftMouseButton).
			 * @param type The type of key action to raise the event. Possible values are:
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
			 * @param factor The amount to increase the event quantity in case of an event.
			 * This number may be negative.
			 */
			void attachKey(int eventNumber, int keyCode, int type, long factor);

			/**
			 * Sets, that the X movement of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 */
			void attachMouseXMove(int eventNumber, long factor);

			/**
			 * Sets, that the Y movement of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 */
			void attachMouseYMove(int eventNumber, long factor);

			/**
			 * Sets, that the mouse wheel movement shall increase a certain event quantity.
			 * @param eventNumber The number of the event for the mouse wheel movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 */
			void attachMouseWheel(int eventNumber, long factor);

			/**
			 * Removes all attachments of an event.
			 * @param eventNumber The number of the event.
			 */
			void detach(int eventNumber);

			/**
			 * Removes all event attachments.
			 */
			void detachAll();

			/**
			 * Removes any attachment of the joystick X axis to an event.
			 */
			void detachJoystickXAxis();

			/**
			 * Removes any attachment of the joystick Y axis to an event.
			 */
			void detachJoystickYAxis();

			/**
			 * Removes any attachment of a key to an event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 */
			void detachKey(int keyCode);

			/**
			 * Removes any attachments of the mouse wheel to an event.
			 */
			void detachMouseWheel();

			/**
			 * Removes any attachment of the mouse X axis to an event.
			 */
			void detachMouseXMove();

			/**
			 * Removes any attachment of the mouse Y axis to an event.
			 */
			void detachMouseYMove();

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
			int getAttachmentKey(int eventNumber, bool positive = true, bool negative = true);
	};
}


#endif
