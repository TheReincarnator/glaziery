/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ClientAreaPopup::ClientAreaPopup()
{
	ASSERTION_COBJECT(this);

	autoDestroying = true;
	clientArea = NULL;

	Desktop::getInstance()->getSkin()->setupClientAreaPopup(this);
}

ClientAreaPopup::~ClientAreaPopup()
{
	ASSERTION_COBJECT(this);

	if (clientArea != NULL)
		if (autoDestroying)
		{
			deleteChild(clientArea);
			clientArea = NULL;
		}
		else
			unsetComponentParent(clientArea);
}

void ClientAreaPopup::adjustArea()
{
	ASSERTION_COBJECT(this);

	if (clientArea != NULL)
	{
		moveComponent(clientArea, Vector());
		resizeComponent(clientArea, getSize());
	}
}

void ClientAreaPopup::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawClientAreaPopup(context, this);
	if (clientArea != NULL)
		clientArea->draw(DrawingContext(context, clientArea));
}

ClientArea * ClientAreaPopup::getContentArea()
{
	ASSERTION_COBJECT(this);
	return clientArea;
}

EventTarget * ClientAreaPopup::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = Popup::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (clientArea != NULL)
		return clientArea->getEventTargetAt(position);

	return this;
}

Component * ClientAreaPopup::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return clientArea;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ClientAreaPopup, Popup);
#endif

bool ClientAreaPopup::isAutoDestroying()
{
	ASSERTION_COBJECT(this);
	return autoDestroying;
}

bool ClientAreaPopup::moveInternal(Vector position, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool moved = Popup::moveInternal(position, notifyParent);
	if (moved)
		adjustArea();

	return moved;
}

void ClientAreaPopup::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMoved(child, oldPosition);
	moveInternal(getPosition() - oldPosition + child->getPosition(), true);
}

void ClientAreaPopup::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMaximumSizeChanged(child, oldMaximumSize);
	setMaximumSize(getMaximumSize() + child->getMaximumSize() - oldMaximumSize);
}

void ClientAreaPopup::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMinimumSizeChanged(child, oldMinimumSize);
	setMinimumSize(getMinimumSize() + child->getMinimumSize() - oldMinimumSize);
}

void ClientAreaPopup::onChildResized(Component * clientArea, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildResized(clientArea, oldSize);
	if (clientArea != NULL)
		resizeInternal(getSize() - oldSize + clientArea->getSize(), true);
}

void ClientAreaPopup::onDestroying()
{
	ASSERTION_COBJECT(this);

	Popup::onDestroying();
	if (clientArea != NULL)
		clientArea->onDestroying();
}

void ClientAreaPopup::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onMaximumSizeChanged(oldMaximumSize);
	if (clientArea != NULL)
		setComponentMaximumSize(clientArea, getMaximumSize(), false);
}

void ClientAreaPopup::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onMinimumSizeChanged(oldMinimumSize);
	if (clientArea != NULL)
		setComponentMinimumSize(clientArea, getMinimumSize(), false);
}

bool ClientAreaPopup::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool resized = Popup::resizeInternal(size, notifyParent);
	if (resized)
		adjustArea();

	return resized;
}

void ClientAreaPopup::setAutoDestroying(bool autoDestroying)
{
	ASSERTION_COBJECT(this);
	this->autoDestroying = autoDestroying;
}

void ClientAreaPopup::setContentArea(ClientArea * clientArea)
{
	ASSERTION_COBJECT(this);

	if (this->clientArea != NULL)
		deleteChild(this->clientArea);

	this->clientArea = clientArea;

	if (clientArea != NULL)
	{
		setComponentParent(clientArea);
		adjustArea();
	}

	invalidate();
}
