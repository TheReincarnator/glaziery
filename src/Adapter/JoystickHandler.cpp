/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


JoystickHandler::JoystickHandler()
{
	ASSERTION_COBJECT(this);
	enabled = true;
}

JoystickHandler::~JoystickHandler()
{
	ASSERTION_COBJECT(this);
}

void JoystickHandler::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(JoystickHandler, CObject);
#endif

void JoystickHandler::initialize()
{
	ASSERTION_COBJECT(this);
	LOGPRINTF0(LOG_INFO, "Initializing joystick");
}

bool JoystickHandler::isEnabled()
{
	ASSERTION_COBJECT(this);
	return enabled;
}

void JoystickHandler::resetHelds()
{
	ASSERTION_COBJECT(this);
}

void JoystickHandler::setEnabled(bool enabled)
{
	ASSERTION_COBJECT(this);
	this->enabled = enabled;
}

void JoystickHandler::updateExclusive()
{
	ASSERTION_COBJECT(this);
}
