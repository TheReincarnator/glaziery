/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


CharacterEvent::CharacterEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
	bool released, int repeats, char character1, char character2)
	: KeyEvent(option1, option2, scanCode, held, pressed, released, repeats)
{
	ASSERTION_COBJECT(this);

	this->character1 = character1;
	this->character2 = character2;
}

void CharacterEvent::executeOn(EventTarget * target)
{
	ASSERTION_COBJECT(this);

	KeyEvent::executeOn(target);

	if (character1 != 0)
		target->onCharacter(character1, option1, option2);

	if (character2 != 0)
		target->onCharacter(character2, option1, option2);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(CharacterEvent, KeyEvent);
#endif
