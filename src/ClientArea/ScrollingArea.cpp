/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ScrollingArea::ScrollingArea()
{
	ASSERTION_COBJECT(this);

	borderSize = Vector(1, 1);
	constrainingScrollPosition = false;
	horizontallyScrollable = false;
	innerPaddingSize = Vector(1, 1);
	outerPaddingSize = Vector(1, 1);
	pressWithinContent = false;
	verticallyScrollable = false;
	scrollerBreadth = 16;
	scrollingUnitSize = Vector(20, 20);

	Desktop::getInstance()->getSkin()->setupScrollingArea(this);
}

ScrollingArea::~ScrollingArea()
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::constrainScrollPosition()
{
	ASSERTION_COBJECT(this);

	if (constrainingScrollPosition)
		return;

	constrainingScrollPosition = true;

	Vector scrollPosition = getScrollPosition();
	Vector maximumScrollPosition = getMaximumScrollPosition();
	if (!(scrollPosition >= Vector()) || !(scrollPosition <= maximumScrollPosition))
	{
		if (scrollPosition.x > maximumScrollPosition.x)
			scrollPosition.x = maximumScrollPosition.x;
		if (scrollPosition.y > maximumScrollPosition.y)
			scrollPosition.y = maximumScrollPosition.y;

		if (scrollPosition.x < 0)
			scrollPosition.x = 0;
		if (scrollPosition.y < 0)
			scrollPosition.y = 0;

		scrollTo(scrollPosition);
	}

	constrainingScrollPosition = false;
}

void ScrollingArea::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	skin->drawScrollingArea(context, this);

	drawAreaContent(DrawingContext(DrawingContext(context, borderSize + outerPaddingSize,
		getContentViewSize()), -scrollPositionNow));
}

void ScrollingArea::ensureVisible(Vector point)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPosition();
	if (scrollPosition.constrain(point - getContentViewSize() + Vector(1, 1), point))
		scrollTo(scrollPosition);
}

void ScrollingArea::ensureVisible(Vector topLeft, Vector bottomRight)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPosition();

	bool constrained = false;
	if (scrollPosition.constrain(bottomRight - getContentViewSize() + Vector(1, 1), bottomRight))
		constrained = true;
	if (scrollPosition.constrain(topLeft - getContentViewSize() + Vector(1, 1), topLeft))
		constrained = true;

	if (constrained)
		scrollTo(scrollPosition);
}

void ScrollingArea::ensureVisibleNow(Vector point)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPositionNow();
	if (scrollPosition.constrain(point - getContentViewSize() + Vector(1, 1), point))
	{
		Desktop::getInstance()->getSkin()->cancelScrollingAreaScrollEffects(this);
		scrollToNow(scrollPosition);
	}
}

void ScrollingArea::ensureVisibleNow(Vector topLeft, Vector bottomRight)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPositionNow();

	bool constrained = false;
	if (scrollPosition.constrain(bottomRight - getContentViewSize() + Vector(1, 1), bottomRight))
		constrained = true;
	if (scrollPosition.constrain(topLeft - getContentViewSize() + Vector(1, 1), topLeft))
		constrained = true;

	if (constrained)
	{
		Desktop::getInstance()->getSkin()->cancelScrollingAreaScrollEffects(this);
		scrollToNow(scrollPosition);
	}
}

Vector ScrollingArea::getBorderSize()
{
	ASSERTION_COBJECT(this);
	return borderSize;
}

Vector ScrollingArea::getChildrenOrigin()
{
	ASSERTION_COBJECT(this);
	return getAbsolutePosition() - scrollPositionNow;
}

EventTarget * ScrollingArea::getContentEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);
	return this;
}

Vector ScrollingArea::getContentSize()
{
	ASSERTION_COBJECT(this);
	return contentSize;
}

Vector ScrollingArea::getContentViewSize()
{
	ASSERTION_COBJECT(this);
	return getSize() - getScrollerSizes() - borderSize * 2 - outerPaddingSize - getEffectiveInnerPaddingSize();
}

Vector ScrollingArea::getEffectiveInnerPaddingSize()
{
	ASSERTION_COBJECT(this);

	return Vector(isVerticallyScrollable() ? innerPaddingSize.x : outerPaddingSize.x,
		isHorizontallyScrollable() ? innerPaddingSize.y : outerPaddingSize.y);
}

EventTarget * ScrollingArea::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ClientArea::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (isWithinContent(position))
		return getContentEventTargetAt(getPositionWithinContent(position));

	return this;
}

Vector ScrollingArea::getInnerPaddingSize()
{
	ASSERTION_COBJECT(this);
	return innerPaddingSize;
}

