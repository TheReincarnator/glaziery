/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TabbingArea::TabbingArea()
{
	ASSERTION_COBJECT(this);

	focusPane = NULL;

	Desktop::getInstance()->getSkin()->setupTabbingArea(this);
}

TabbingArea::~TabbingArea()
{
	ASSERTION_COBJECT(this);
	panes.DeleteAll();
}

void TabbingArea::adjustClientAreas()
{
	ASSERTION_COBJECT(this);

	int paneCount = panes.GetCount();
	if (paneCount <= 0)
		return;

	Vector topLeftSize;
	Vector bottomRightSize;
	Skin * skin = Desktop::getInstance()->getSkin();
	skin->getTabbingAreaFrameSizes(this, topLeftSize, bottomRightSize);
	Vector size = getSize() - topLeftSize - bottomRightSize;

	for (int i=0; i<paneCount; i++)
	{
		ClientArea * pane = panes.Get(i);
		moveComponent(pane, topLeftSize);
		resizeComponent(pane, size);
	}
}

void TabbingArea::appendPane(ClientArea * pane, const String & name)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	panes.Append(pane);
	setComponentParent(pane);

	String * nameCopy;
	if ((nameCopy = new String(name)) == NULL)
		throw EOUTOFMEMORY;
	paneNames.Append(nameCopy);

	addHotKey(findHotKey(name), pane);

	adjustClientAreas();
	invalidate();
}

void TabbingArea::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawTabbingArea(context, this);

	if (focusPane != NULL && focusPane->isVisible())
		focusPane->draw(DrawingContext(context, focusPane));
}

EventTarget * TabbingArea::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ClientArea::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	Vector topLeftSize;
	Vector bottomRightSize;
	Skin * skin = Desktop::getInstance()->getSkin();
	skin->getTabbingAreaFrameSizes(this, topLeftSize, bottomRightSize);

	if (focusPane != NULL && focusPane->isVisible()
		&& position >= topLeftSize && position < getSize() - bottomRightSize)
		return focusPane->getEventTargetAt(position - topLeftSize);

	return this;
}

Component * TabbingArea::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return focusPane;
}

ClientArea * TabbingArea::getFocusPane()
{
	ASSERTION_COBJECT(this);
	return focusPane;
}

const String & TabbingArea::getPaneName(int position)
{
	ASSERTION_COBJECT(this);

	ClientArea * pane = panes.Get(position);
	if (pane == NULL)
		throw EILLEGALARGUMENT("Invalid tabbing pane position provided when getting its name");

	return *paneNames.Get(position);
}

const ArrayList<ClientArea> & TabbingArea::getPanes()
{
	ASSERTION_COBJECT(this);
	return panes;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TabbingArea, ClientArea);
#endif

void TabbingArea::insertPane(int position, ClientArea * pane, const String & name)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	panes.Insert(pane, position);
	setComponentParent(pane);

	String * nameCopy;
	if ((nameCopy = new String(name)) == NULL)
		throw EOUTOFMEMORY;
	paneNames.Insert(nameCopy, position);

	addHotKey(findHotKey(name), pane);

	adjustClientAreas();
	invalidate();
}

bool TabbingArea::isChildVisible(Component * child)
{
	ASSERTION_COBJECT(this);
	return focusPane == child && child->isVisible();
}

bool TabbingArea::onBackTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = ClientArea::onBackTab(secondary);
	if (secondary)
	{
		setFocusToPreviousPane();
		return true;
	}
	else
		return consumed;
}

void TabbingArea::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildMoved(child, oldPosition);
	moveInternal(getPosition() + child->getPosition() - oldPosition, true);
	moveComponent(child, oldPosition);
}

void TabbingArea::onChildResized(Component * child, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildResized(child, oldSize);
	Vector delta = child->getSize() - oldSize;
	resizeInternal(getSize() + delta, true);
}

void TabbingArea::onDestroying()
{
	ASSERTION_COBJECT(this);

	ClientArea::onDestroying();
	for (int i=0; i<panes.GetCount(); i++)
		panes.Get(i)->onDestroying();
}

bool TabbingArea::onHotKey(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ClientArea::onHotKey(character, option1, option2);
	ClientArea * pane = dynamic_cast<ClientArea *>(getHotKeyComponent(character));
	if (pane != NULL && pane->isVisible())
	{
		setFocusPane(pane);
		return true;
	}
	else
		return consumed;
}

bool TabbingArea::onTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = ClientArea::onTab(secondary);
	if (secondary)
	{
		setFocusToNextPane();
		return true;
	}
	else
		return consumed;
}

void TabbingArea::prependPane(ClientArea * pane, const String & name)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	panes.Prepend(pane);
	setComponentParent(pane);

	String * nameCopy;
	if ((nameCopy = new String(name)) == NULL)
		throw EOUTOFMEMORY;
	paneNames.Prepend(nameCopy);

	addHotKey(findHotKey(name), pane);

	adjustClientAreas();
	invalidate();
}

void TabbingArea::releaseCache()
{
	ASSERTION_COBJECT(this);

	ClientArea::releaseCache();

	for (int i=0; i<panes.GetCount(); i++)
		panes.Get(i)->releaseCache();
}

void TabbingArea::removePane(int position)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	ClientArea * pane = panes.Get(position);
	if (pane == NULL)
		return;

	removeHotKey(findHotKey(*paneNames.Get(position)));

	panes.Unlink(position);
	paneNames.Delete(position);
	pane->destroy();

	adjustClientAreas();
	invalidate();
}

bool TabbingArea::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool resized = ClientArea::resizeInternal(size, notifyParent);
	if (resized)
		adjustClientAreas();

	return resized;
}

void TabbingArea::setFocusPane(ClientArea * focusPane)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	if (this->focusPane == focusPane)
		return;

	if (this->focusPane != NULL)
		this->focusPane->onLostFocus();

	this->focusPane = focusPane;

	if (this->focusPane != NULL)
		this->focusPane->onGotFocus(false);
}

bool TabbingArea::setFocusToNextPane()
{
	ASSERTION_COBJECT(this);
	return ClientArea::setFocusToNextPane();
}

bool TabbingArea::setFocusToPreviousPane()
{
	ASSERTION_COBJECT(this);
	return ClientArea::setFocusToPreviousPane();
}

void TabbingArea::setPaneName(int position, const String & name)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The tabbing area must be added to a parent"
			" before using it");

	ClientArea * pane = panes.Get(position);
	if (pane == NULL)
		return;

	removeHotKey(findHotKey(*paneNames.Get(position)));
	*paneNames.Get(position) = name;
	addHotKey(findHotKey(name), pane);

	adjustClientAreas();
	invalidate();
}
