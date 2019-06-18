/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PointerWheelEvent::PointerWheelEvent(bool option1, bool option2, int delta)
	: InputEvent(option1, option2)
{
	ASSERTION_COBJECT(this);
	this->delta = delta;
}

void PointerWheelEvent::execute()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	EventTarget * target = desktop->getEventTargetAt(desktop->getPointerPosition());
	if (target != NULL)
		target->onWheel(delta);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PointerWheelEvent, InputEvent);
#endif
