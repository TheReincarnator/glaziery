/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ClientArea::ClientArea()
{
	ASSERTION_COBJECT(this);

	backgroundColor = GL_RGB(0, 0, 128);

	Desktop::getInstance()->getSkin()->setupClientArea(this);
}

ClientArea::~ClientArea()
{
	ASSERTION_COBJECT(this);

	// The hot key lookup should not remove the fields.
	hotKeyComponents.UnsetAll();
}

void ClientArea::addHotKey(char hotKey, Component * component)
{
	ASSERTION_COBJECT(this);

	if (hotKey == 0)
		return;

	Component * existingComponent = hotKeyComponents.Get(hotKey);
	if (existingComponent != NULL)
	{
		String message;
		message.Format("Duplicate client area hot key '%c': %s and %s",
			hotKey, (const char *) existingComponent->toString(), (const char *) component->toString());

		#ifdef _DEBUG
			throw EILLEGALARGUMENT(message);
		#else
			LOGPRINTF0(LOG_ERROR, message);
			return;
		#endif
	}

	hotKeyComponents.Set(hotKey, component);
}

char ClientArea::findHotKey(const String & text)
{
	int index = text.Find("&");
	while (index >= 0 && text[index + 1] != 0)
	{
		if (text[index + 1] != '&')
			return String::ToLowerCase(text[index + 1]);
		else
			index = text.Find("&", index + 2);
	}

	return 0;
}

unsigned long ClientArea::getBackgroundColor()
{
	ASSERTION_COBJECT(this);
	return backgroundColor;
}

ClientArea * ClientArea::getFocusPane()
{
	ASSERTION_COBJECT(this);
	return NULL;
}

Component * ClientArea::getHotKeyComponent(char hotKey)
{
	ASSERTION_COBJECT(this);

	if (hotKey == 0)
		return NULL;

	return hotKeyComponents.Get(hotKey);
}

const ArrayList<ClientArea> & ClientArea::getPanes()
{
	ASSERTION_COBJECT(this);
	return noPanes;
}

Vector ClientArea::getPositionOnWindow()
{
	ASSERTION_COBJECT(this);

	Vector position = getPosition();

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<Window *>(ancestor) == NULL)
	{
		position += ancestor->getPosition();
		ancestor = ancestor->getParent();
	}

	return position;
}

Window * ClientArea::getWindow()
{
	ASSERTION_COBJECT(this);

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<Window *>(ancestor) == NULL)
		ancestor = ancestor->getParent();

	return (Window *) ancestor;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ClientArea, CachableComponent);
#endif

void ClientArea::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Window * window = getWindow();
	if (window != NULL)
		window->bringToFront();

	CachableComponent::onPress(position, option1, option2);
}

void ClientArea::removeHotKey(char hotKey)
{
	ASSERTION_COBJECT(this);

	if (hotKey == 0)
		return;

	hotKeyComponents.Unset(hotKey);
}

void ClientArea::setBackgroundColor(unsigned long backgroundColor)
{
	ASSERTION_COBJECT(this);
	this->backgroundColor = backgroundColor;
}

void ClientArea::setFocusPane(ClientArea * focusPane)
{
	ASSERTION_COBJECT(this);
}

void ClientArea::setFocusToFirstPaneDeeply()
{
	ASSERTION_COBJECT(this);

	const ArrayList<ClientArea> & panes = getPanes();
	for (int i = 0; i < panes.GetCount(); i++)
	{
		ClientArea * pane = panes.Get(i);
		if (panes.Get(i)->isVisible())
		{
			setFocusPane(pane);
			pane->setFocusToLastPaneDeeply();
			return;
		}
	}
}

bool ClientArea::setFocusToNextPane()
{
	ASSERTION_COBJECT(this);

	const ArrayList<ClientArea> & panes = getPanes();

	int paneCount = panes.GetCount();
	if (paneCount == 0)
		return true;

	ClientArea * focusPane = getFocusPane();
	if (focusPane != NULL)
		if (!focusPane->setFocusToNextPane())
			return false;

	int paneNo = paneCount - 1;
	if (focusPane != NULL)
	{
		paneNo = panes.Find(focusPane);
		if (paneNo < 0)
			paneNo = paneCount - 1;
	}

	for (int step = 1; step < paneCount; step++)
	{
		int newPaneNo = (paneNo + step) % paneCount;
		ClientArea * pane = panes.Get(newPaneNo);
		if (pane->isVisible())
		{
			bool rolled = newPaneNo <= paneNo;
			setFocusPane(pane);
			pane->setFocusToFirstPaneDeeply();
			return rolled;
		}
	}

	return true;
}

void ClientArea::setFocusToLastPaneDeeply()
{
	ASSERTION_COBJECT(this);

	const ArrayList<ClientArea> & panes = getPanes();
	for (int i = panes.GetCount() - 1; i >= 0; i--)
	{
		ClientArea * pane = panes.Get(i);
		if (panes.Get(i)->isVisible())
		{
			setFocusPane(pane);
			pane->setFocusToLastPaneDeeply();
			return;
		}
	}
}

bool ClientArea::setFocusToPreviousPane()
{
	ASSERTION_COBJECT(this);

	const ArrayList<ClientArea> & panes = getPanes();

	int paneCount = panes.GetCount();
	if (paneCount == 0)
		return true;

	ClientArea * focusPane = getFocusPane();
	if (focusPane != NULL)
		if (!focusPane->setFocusToPreviousPane())
			return false;

	int paneNo = 0;
	if (focusPane != NULL)
	{
		paneNo = panes.Find(focusPane);
		if (paneNo < 0)
			paneNo = 0;
	}

	for (int step = paneCount - 1; step > 0; step--)
	{
		int newPaneNo = (paneNo + step) % paneCount;
		ClientArea * pane = panes.Get(newPaneNo);
		if (pane->isVisible())
		{
			bool rolled = newPaneNo >= paneNo;
			setFocusPane(pane);
			pane->setFocusToLastPaneDeeply();
			return rolled;
		}
	}

	return true;
}

void ClientArea::show()
{
	ASSERTION_COBJECT(this);

	CachableComponent::show();
	invalidate();
}

String ClientArea::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("ClientArea(position:%s,size:%s)",
		(const char *) getPosition().toString(), (const char *) getSize().toString());
	return string;
}
