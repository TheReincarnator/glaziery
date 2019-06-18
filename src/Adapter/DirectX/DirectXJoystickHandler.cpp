/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


DirectXJoystickHandler::DirectXJoystickHandler()
{
	ASSERTION_COBJECT(this);

	joystickAcquired = false;
	joystickDevice = NULL;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXJoystickHandler, JoystickHandler);
#endif

void DirectXJoystickHandler::initialize()
{
	ASSERTION_COBJECT(this);

	JoystickHandler::initialize();

	// Initializing DirectX

	// TODO: Initialize (among others SetCooperativeLevel: DISCL_FOREGROUND | DISCL_EXCLUSIVE)
	joystickDevice = NULL;

	LOGPRINTF0(LOG_INFO, "Initialized joystick input handler");
}

void DirectXJoystickHandler::shutDown()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_INFO, "Shutting down joystick input handler");

	if (joystickDevice != NULL)
	{
		// Unacquire is ok if device is not acquired.
		joystickDevice->Unacquire();
		joystickDevice->Release();
		joystickDevice = NULL;
	}

	LOGPRINTF0(LOG_INFO, "Joystick input handler shut down");
}

void DirectXJoystickHandler::update()
{
	ASSERTION_COBJECT(this);
	// TODO
}
