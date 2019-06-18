/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SingleFieldArea::SingleFieldArea()
{
	ASSERTION_COBJECT(this);

	contentField = NULL;

	Desktop::getInstance()->getSkin()->setupSingleFieldArea(this);
}

SingleFieldArea::~SingleFieldArea()
{
	ASSERTION_COBJECT(this);

	if (contentField != NULL) {deleteChild(contentField); contentField = NULL;}
}

void SingleFieldArea::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawSingleFieldArea(context, this);
	if (contentField != NULL && contentField->isVisible())
		contentField->draw(context);
}

class Field * SingleFieldArea::getContentField()
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The single field area must be added to a parent"
			" before using it");

	return contentField;
}

EventTarget * SingleFieldArea::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ClientArea::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (contentField != NULL && contentField->isVisible())
		return contentField->getEventTargetAt(position);

	return this;
}

Component * SingleFieldArea::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return contentField;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SingleFieldArea, ClientArea);
#endif

bool SingleFieldArea::moveInternal(Vector position, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool moved = ClientArea::moveInternal(position, notifyParent);
	if (moved && contentField != NULL)
		moveComponent(contentField, Vector());
	return moved;
}

void SingleFieldArea::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildMoved(child, oldPosition);
	moveInternal(getPosition() - oldPosition + child->getPosition(), true);
}

void SingleFieldArea::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildMaximumSizeChanged(child, oldMaximumSize);
	setMaximumSize(getMaximumSize() + child->getMaximumSize() - oldMaximumSize);
}

void SingleFieldArea::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildMinimumSizeChanged(child, oldMinimumSize);
	setMinimumSize(getMinimumSize() + child->getMinimumSize() - oldMinimumSize);
}

void SingleFieldArea::onChildResized(Component * contentField, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onChildResized(contentField, oldSize);
	if (contentField != NULL)
		resizeInternal(getSize() - oldSize + contentField->getSize(), true);
}

void SingleFieldArea::onDestroying()
{
	ASSERTION_COBJECT(this);

	ClientArea::onDestroying();
	if (contentField != NULL)
		contentField->onDestroying();
}

void SingleFieldArea::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onMaximumSizeChanged(oldMaximumSize);
	if (contentField != NULL)
		setComponentMaximumSize(contentField, getMaximumSize(), false);
}

void SingleFieldArea::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	ClientArea::onMinimumSizeChanged(oldMinimumSize);
	if (contentField != NULL)
		setComponentMinimumSize(contentField, getMinimumSize(), false);
}

bool SingleFieldArea::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool resized = ClientArea::resizeInternal(size, notifyParent);
	if (resized && contentField != NULL)
		resizeComponent(contentField, getSize());

	return resized;
}

void SingleFieldArea::setContentField(Field * contentField)
{
	ASSERTION_COBJECT(this);

	if (this->contentField != NULL)
		deleteChild(this->contentField);

	this->contentField = contentField;

	ScrollingField * scrollingField = dynamic_cast<ScrollingField *>(contentField);
	if (scrollingField != NULL)
	{
		scrollingField->setBorderSize(Vector());
		scrollingField->setInnerPaddingSize(Vector(2, 2));
		scrollingField->setOuterPaddingSize(Vector(2, 2));
	}

	if (contentField != NULL)
	{
		setComponentParent(contentField);
		moveComponent(contentField, Vector());
		resizeComponent(contentField, getSize());
	}

	invalidate();
}
