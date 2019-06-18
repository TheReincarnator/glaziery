/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


MouseHandler::MouseHandler()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<3; i++)
		buttonWasPressed[i] = false;

	enabled = true;
}

MouseHandler::~MouseHandler()
{
	ASSERTION_COBJECT(this);
}

void MouseHandler::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(MouseHandler, CObject);
#endif

void MouseHandler::initialize()
{
	ASSERTION_COBJECT(this);
	LOGPRINTF0(LOG_INFO, "Initializing mouse");
}

bool MouseHandler::isEnabled()
{
	ASSERTION_COBJECT(this);
	return enabled;
}

void MouseHandler::resetHelds()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<3; i++)
		buttonWasPressed[i] = false;
}

void MouseHandler::setEnabled(bool enabled)
{
	ASSERTION_COBJECT(this);
	this->enabled = enabled;
}

void MouseHandler::updateExclusive()
{
	ASSERTION_COBJECT(this);
}
