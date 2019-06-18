/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SplittingArea::SplittingArea(SplittingArea::Direction direction)
{
	ASSERTION_COBJECT(this);

	this->direction = direction;
	focusPane = NULL;
	panesResizable = true;

	Desktop::getInstance()->getSkin()->setupSplittingArea(this);
}

SplittingArea::~SplittingArea()
{
	ASSERTION_COBJECT(this);
	panes.DeleteAll();
}

void SplittingArea::adjustClientAreas()
{
	ASSERTION_COBJECT(this);

	int paneCount = panes.GetCount();
	if (paneCount <= 0)
		return;

	Skin * skin = Desktop::getInstance()->getSkin();
	int splitterSize = skin->getSplitterSize(this);

	int oldPaneSizeSum = 0;
	if (direction == HORIZONTAL)
		for (int i=0; i<paneCount; i++)
		{
			ClientArea * pane = panes.Get(i);
			oldPaneSizeSum += pane->getSize().x;
		}
	else
		for (int i=0; i<paneCount; i++)
		{
			ClientArea * pane = panes.Get(i);
			oldPaneSizeSum += pane->getSize().y;
		}

	int newPaneSizeSum = (direction == HORIZONTAL ? getSize().x : getSize().y)
		- (paneCount - 1) * splitterSize;

	int moveSum = 0;
	if (direction == HORIZONTAL)
		for (int i=0; i<paneCount; i++)
		{
			ClientArea * pane = panes.Get(i);
			int oldDistribution = (oldPaneSizeSum + i) / paneCount;
			int newDistribution = (newPaneSizeSum + i) / paneCount;
			int newSize = pane->getSize().x + newDistribution - oldDistribution;
			moveComponent(pane, Vector(moveSum, 0));
			resizeComponent(pane, Vector(newSize, getSize().y));
			moveSum += newSize + splitterSize;
		}
	else
		for (int i=0; i<paneCount; i++)
		{
			ClientArea * pane = panes.Get(i);
			int oldDistribution = (oldPaneSizeSum + i) / paneCount;
			int newDistribution = (newPaneSizeSum + i) / paneCount;
			int newSize = pane->getSize().y + newDistribution - oldDistribution;
			moveComponent(pane, Vector(0, moveSum));
			resizeComponent(pane, Vector(getSize().x, newSize));
			moveSum += newSize + splitterSize;
		}
}

void SplittingArea::appendPane(ClientArea * pane)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The splitting area must be added to a parent"
			" before using it");

	panes.Append(pane);
	setComponentParent(pane);

	if (panes.GetCount() == 1)
		resizeInternal(pane->getSize(), true);
	else
		resizeForNewPane(pane);

	adjustClientAreas();
	invalidate();
}

void SplittingArea::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawSplittingArea(context, this);
	for (int i=0; i<panes.GetCount(); i++)
	{
		ClientArea * pane = panes.Get(i);
		if (pane->isVisible())
			pane->draw(DrawingContext(context, pane));
	}
}

SplittingArea::Direction SplittingArea::getDirection()
{
	ASSERTION_COBJECT(this);
	return direction;
}

EventTarget * SplittingArea::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ClientArea::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	Skin * skin = Desktop::getInstance()->getSkin();
	int splitterSize = skin->getSplitterSize(this);

	int offset = 0;
	if (direction == HORIZONTAL)
		for (int i=0; i<panes.GetCount(); i++)
		{
			ClientArea * pane = panes.Get(i);
			if (pane->isVisible() && position.x >= offset
				&& position.x < offset + pane->getSize().x)
				return pane->getEventTargetAt(Vector(position.x - offset, position.y));
			offset += pane->getSize().x + splitterSize;
		}
	else
		for (int i=0; i<panes.GetCount(); i++)
		{
			ClientArea * pane = panes.Get(i);
			if (pane->isVisible() && position.y >= offset
				&& position.y < offset + pane->getSize().y)
				return pane->getEventTargetAt(Vector(position.x, position.y - offset));
			offset += pane->getSize().y + splitterSize;
		}

	return this;
}

Component * SplittingArea::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return focusPane;
}

ClientArea * SplittingArea::getFocusPane()
{
	ASSERTION_COBJECT(this);
	return focusPane;
}

const ArrayList<ClientArea> & SplittingArea::getPanes()
{
	ASSERTION_COBJECT(this);
	return panes;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SplittingArea, ClientArea);
#endif

void SplittingArea::insertPane(int position, ClientArea * pane)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The splitting area must be added to a parent"
			" before using it");

	panes.Insert(pane, position);
	setComponentParent(pane);

	if (panes.GetCount() == 1)
		resizeInternal(pane->getSize(), true);
	else
		resizeForNewPane(pane);

	adjustClientAreas();
	invalidate();
}

bool SplittingArea::isPanesResizable()
{
	ASSERTION_COBJECT(this);
	return panesResizable;
}