Vector ScrollingArea::getMaximumScrollPosition()
{
	ASSERTION_COBJECT(this);

	Vector contentViewSize = getContentViewSize();

	Vector maximumScrollPosition = contentSize - contentViewSize;
	if (maximumScrollPosition.x < 0)
		maximumScrollPosition.x = 0;
	if (maximumScrollPosition.y < 0)
		maximumScrollPosition.y = 0;

	return maximumScrollPosition;
}

Vector ScrollingArea::getOuterPaddingSize()
{
	ASSERTION_COBJECT(this);
	return outerPaddingSize;
}

Vector ScrollingArea::getPositionWithinContent(Vector positionWithinArea)
{
	ASSERTION_COBJECT(this);
	return positionWithinArea + scrollPositionNow - borderSize - outerPaddingSize;
}

int ScrollingArea::getScrollerBreadth()
{
	ASSERTION_COBJECT(this);
	return scrollerBreadth;
}

Vector ScrollingArea::getScrollerSizes()
{
	ASSERTION_COBJECT(this);

	Vector scrollerSizes(0, 0);

	if (isHorizontallyScrollable())
		scrollerSizes.y = scrollerBreadth;
	if (isVerticallyScrollable())
		scrollerSizes.x = scrollerBreadth;

	return scrollerSizes;
}

Vector ScrollingArea::getScrollingUnitSize()
{
	ASSERTION_COBJECT(this);
	return scrollingUnitSize;
}

Vector ScrollingArea::getScrollPosition()
{
	ASSERTION_COBJECT(this);
	return Desktop::getInstance()->getSkin()->getScrollingAreaScrollPosition(this);
}

Vector ScrollingArea::getScrollPositionNow()
{
	ASSERTION_COBJECT(this);
	return scrollPositionNow;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ScrollingArea, ClientArea);
#endif

void ScrollingArea::invalidateArea(Vector position, Vector size)
{
	ASSERTION_COBJECT(this);

	Component * parent = getParent();
	if (parent != NULL && parent->isChildVisible(this))
		parent->invalidateArea(getPosition() + position - scrollPositionNow, size);

	if (parent == NULL)
		return;

	constrainScrollPosition();
}

bool ScrollingArea::isHorizontallyScrollable()
{
	ASSERTION_COBJECT(this);
	return horizontallyScrollable;
}

bool ScrollingArea::isVerticallyScrollable()
{
	ASSERTION_COBJECT(this);
	return verticallyScrollable;
}

bool ScrollingArea::isWithinContent(Vector position)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	return position >= borderSize + outerPaddingSize
		&& position < getSize() - getScrollerSizes() - borderSize - getEffectiveInnerPaddingSize();
}

void ScrollingArea::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ClientArea::onClick(position, option1, option2);

	if (isWithinContent(position))
		onContentClick(getPositionWithinContent(position), option1, option2);
}

void ScrollingArea::onContentClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentClick(this, position,
				option1, option2);
	}
}

void ScrollingArea::onContentContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentContextClick(this, position,
				option1, option2);
	}
}

void ScrollingArea::onContentDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentDoubleClick(this, position,
				option1, option2);
	}
}

long ScrollingArea::onContentHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentHold(this, position, holdTime);
	}

	return 0;
}

void ScrollingArea::onContentPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentPress(this, position,
				option1, option2);
	}
}

void ScrollingArea::onContentRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onContentRelease(this,
				option1, option2);
	}
}

void ScrollingArea::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ClientArea::onContextClick(position, option1, option2);

	if (isWithinContent(position))
		onContentContextClick(getPositionWithinContent(position), option1, option2);
}

void ScrollingArea::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ClientArea::onDoubleClick(position, option1, option2);

	if (isWithinContent(position))
		onContentDoubleClick(getPositionWithinContent(position), option1, option2);
}

long ScrollingArea::onHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	long remainingHoldTime = ClientArea::onHold(position, holdTime);

	if (pressWithinContent)
		return onContentHold(getPositionWithinContent(position), holdTime);
	else
		return remainingHoldTime;
}

bool ScrollingArea::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveDown(option1, option2))
		return true;

	scroll(Vector(0, scrollingUnitSize.y));

	return true;
}

bool ScrollingArea::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveLeft(option1, option2))
		return true;

	scroll(Vector(-scrollingUnitSize.x, 0));

	return true;
}

bool ScrollingArea::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveRight(option1, option2))
		return true;

	scroll(Vector(scrollingUnitSize.x, 0));

	return true;
}

