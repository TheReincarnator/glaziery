/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PointerMoveEvent::PointerMoveEvent(bool option1, bool option2, Vector oldPosition, Vector newPosition)
	: InputEvent(option1, option2)
{
	ASSERTION_COBJECT(this);

	this->oldPosition = oldPosition;
	this->newPosition = newPosition;
}

void PointerMoveEvent::execute()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();

	if (desktop->pointerType != NULL)
		desktop->pointerType->onPointerMoved(newPosition, oldPosition);

	if (desktop->primaryPressTarget != NULL)
	{
		if (desktop->draggingSurrogate != NULL)
		{
			desktop->draggingSurrogate->onDragMove(desktop->primaryPressTarget,
				desktop->primaryPressPosition, oldPosition, newPosition);
			desktop->primaryPressTarget->onDragMove(desktop->primaryPressPosition,
				oldPosition, newPosition);
		}
		else if (!desktop->draggingTested)
		{
			Vector dragDelta = (desktop->primaryPressPosition - newPosition).absolute();
			if (dragDelta.x >= desktop->dragStartDistance || dragDelta.y >= desktop->dragStartDistance)
			{
				desktop->draggingTested = true;
				if (desktop->draggingSurrogate != NULL)
					delete desktop->draggingSurrogate;

				desktop->draggingSurrogate = desktop->primaryPressTarget->onDragStart(desktop->primaryPressPosition);

				if (desktop->draggingSurrogate != NULL)
				{
					Vector relativePosition = desktop->primaryPressPosition - desktop->primaryPressTarget->getOrigin();
					desktop->draggingSurrogate->setOffset(-relativePosition);

					desktop->draggingSurrogate->onDragMove(desktop->primaryPressTarget,
						desktop->primaryPressPosition, desktop->primaryPressPosition, newPosition);
					desktop->primaryPressTarget->onDragMove(desktop->primaryPressPosition,
						desktop->primaryPressPosition, newPosition);
				}
			}
		}
	}

	desktop->updatePointerType();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PointerMoveEvent, InputEvent);
#endif
