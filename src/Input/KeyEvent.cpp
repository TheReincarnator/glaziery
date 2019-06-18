/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


KeyEvent::KeyEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
	bool released, int repeats)
	: InputEvent(option1, option2)
{
	ASSERTION_COBJECT(this);

	this->held = held;
	this->pressed = pressed;
	this->released = released;
	this->repeats = repeats;
	this->scanCode = scanCode;
}

void KeyEvent::execute()
{
	ASSERTION_COBJECT(this);

	int pressesAndRepeats = (pressed ? 1 : 0) + repeats;
	Desktop * desktop = Desktop::getInstance();
	EventTarget * target = desktop->getFocusWindowOrPopup();

	bool eventConsumingTarget;
	if (dynamic_cast<Popup *>(target) != NULL)
		eventConsumingTarget = true;
	else if (dynamic_cast<Window *>(target) != NULL)
		eventConsumingTarget = ((Window *) target)->isEventConsuming();
	else
		eventConsumingTarget = false;

	if (target == NULL)
		target = desktop;

	if (!eventConsumingTarget)
		if (desktop->getInputManager().getKeyboardHandler()->increaseManagerEventQuantities(scanCode,
			held, pressed, released, repeats))
			return;

	for (int i=0; i<pressesAndRepeats; i++)
		executeOn(target);
}

void KeyEvent::executeOn(EventTarget * target)
{
	ASSERTION_COBJECT(this);
	target->onAnyKey(option1, option2);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(KeyEvent, InputEvent);
#endif
