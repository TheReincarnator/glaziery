/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_KEYBOARDINPUTHANDLER_H
#define __GLAZIERY_KEYBOARDINPUTHANDLER_H


namespace glaziery
{
	/**
	 * The keyboard input handler handles input events produced
	 * by the keyboard connected to this computer.
	 * The update method is called by the input manager,
	 * and the input handler increases events by calling the
	 * input manager's increaseEventQuantity method.
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class KeyboardHandler : public CObject, public Configurable
	#else
		class KeyboardHandler : public Configurable
	#endif
	{
		// Configurable properties
		CF_INT_DECL(KeyboardRepeatDelay)
		CF_INT_DECL(KeyboardRepeatRate)

		protected:

			/**
			 * The size of the keyboard buffer.
			 */
			static const int BUFFER_SIZE;

			/**
			 * Whether the handler is enabled (processes inputs). Defaults to true.
			 */
			bool enabled;

			/**
			 * The time when the key was pressed, in terms of the timing control.
			 */
			unsigned long keyPressTimes[256];

			/**
			 * An array containing zeros and ones. One indicates that the respective key
			 * has been held down the last frame.
			 */
			char lastFrameKeyHelds[256];

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(KeyboardHandler);
			#endif

		public:

			/**
			 * Creates a new keyboard handler.
			 */
			KeyboardHandler();

			/**
			 * Destroys the keyboard handler.
			 */
			virtual ~KeyboardHandler();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Notifies the input manager about keyboard events not consumed by any event targets.
			 * These events cause input manager's event quantities to increase.
			 * Do not call this method directly, it is called by the respective input event.
			 * @param scanCode The scan code of the key that caused the event.
			 * @param held Whether the key is currently held down.
			 * @param pressed Whether the key is newly pressed down.
			 * @param released Whether the key has been released.
			 * @param repeats The number of key repeats.
			 * @return Whether any attached event matches the given parameters.
			 */
			bool increaseManagerEventQuantities(int scanCode, bool held,
				bool pressed, bool released, int repeats);

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
