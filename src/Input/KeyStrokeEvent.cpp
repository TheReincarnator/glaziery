/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


KeyStrokeEvent::KeyStrokeEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
	bool released, int repeats)
	: KeyEvent(option1, option2, scanCode, held, pressed, released, repeats)
{
	ASSERTION_COBJECT(this);
}

void KeyStrokeEvent::executeOn(EventTarget * target)
{
	ASSERTION_COBJECT(this);

	KeyEvent::executeOn(target);

	target->onKeyStroke(scanCode, option1, option2);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(KeyStrokeEvent, KeyEvent);
#endif
