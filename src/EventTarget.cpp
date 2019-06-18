/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


EventTarget::~EventTarget()
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->releaseReferences(this);
}

void EventTarget::executeDeferrals()
{
	ASSERTION_COBJECT(this);
}

const String & EventTarget::getTooltip()
{
	ASSERTION_COBJECT(this);
	return tooltip;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(EventTarget, Listenable);
#endif

bool EventTarget::onAnyKey(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onAnyKey(this, option1, option2);
	}

	return false;
}

bool EventTarget::onBackSpace()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onBackSpace(this);
	}

	return false;
}

bool EventTarget::onBackTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onBackTab(this, secondary);
	}

	return false;
}

bool EventTarget::onCancel()
{
	ASSERTION_COBJECT(this);
	return false;
}

bool EventTarget::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onCharacter(this, character, option1, option2);
	}

	return false;
}

void EventTarget::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onClick(this, position, option1, option2);
	}
}

void EventTarget::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onContextClick(this, position, option1, option2);
	}
}

bool EventTarget::onCopy()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onCopy(this);
	}

	return false;
}

bool EventTarget::onCut()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onCut(this);
	}

	return false;
}

bool EventTarget::onDelete()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onDelete(this);
	}

	return false;
}

void EventTarget::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onDoubleClick(this, position, option1, option2);
	}
}

void EventTarget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);
}

DraggingSurrogate * EventTarget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);
	return NULL;
}

bool EventTarget::onEdit()
{
	ASSERTION_COBJECT(this);
	return false;
}

bool EventTarget::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onEnter(this, option1, option2);
	}

	return false;
}

long EventTarget::onHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);
	return 0;
}

bool EventTarget::onHotKey(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onHotKey(this, character, option1, option2);
	}

	return false;
}

void EventTarget::onHover()
{
	ASSERTION_COBJECT(this);

	if (!tooltip.IsEmpty())
	{
		BalloonPopup * popup;
		if ((popup = new BalloonPopup) == NULL)
			throw EOUTOFMEMORY;
		Desktop::getInstance()->addPopup(popup);

		popup->setMessage(tooltip);

		Vector position = Desktop::getInstance()->getPointerPosition();
		popup->pointTo(position - Vector(0, 8), position + Vector(0, 8));
	}

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onHover(this);
	}
}

bool EventTarget::onKeyStroke(int keyCode, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onKeyStroke(this, keyCode, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveDown(this, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveLeft(this, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveRight(this, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveToEnd(this, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveToStart(this, option1, option2);
	}

	return false;
}

bool EventTarget::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onMoveUp(this, option1, option2);
	}

	return false;
}

bool EventTarget::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onPageDown(this, option1, option2);
	}

	return false;
}

bool EventTarget::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onPageUp(this, option1, option2);
	}

	return false;
}

bool EventTarget::onPaste()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onPaste(this);
	}

	return false;
}

void EventTarget::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onPress(this, position, option1, option2);
	}
}

void EventTarget::onRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onRelease(this, option1, option2);
	}
}

bool EventTarget::onSelectAll()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onSelectAll(this);
	}

	return false;
}

bool EventTarget::onTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onTab(this, secondary);
	}

	return false;
}

void EventTarget::onWheel(int delta)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		EventTarget::Listener * eventTargetListener
			= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
		if (eventTargetListener != NULL)
			eventTargetListener->onWheel(this, delta);
	}
}

void EventTarget::setTooltip(const String & tooltip)
{
	ASSERTION_COBJECT(this);
	this->tooltip = tooltip;
}


EventTarget::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onAnyKey(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onBackSpace(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onBackTab(EventTarget * target, bool secondary)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onCharacter(EventTarget * target, char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onClick(EventTarget * target, Vector position,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onContextClick(EventTarget * target, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onCopy(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onCut(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onDelete(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onDoubleClick(EventTarget * target, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onDropped(EventTarget * target,
	EventTarget * draggedEventTarget, Vector relativePosition, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onEnter(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onHotKey(EventTarget * target, char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onHover(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onKeyStroke(EventTarget * target, int keyCode, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveDown(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveLeft(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveRight(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveToEnd(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveToStart(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onMoveUp(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onPageDown(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onPageUp(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onPaste(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onPress(EventTarget * target, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onRelease(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onSelectAll(EventTarget * target)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onTab(EventTarget * target, bool secondary)
{
	ASSERTION_COBJECT(this);
}

void EventTarget::Listener::onWheel(EventTarget * target, int delta)
{
	ASSERTION_COBJECT(this);
}
