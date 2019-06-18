/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXKEYBOARDINPUTHANDLER_H
#define __GLAZIERY_DIRECTXKEYBOARDINPUTHANDLER_H


namespace glaziery
{
	class DirectXKeyboardHandler : public KeyboardHandler
	{
		friend DirectXMouseHandler;

		protected:

			static const char REPEATING_KEY_CODES[257];
			bool keyboardAcquired;
			IDirectInputDevice8 * keyboardDevice;
			unsigned char keyStates[256];
			char pressedCharacters1[256];
			char pressedCharacters2[256];
			char previousDeadKey;

			void emitEvent(int scanCode, bool held, bool pressed, bool released, int repeats);
			bool isAltPressed();
			bool isControlPressed();
			bool isShiftPressed();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXKeyboardHandler);
			#endif

		public:

			DirectXKeyboardHandler();
			virtual void initialize();
			virtual void shutDown();
			virtual void update();
			virtual void updateExclusive();
	};
}


#endif
