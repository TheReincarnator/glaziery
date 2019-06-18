/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_JOYSTICKINPUTHANDLER_H
#define __GLAZIERY_JOYSTICKINPUTHANDLER_H


namespace glaziery
{
	/**
	 * The joystick input handler handles input events produced
	 * by a joystick (or game pad) connected to this computer.
	 * The update method is called by the input manager,
	 * and the input handler increases events by calling the
	 * input manager's increaseEventQuantity method.
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class JoystickHandler : public CObject, public Configurable
	#else
		class JoystickHandler : public Configurable
	#endif
	{
		protected:

			/**
			 * Whether the handler is enabled (processes inputs). Defaults to true.
			 */
			bool enabled;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(JoystickHandler);
			#endif

		public:

			/**
			 * Creates a new joystick handler.
			 */
			JoystickHandler();

			/**
			 * Destroys the joystick handler.
			 */
			virtual ~JoystickHandler();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

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
			 * Sets whether the handler is enabled (processes inputs). Defaults to true.
			 * @param enabled Whether the handler is enabled (processes inputs).
			 */
			void setEnabled(bool enabled);

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
