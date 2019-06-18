/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SingleFieldPopup::SingleFieldPopup()
{
	ASSERTION_COBJECT(this);

	autoDestroying = true;
	contentField = NULL;

	Desktop::getInstance()->getSkin()->setupSingleFieldPopup(this);
}

SingleFieldPopup::~SingleFieldPopup()
{
	ASSERTION_COBJECT(this);

	if (contentField != NULL)
		if (autoDestroying)
		{
			deleteChild(contentField);
			contentField = NULL;
		}
		else
			unsetComponentParent(contentField);
}

void SingleFieldPopup::adjustField()
{
	ASSERTION_COBJECT(this);

	if (contentField != NULL)
	{
		Vector topLeftSize;
		Vector bottomRightSize;
		Desktop::getInstance()->getSkin()->getSingleFieldPopupBorderSizes(this,
			topLeftSize, bottomRightSize);

		moveComponent(contentField, topLeftSize);
		resizeComponent(contentField, getSize() - topLeftSize - bottomRightSize);
		setComponentMinimumSize(contentField, getMinimumSize() - topLeftSize - bottomRightSize, false);
		setComponentMaximumSize(contentField, getMaximumSize() - topLeftSize - bottomRightSize, false);
	}
}

void SingleFieldPopup::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawSingleFieldPopup(context, this);
	if (contentField != NULL && contentField->isVisible())
		contentField->draw(DrawingContext(context, contentField));
}

Field * SingleFieldPopup::getContentField()
{
	ASSERTION_COBJECT(this);
	return contentField;
}

EventTarget * SingleFieldPopup::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = Popup::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (contentField != NULL && contentField->isVisible())
		return contentField->getEventTargetAt(position - contentField->getPosition());

	return this;
}

Component * SingleFieldPopup::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return contentField;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SingleFieldPopup, Popup);
#endif

bool SingleFieldPopup::isAutoDestroying()
{
	ASSERTION_COBJECT(this);
	return autoDestroying;
}

bool SingleFieldPopup::moveInternal(Vector position, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool moved = Popup::moveInternal(position, notifyParent);
	if (moved)
		adjustField();

	return moved;
}

void SingleFieldPopup::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMoved(child, oldPosition);
	moveInternal(getPosition() - oldPosition + child->getPosition(), true);
}

void SingleFieldPopup::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMaximumSizeChanged(child, oldMaximumSize);
	setMaximumSize(getMaximumSize() + child->getMaximumSize() - oldMaximumSize);
}

void SingleFieldPopup::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildMinimumSizeChanged(child, oldMinimumSize);
	setMinimumSize(getMinimumSize() + child->getMinimumSize() - oldMinimumSize);
}

void SingleFieldPopup::onChildResized(Component * contentField, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	Popup::onChildResized(contentField, oldSize);
	if (contentField != NULL)
		resizeInternal(getSize() - oldSize + contentField->getSize(), true);
}

void SingleFieldPopup::onDestroying()
{
	ASSERTION_COBJECT(this);

	Popup::onDestroying();
	if (contentField != NULL)
		contentField->onDestroying();
}

void SingleFieldPopup::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onMaximumSizeChanged(oldMaximumSize);
	if (contentField != NULL)
		setComponentMaximumSize(contentField, getMaximumSize(), false);
}

void SingleFieldPopup::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	Popup::onMinimumSizeChanged(oldMinimumSize);
	if (contentField != NULL)
		setComponentMinimumSize(contentField, getMinimumSize(), false);
}

bool SingleFieldPopup::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	bool resized = Popup::resizeInternal(size, notifyParent);
	if (resized)
		adjustField();

	return resized;
}

void SingleFieldPopup::setAutoDestroying(bool autoDestroying)
{
	ASSERTION_COBJECT(this);
	this->autoDestroying = autoDestroying;
}

void SingleFieldPopup::setContentField(Field * contentField)
{
	ASSERTION_COBJECT(this);

	if (this->contentField != NULL && autoDestroying)
		deleteChild(this->contentField);

	this->contentField = contentField;

	if (contentField != NULL)
	{
		setComponentParent(contentField);
		adjustField();
	}

	invalidate();
}