void SplittingArea::moveSplitter(int splitter, int delta)
{
	ASSERTION_COBJECT(this);

	if (splitter < 0 || splitter >= panes.GetCount() - 1)
		return;

	ClientArea * pane1 = panes.Get(splitter);
	ClientArea * pane2 = panes.Get(splitter + 1);

	if (direction == HORIZONTAL)
	{
		resizeComponent(pane1, Vector(pane1->getSize().x + delta, pane1->getSize().y));
		resizeComponent(pane2, Vector(pane2->getSize().x - delta, pane2->getSize().y));
		moveComponent(pane2, Vector(pane2->getPosition().x + delta, pane2->getPosition().y));
	}
	else
	{
		resizeComponent(pane1, Vector(pane1->getSize().x, pane1->getSize().y + delta));
		resizeComponent(pane2, Vector(pane2->getSize().x, pane2->getSize().y - delta));
		moveComponent(pane2, Vector(pane2->getPosition().x, pane2->getPosition().y + delta));
	}

	invalidate();
}

bool SplittingArea::onBackTab(bool secondary)
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

void SplittingArea::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildMoved(child, oldPosition);
	moveInternal(getPosition() + child->getPosition() - oldPosition, true);
	moveComponent(child, oldPosition);
}

void SplittingArea::onChildResized(Component * child, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildResized(child, oldSize);
	Vector delta = child->getSize() - oldSize;
	resizeInternal(getSize() + delta, true);
}

void SplittingArea::onDestroying()
{
	ASSERTION_COBJECT(this);

	ClientArea::onDestroying();
	for (int i=0; i<panes.GetCount(); i++)
		panes.Get(i)->onDestroying();
}

void SplittingArea::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ClientArea::getEventTargetAt(position);
	if (superTarget != this)
		return;

	Skin * skin = Desktop::getInstance()->getSkin();
	int splitterSize = skin->getSplitterSize(this);

	int offset = 0;
	if (direction == HORIZONTAL)
		for (int i=0; i<panes.GetCount(); i++)
		{
			ClientArea * pane = panes.Get(i);
			if (pane->isVisible() && position.x >= offset
				&& position.x < offset + pane->getSize().x)
			{
				setFocusPane(pane);
				return;
			}
			offset += pane->getSize().x + splitterSize;
		}
	else
		for (int i=0; i<panes.GetCount(); i++)
		{
			ClientArea * pane = panes.Get(i);
			if (pane->isVisible() && position.y >= offset
				&& position.y < offset + pane->getSize().y)
			{
				setFocusPane(pane);
				return;
			}
			offset += pane->getSize().y + splitterSize;
		}
}

bool SplittingArea::onTab(bool secondary)
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

void SplittingArea::prependPane(ClientArea * pane)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The splitting area must be added to a parent"
			" before using it");

	panes.Prepend(pane);
	setComponentParent(pane);

	if (panes.GetCount() == 1)
		resizeInternal(pane->getSize(), true);
	else
		resizeForNewPane(pane);

	adjustClientAreas();
	invalidate();
}

void SplittingArea::releaseCache()
{
	ASSERTION_COBJECT(this);

	ClientArea::releaseCache();

	for (int i=0; i<panes.GetCount(); i++)
		panes.Get(i)->releaseCache();
}

void SplittingArea::removePane(int position)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The splitting area must be added to a parent"
			" before using it");

	ClientArea * pane = panes.Get(position);
	if (pane == NULL)
		return;

	Vector areaSize = getSize();
	Vector paneSize = pane->getSize();
	if (panes.GetCount() > 1)
		if (direction == HORIZONTAL)
			resizeInternal(Vector(areaSize.x - paneSize.x, areaSize.y), true);
		else
			resizeInternal(Vector(areaSize.x, areaSize.y - paneSize.y), true);

	panes.Unlink(position);
	pane->destroy();

	adjustClientAreas();
	invalidate();
}

void SplittingArea::resizeForNewPane(ClientArea * pane)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	int splitterSize = skin->getSplitterSize(this);

	Vector areaSize = getSize();
	Vector paneSize = pane->getSize();
	if (direction == HORIZONTAL)
		if (paneSize.y > areaSize.y)
			resizeInternal(Vector(areaSize.x + paneSize.x + splitterSize, paneSize.y), true);
		else
			resizeInternal(Vector(areaSize.x + paneSize.x + splitterSize, areaSize.y), true);
	else
		if (paneSize.x > areaSize.x)
			resizeInternal(Vector(paneSize.x, areaSize.y + paneSize.y + splitterSize), true);
		else
			resizeInternal(Vector(areaSize.x, areaSize.y + paneSize.y + splitterSize), true);
}

bool SplittingArea::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool resized = ClientArea::resizeInternal(size, notifyParent);
	if (resized)
		adjustClientAreas();

	return resized;
}

void SplittingArea::setFocusPane(ClientArea * focusPane)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The splitting area must be added to a parent"
			" before using it");

	if (this->focusPane == focusPane)
		return;

	if (this->focusPane != NULL)
		this->focusPane->onLostFocus();

	this->focusPane = focusPane;

	if (this->focusPane != NULL)
		this->focusPane->onGotFocus(false);
}

bool SplittingArea::setFocusToNextPane()
{
	ASSERTION_COBJECT(this);
	return ClientArea::setFocusToNextPane();
}

bool SplittingArea::setFocusToPreviousPane()
{
	ASSERTION_COBJECT(this);
	return ClientArea::setFocusToPreviousPane();
}

void SplittingArea::setPanesResizable(bool panesResizable)
{
	ASSERTION_COBJECT(this);
	this->panesResizable = panesResizable;
}
