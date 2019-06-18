/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SpecialEvent::SpecialEvent(bool option1, bool option2, int scanCode, bool held, bool pressed,
	bool released, int repeats, Type type)
	: KeyEvent(option1, option2, scanCode, held, pressed, released, repeats)
{
	ASSERTION_COBJECT(this);

	this->option1 = option1;
	this->option2 = option2;
	this->type = type;
}

void SpecialEvent::execute()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (type == EVENT_CANCEL && desktop->isDragging())
		desktop->cancelDragging();
	else
		KeyEvent::execute();
}

void SpecialEvent::executeOn(EventTarget * target)
{
	ASSERTION_COBJECT(this);

	KeyEvent::executeOn(target);

	switch (type)
	{
		case EVENT_BACK_SPACE: target->onBackSpace(); break;
		case EVENT_BACK_TAB: target->onBackTab(false); break;
		case EVENT_CANCEL: target->onCancel(); break;
		case EVENT_COPY: target->onCopy(); break;
		case EVENT_CUT: target->onCut(); break;
		case EVENT_DELETE: target->onDelete(); break;
		case EVENT_EDIT: target->onEdit(); break;
		case EVENT_ENTER: target->onEnter(option1, option2); break;
		case EVENT_MOVE_DOWN: target->onMoveDown(option1, option2); break;
		case EVENT_MOVE_LEFT: target->onMoveLeft(option1, option2); break;
		case EVENT_MOVE_RIGHT: target->onMoveRight(option1, option2); break;
		case EVENT_MOVE_TO_END: target->onMoveToEnd(option1, option2); break;
		case EVENT_MOVE_TO_START: target->onMoveToStart(option1, option2); break;
		case EVENT_MOVE_UP: target->onMoveUp(option1, option2); break;
		case EVENT_PAGE_DOWN: target->onPageDown(option1, option2); break;
		case EVENT_PAGE_UP: target->onPageUp(option1, option2); break;
		case EVENT_PASTE: target->onPaste(); break;
		case EVENT_SECONDARY_BACK_TAB: target->onBackTab(true); break;
		case EVENT_SECONDARY_TAB: target->onTab(true); break;
		case EVENT_SELECT_ALL: target->onSelectAll(); break;
		case EVENT_TAB: target->onTab(false); break;
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SpecialEvent, KeyEvent);
#endif