bool ScrollingArea::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveToEnd(option1, option2))
		return true;

	Vector maximumScrollPosition = getMaximumScrollPosition();
	scrollTo(Vector(getScrollPosition().x, maximumScrollPosition.y));

	return true;
}

bool ScrollingArea::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveToStart(option1, option2))
		return true;

	scrollTo(Vector(getScrollPosition().x, 0));

	return true;
}

bool ScrollingArea::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onMoveUp(option1, option2))
		return true;

	scroll(Vector(0, -scrollingUnitSize.y));

	return true;
}

bool ScrollingArea::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onPageDown(option1, option2))
		return true;

	Vector contentViewSize = getContentViewSize();
	scroll(Vector(0, contentViewSize.y));

	return true;
}

bool ScrollingArea::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (ClientArea::onPageUp(option1, option2))
		return true;

	Vector contentViewSize = getContentViewSize();
	scroll(Vector(0, -contentViewSize.y));

	return true;
}

void ScrollingArea::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ClientArea::onPress(position, option1, option2);

	if (isWithinContent(position))
	{
		pressWithinContent = true;
		onContentPress(getPositionWithinContent(position), option1, option2);
	}
	else
		pressWithinContent = false;
}

void ScrollingArea::onRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ClientArea::onRelease(option1, option2);

	if (pressWithinContent)
		onContentRelease(option1, option2);
}

void ScrollingArea::onWheel(int delta)
{
	ASSERTION_COBJECT(this);

	ClientArea::onWheel(delta);

	Vector contentViewSize = getContentViewSize();
	scroll(Vector(0, -contentViewSize.y * delta));
}

void ScrollingArea::scroll(Vector delta)
{
	ASSERTION_COBJECT(this);
	scrollTo(getScrollPosition() + delta);
}

void ScrollingArea::scrollTo(Vector scrollPosition)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->setScrollingAreaScrollPosition(this, scrollPosition);
}

void ScrollingArea::scrollToNow(Vector scrollPositionNow)
{
	ASSERTION_COBJECT(this);

	if (this->scrollPositionNow == scrollPositionNow)
		return;

	Vector previousPosition = this->scrollPositionNow;
	this->scrollPositionNow = scrollPositionNow;

	// Performs scroll position checks, too.
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingArea::Listener * scrollingAreaListener
			= dynamic_cast<ScrollingArea::Listener *>(listeners.Get(i));
		if (scrollingAreaListener != NULL)
			scrollingAreaListener->onScrollPositionNowSet(this, previousPosition);
	}
}

void ScrollingArea::setBorderSize(Vector borderSize)
{
	ASSERTION_COBJECT(this);

	if (this->borderSize == borderSize)
		return;

	this->borderSize = borderSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingArea::setContentSize(Vector contentSize)
{
	ASSERTION_COBJECT(this);

	if (this->contentSize == contentSize)
		return;

	this->contentSize = contentSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingArea::setHorizontallyScrollable(bool horizontallyScrollable)
{
	ASSERTION_COBJECT(this);

	if (this->horizontallyScrollable == horizontallyScrollable)
		return;

	this->horizontallyScrollable = horizontallyScrollable;
	invalidate();
}

void ScrollingArea::setInnerPaddingSize(Vector innerPaddingSize)
{
	ASSERTION_COBJECT(this);

	if (this->innerPaddingSize == innerPaddingSize)
		return;

	this->innerPaddingSize = innerPaddingSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingArea::setOuterPaddingSize(Vector outerPaddingSize)
{
	ASSERTION_COBJECT(this);

	if (this->outerPaddingSize == outerPaddingSize)
		return;

	this->outerPaddingSize = outerPaddingSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingArea::setScrollerBreadth(int scrollerBreadth)
{
	ASSERTION_COBJECT(this);

	if (this->scrollerBreadth == scrollerBreadth)
		return;

	this->scrollerBreadth = scrollerBreadth;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingArea::setScrollingUnitSize(Vector scrollingUnitSize)
{
	ASSERTION_COBJECT(this);

	if (this->scrollingUnitSize == scrollingUnitSize)
		return;

	this->scrollingUnitSize = scrollingUnitSize;
}

void ScrollingArea::setVerticallyScrollable(bool verticallyScrollable)
{
	ASSERTION_COBJECT(this);

	if (this->verticallyScrollable == verticallyScrollable)
		return;

	this->verticallyScrollable = verticallyScrollable;
	invalidate();
}


ScrollingArea::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentContextClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentDoubleClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentHold(Component * component, Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentPress(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onContentRelease(Component * component,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingArea::Listener::onScrollPositionNowSet(ScrollingArea * area, Vector previousPosition)
{
	ASSERTION_COBJECT(this);
}
