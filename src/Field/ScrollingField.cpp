/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ScrollingField::ScrollingField()
{
	ASSERTION_COBJECT(this);

	borderSize = Vector(1, 1);
	constrainingScrollPosition = false;
	innerPaddingSize = Vector(1, 1);
	inScrollToNow = false;
	outerPaddingSize = Vector(1, 1);
	pressWithinContent = false;
	scrollerBreadth = 16;
	scrollingUnitSize = Vector(20, 20);

	Desktop::getInstance()->getSkin()->setupScrollingField(this);
}

ScrollingField::~ScrollingField()
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::constrainScrollPosition()
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

void ScrollingField::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	skin->drawScrollingField(context, this);

	drawContent(DrawingContext(DrawingContext(context, borderSize + outerPaddingSize,
		getContentViewSize()), -scrollPositionNow));
}

void ScrollingField::ensureVisible(Vector point)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPosition();
	if (scrollPosition.constrain(point - getContentViewSize() + Vector(1, 1), point))
		scrollTo(scrollPosition);
}

void ScrollingField::ensureVisible(Vector topLeft, Vector bottomRight)
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

void ScrollingField::ensureVisibleNow(Vector point)
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = getScrollPositionNow();
	if (scrollPosition.constrain(point - getContentViewSize() + Vector(1, 1), point))
	{
		Desktop::getInstance()->getSkin()->cancelScrollingFieldScrollEffects(this);
		scrollToNow(scrollPosition);
	}
}

void ScrollingField::ensureVisibleNow(Vector topLeft, Vector bottomRight)
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
		Desktop::getInstance()->getSkin()->cancelScrollingFieldScrollEffects(this);
		scrollToNow(scrollPosition);
	}
}

Vector ScrollingField::getBorderSize()
{
	ASSERTION_COBJECT(this);
	return borderSize;
}

EventTarget * ScrollingField::getContentEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);
	return this;
}

Vector ScrollingField::getContentSize()
{
	ASSERTION_COBJECT(this);
	return contentSize;
}

Vector ScrollingField::getContentViewSize()
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	return getSize() - getScrollerSizes() - borderSize * 2 - outerPaddingSize - getEffectiveInnerPaddingSize();
}

Vector ScrollingField::getEffectiveInnerPaddingSize()
{
	ASSERTION_COBJECT(this);

	return Vector(isVerticallyScrollable() ? innerPaddingSize.x : outerPaddingSize.x,
		isHorizontallyScrollable() ? innerPaddingSize.y : outerPaddingSize.y);
}

EventTarget * ScrollingField::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = InputField::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (isWithinContent(position))
		return getContentEventTargetAt(getPositionWithinContent(position));

	return this;
}

Vector ScrollingField::getInnerPaddingSize()
{
	ASSERTION_COBJECT(this);
	return innerPaddingSize;
}

Vector ScrollingField::getMaximumScrollPosition()
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

Vector ScrollingField::getOuterPaddingSize()
{
	ASSERTION_COBJECT(this);
	return outerPaddingSize;
}

Vector ScrollingField::getPositionWithinContent(Vector positionWithinField)
{
	ASSERTION_COBJECT(this);
	return positionWithinField + scrollPositionNow - borderSize - outerPaddingSize;
}

int ScrollingField::getScrollerBreadth()
{
	ASSERTION_COBJECT(this);
	return scrollerBreadth;
}

Vector ScrollingField::getScrollerSizes()
{
	ASSERTION_COBJECT(this);

	Vector scrollerSizes(0, 0);

	if (isHorizontallyScrollable())
		scrollerSizes.y = scrollerBreadth;
	if (isVerticallyScrollable())
		scrollerSizes.x = scrollerBreadth;

	return scrollerSizes;
}

Vector ScrollingField::getScrollingUnitSize()
{
	ASSERTION_COBJECT(this);
	return scrollingUnitSize;
}

Vector ScrollingField::getScrollPosition()
{
	ASSERTION_COBJECT(this);

	Vector scrollPosition = Desktop::getInstance()->getSkin()->getScrollingFieldScrollPosition(this);
	return scrollPosition;
}

Vector ScrollingField::getScrollPositionNow()
{
	ASSERTION_COBJECT(this);
	return scrollPositionNow;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ScrollingField, InputField);
#endif

void ScrollingField::invalidateArea(Vector position, Vector size)
{
	ASSERTION_COBJECT(this);

	Component * parent = getParent();
	if (parent != NULL && parent->isChildVisible(this))
		parent->invalidateArea(getPosition() + position, size);

	if (getParent() == NULL)
		return;

	constrainScrollPosition();
}

bool ScrollingField::isWithinContent(Vector position)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	return position >= borderSize + outerPaddingSize
		&& position < getSize() - getScrollerSizes() - borderSize - getEffectiveInnerPaddingSize();
}

void ScrollingField::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onClick(position, option1, option2);

	if (isWithinContent(position))
		onContentClick(getPositionWithinContent(position), option1, option2);
}

