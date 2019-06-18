/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXJOYSTICKINPUTHANDLER_H
#define __GLAZIERY_DIRECTXJOYSTICKINPUTHANDLER_H


namespace glaziery
{
	class DirectXJoystickHandler : public JoystickHandler
	{
		protected:

			bool joystickAcquired;
			IDirectInputDevice8 * joystickDevice;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXJoystickHandler);
			#endif

		public:

			DirectXJoystickHandler();
			virtual void initialize();
			virtual void shutDown();
			virtual void update();
	};
}


#endif
