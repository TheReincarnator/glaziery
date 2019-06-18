/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


void BottomRightCustomArea::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->clear(context);

	const ArrayList<Widget> & widgets = getWidgets();
	for (int i=0; i<widgets.GetCount(); i++)
	{
		BottomRightCustomArea::BallWidget * widget = (BallWidget *) widgets.Get(i);
		if (widget->show)
			widget->draw(context);
	}

	Vector vertices[3];
	vertices[0].x = 30; vertices[0].y = 30;
	vertices[1].x = 80; vertices[1].y = 40;
	vertices[2].x = 50; vertices[2].y = 90;
	adapter->drawTriangle(context, vertices,
		GL_RGBA(128, 190, 128, 64), GL_RGBA(255, 128, 128, 255));

	// Area never gets valid
	invalidate();
}

void BottomRightCustomArea::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	BallWidget * widget;
	if ((widget = new BallWidget()) == NULL)
		throw EOUTOFMEMORY;
	appendWidget(widget);
	widget->position = position * 10;
}


BottomRightCustomArea::BallWidget::BallWidget()
{
	ASSERTION_COBJECT(this);

	direction = Vector(1, 1);
	position = Vector(160, 80);
	pressed = false;
	show = true;
}

void BottomRightCustomArea::BallWidget::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	BottomRightCustomArea * area = (BottomRightCustomArea *) getParent();
	Vector areaSize = area->getSize();
	adapter->drawRect(context, position / 10, position / 10 + Vector(15, 15), GL_RGBA(224, 255, 128, 255));

	if (pressed)
		return;

	int millis = Desktop::getInstance()->getTimeStep();
	position += direction * millis;

	if (position.x < 0)
	{
		position.x = 0;
		direction.x = 1;
	}

	if (position.y < 0)
	{
		position.y = 0;
		direction.y = 1;
	}

	if (position.x > areaSize.x * 10 - 150)
	{
		position.x = areaSize.x * 10 - 150;
		direction.x = -1;
	}

	if (position.y > areaSize.y * 10 - 150)
	{
		position.y = areaSize.y * 10 - 150;
		direction.y = -1;
	}
}

bool BottomRightCustomArea::BallWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);
	return point >= position / 10 && point <= position / 10 + Vector(15, 15);
}

DraggingSurrogate * BottomRightCustomArea::BallWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	show = false;

	BallWidgetSurrogate * surrogate;
	if ((surrogate = new BallWidgetSurrogate) == NULL)
		throw EOUTOFMEMORY;
	return surrogate;
}

void BottomRightCustomArea::BallWidget::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	pressed = true;
}

void BottomRightCustomArea::BallWidget::onRelease(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	pressed = false;
}


void BottomRightCustomArea::BallWidgetSurrogate::onDropped(EventTarget * draggedObject,
	EventTarget * dropTarget, Vector relativePosition, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	BallWidget * widget = (BallWidget *) draggedObject;
	Vector direction = widget->direction;
	widget->getParent()->removeWidget(widget);

	if ((widget = new BallWidget) == NULL)
		throw EOUTOFMEMORY;
	((BottomRightCustomArea *) dropTarget)->appendWidget(widget);

	widget->position = relativePosition * 10;
	widget->direction = direction;
}

bool BottomRightCustomArea::BallWidgetSurrogate::supportsDropping(EventTarget * draggedObject,
	EventTarget * dropTarget, Vector relativePosition)
{
	ASSERTION_COBJECT(this);
	return dynamic_cast<BottomRightCustomArea *>(dropTarget) != NULL;
}
