/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MOUSEINPUTHANDLER_H
#define __GLAZIERY_MOUSEINPUTHANDLER_H


namespace glaziery
{
	/**
	 * The mouse input handler handles input events produced
	 * by a mouse connected to this computer.
	 * The update method is called by the input manager,
	 * and the input handler increases events by calling the
	 * input manager's increaseEventQuantity method.
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class MouseHandler : public CObject, public Configurable
	#else
		class MouseHandler : public Configurable
	#endif
	{
		protected:

			/**
			 * Whether the mouse buttons were pressed last frame.
			 * The index is the button number.
			 * @see InputManager::KEY_MOUSE_LEFT
			 * @see InputManager::KEY_MOUSE_MIDDLE
			 * @see InputManager::KEY_MOUSE_RIGHT
			 */
			bool buttonWasPressed[3];

			/**
			 * Whether the handler is enabled (processes inputs). Defaults to true.
			 */
			bool enabled;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(MouseHandler);
			#endif

		public:

			/**
			 * Creates a new mouse handler.
			 */
			MouseHandler();

			/**
			 * Destroys the mouse handler.
			 */
			virtual ~MouseHandler();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Returns the maximum time between clicks to accept them as double clicks.
			 * @return The maximum time between double clicks.
			 */
			inline int getDoubleClickDelay();

			/**
			 * The maximum X and Y distance between clicks to accept them as double clicks.
			 * @return The maximum X and Y distance between double clicks.
			 */
			inline int getDoubleClickDistance();

			/**
			 * Returns the pointer position. The mouse handler is responsible for delegating
			 * to the underlying OS pointer (and maybe caching).
			 * @return The pointer position.
			 */
			virtual Vector getPointerPosition() = NULL;

			/**
			 * Initializes the input handler.
			 * @note Override but supercall this method in the platform implementation.
			 */
			virtual void initialize();

			/**
			 * Returns whether the handler is enabled (processes inputs). Defaults to true.
			 * @return Whether the handler is enabled (processes inputs).
			 */
			bool isEnabled();

			/**
			 * Resets all information about held keys, buttons, etc.
			 */
			virtual void resetHelds();

			/**
			 * Returns the maximum time between clicks to accept them as double clicks.
			 * @param doubleClickDelay The maximum time between double clicks.
			 */
			inline void setDoubleClickDelay(int doubleClickDelay);

			/**
			 * The maximum X and Y distance between clicks to accept them as double clicks.
			 * @param doubleClickDistance The maximum X and Y distance between double clicks.
			 */
			inline void setDoubleClickDistance(int doubleClickDistance);

			/**
			 * Sets whether the handler is enabled (processes inputs). Defaults to true.
			 * @param enabled Whether the handler is enabled (processes inputs).
			 */
			void setEnabled(bool enabled);

			/**
			 * Sets the pointer position. The mouse handler is responsible for delegating
			 * to the underlying OS pointer (and maybe caching).
			 * @param position The new pointer position.
			 */
			virtual void setPointerPosition(Vector position) = NULL;

			/**
			 * Shuts down the input handler.
			 * @note Implement this method in the platform implementation.
			 */
			virtual void shutDown() = NULL;

			/**
			 * Handles all input and updates event quantities.
			 * @note Implement this method in the platform implementation.
			 */
			virtual void update() = NULL;

			/**
			 * Updates the handler when the scene's exclusive changes.
			 */
			virtual void updateExclusive();
	};
}


#endif