void ScrollingField::onContentClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentClick(this, position,
				option1, option2);
	}
}

void ScrollingField::onContentContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentContextClick(this, position,
				option1, option2);
	}
}

void ScrollingField::onContentDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentDoubleClick(this, position,
				option1, option2);
	}
}

long ScrollingField::onContentHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentHold(this, position, holdTime);
	}

	return 0;
}

void ScrollingField::onContentPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentPress(this, position,
				option1, option2);
	}
}

void ScrollingField::onContentRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onContentRelease(this,
				option1, option2);
	}
}

void ScrollingField::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onContextClick(position, option1, option2);

	if (isWithinContent(position))
		onContentContextClick(getPositionWithinContent(position), option1, option2);
}

void ScrollingField::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onDoubleClick(position, option1, option2);

	if (isWithinContent(position))
		onContentDoubleClick(getPositionWithinContent(position), option1, option2);
}

long ScrollingField::onHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	long remainingHoldTime = InputField::onHold(position, holdTime);

	if (pressWithinContent)
		return onContentHold(getPositionWithinContent(position), holdTime);
	else
		return remainingHoldTime;
}

void ScrollingField::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onPress(position, option1, option2);

	if (isWithinContent(position))
	{
		pressWithinContent = true;
		onContentPress(getPositionWithinContent(position), option1, option2);
	}
	else
		pressWithinContent = false;
}

void ScrollingField::onRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onRelease(option1, option2);

	if (pressWithinContent)
		onContentRelease(option1, option2);
}

void ScrollingField::onScrollPositionNowChanged(Vector previousPosition)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ScrollingField::Listener * scrollingFieldListener
			= dynamic_cast<ScrollingField::Listener *>(listeners.Get(i));
		if (scrollingFieldListener != NULL)
			scrollingFieldListener->onScrollPositionNowSet(this, previousPosition);
	}
}

void ScrollingField::onWheel(int delta)
{
	ASSERTION_COBJECT(this);

	InputField::onWheel(delta);

	if (isActiveIncludingAncestors())
	{
		int wheelLines = Desktop::getInstance()->getInputManager().GetWheelLines();
		if (wheelLines < 0)
		{
			Vector contentViewSize = getContentViewSize();
			scroll(Vector(0, -contentViewSize.y * delta));
		}
		else
			scroll(Vector(0, -16 * wheelLines * delta));
	}
}

void ScrollingField::scroll(Vector delta)
{
	ASSERTION_COBJECT(this);
	scrollTo(getScrollPosition() + delta);
}

void ScrollingField::scrollTo(Vector scrollPosition)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->setScrollingFieldScrollPosition(this, scrollPosition);
}

void ScrollingField::scrollToNow(Vector scrollPositionNow)
{
	ASSERTION_COBJECT(this);

	if (inScrollToNow)
		return;

	inScrollToNow = true;

	Vector maximumScrollPosition = getMaximumScrollPosition();

	if (scrollPositionNow.x > maximumScrollPosition.x)
		scrollPositionNow.x = maximumScrollPosition.x;
	if (scrollPositionNow.y > maximumScrollPosition.y)
		scrollPositionNow.y = maximumScrollPosition.y;

	if (scrollPositionNow.x < 0)
		scrollPositionNow.x = 0;
	if (scrollPositionNow.y < 0)
		scrollPositionNow.y = 0;

	if (this->scrollPositionNow == scrollPositionNow)
	{
		inScrollToNow = false;
		return;
	}

	Vector previousPosition = this->scrollPositionNow;
	this->scrollPositionNow = scrollPositionNow;

	invalidate();
	onScrollPositionNowChanged(previousPosition);

	inScrollToNow = false;
}

void ScrollingField::setBorderSize(Vector borderSize)
{
	ASSERTION_COBJECT(this);

	this->borderSize = borderSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingField::setInnerPaddingSize(Vector innerPaddingSize)
{
	ASSERTION_COBJECT(this);

	this->innerPaddingSize = innerPaddingSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingField::setOuterPaddingSize(Vector outerPaddingSize)
{
	ASSERTION_COBJECT(this);

	this->outerPaddingSize = outerPaddingSize;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingField::setScrollerBreadth(int scrollerBreadth)
{
	ASSERTION_COBJECT(this);

	this->scrollerBreadth = scrollerBreadth;

	// Performs scroll position checks, too.
	invalidate();
}

void ScrollingField::setScrollingUnitSize(Vector scrollingUnitSize)
{
	ASSERTION_COBJECT(this);
	this->scrollingUnitSize = scrollingUnitSize;
}


ScrollingField::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentContextClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentDoubleClick(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentHold(Component * component, Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentPress(Component * component,
	Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onContentRelease(Component * component,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ScrollingField::Listener::onScrollPositionNowSet(ScrollingField * field, Vector previousPosition)
{
	ASSERTION_COBJECT(this);
}
