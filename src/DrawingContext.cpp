/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


DrawingContext::DrawingContext()
{
	ASSERTION_COBJECT(this);

	target = NULL;
}

DrawingContext::DrawingContext(Desktop * desktop)
{
	ASSERTION_COBJECT(this);

	clippingSize = desktop->getPlatformAdapter()->getScreenSize();
	target = NULL;
}

DrawingContext::DrawingContext(Image * target)
{
	ASSERTION_COBJECT(this);

	clippingSize = target->getSize();
	this->target = target;
}

DrawingContext::DrawingContext(const DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	clippingOffset = context.clippingOffset;
	clippingSize = context.clippingSize;
	drawingOffset = context.drawingOffset;
	target = context.target;
}

DrawingContext::DrawingContext(const DrawingContext & context, Vector offset)
{
	ASSERTION_COBJECT(this);

	clippingOffset = context.clippingOffset;
	clippingSize = context.clippingSize;
	drawingOffset = context.drawingOffset + offset;
	target = context.target;
}

DrawingContext::DrawingContext(const DrawingContext & context, Vector offset, Vector size)
{
	ASSERTION_COBJECT(this);

	clippingOffset = context.clippingOffset;
	clippingSize = context.clippingSize;
	drawingOffset = context.drawingOffset;
	target = context.target;

	clipTo(offset, size);
}

DrawingContext::DrawingContext(const DrawingContext & context, Component * component)
{
	ASSERTION_COBJECT(this);

	clippingOffset = context.clippingOffset;
	clippingSize = context.clippingSize;
	drawingOffset = context.drawingOffset;
	target = context.target;

	clipTo(component->getPosition(), component->getSize());
}

void DrawingContext::operator =(const DrawingContext & drawingContext)
{
	ASSERTION_COBJECT(this);

	target = drawingContext.target;
	drawingOffset = drawingContext.drawingOffset;
	clippingOffset = drawingContext.clippingOffset;
	clippingSize = drawingContext.clippingSize;
}

bool DrawingContext::operator ==(const DrawingContext & drawingContext) const
{
	ASSERTION_COBJECT(this);

	bool equal = target == drawingContext.target
		&& drawingOffset == drawingContext.drawingOffset
		&& clippingOffset == drawingContext.clippingOffset
		&& clippingSize == drawingContext.clippingSize;
	return equal;
}

bool DrawingContext::operator !=(const DrawingContext & drawingContext) const
{
	ASSERTION_COBJECT(this);

	bool notEqual = target != drawingContext.target
		|| drawingOffset != drawingContext.drawingOffset
		|| clippingOffset != drawingContext.clippingOffset
		|| clippingSize != drawingContext.clippingSize;
	return notEqual;
}

void DrawingContext::clipTo(Vector offset, Vector size)
{
	ASSERTION_COBJECT(this);

	// The drawing offset is absolute to the target,
	// and the offset is relative to the current drawing offset,
	// so the new drawing offset is translated by the offset.
	drawingOffset += offset;

	// If the bottom-right position of the new requested rectangle
	// is below or right to the current clipping size,
	// the new size must be intersected (clipped).
	if (drawingOffset.x - clippingOffset.x + size.x > clippingSize.x)
		size.x = clippingSize.x - drawingOffset.x + clippingOffset.x;
	if (drawingOffset.y - clippingOffset.y + size.y > clippingSize.y)
		size.y = clippingSize.y - drawingOffset.y + clippingOffset.y;

	// If the top-left position of the new requested rectangle
	// is above or left to the current clipping offset,
	// the offset must be translated and the size must be reduced.
	// Otherwise, the drawing offset becomes the new clipping offset.

	if (drawingOffset.x < clippingOffset.x)
		clippingSize.x = size.x + drawingOffset.x - clippingOffset.x;
	else
	{
		clippingOffset.x = drawingOffset.x;
		clippingSize.x = size.x;
	}

	if (drawingOffset.y < clippingOffset.y)
		clippingSize.y = size.y + drawingOffset.y - clippingOffset.y;
	else
	{
		clippingOffset.y = drawingOffset.y;
		clippingSize.y = size.y;
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DrawingContext, CObject);
#endif

bool DrawingContext::isCompatibleTo(const DrawingContext & drawingContext) const
{
	ASSERTION_COBJECT(this);

	bool equal = target == drawingContext.target
		&& clippingOffset == drawingContext.clippingOffset
		&& clippingSize == drawingContext.clippingSize;
	return equal;
}

bool DrawingContext::isInClippingArea(Vector position) const
{
	ASSERTION_COBJECT(this);

	if (position.x < clippingOffset.x || position.y < clippingOffset.y)
		return false;

	if (position.x >= clippingOffset.x + clippingSize.x || position.y >= clippingOffset.y + clippingSize.y)
		return false;

	return true;
}

bool DrawingContext::isInClippingArea(Vector from, Vector to) const
{
	ASSERTION_COBJECT(this);

	if (from.x > to.x)
	{
		int temp = from.x;
		from.x = to.x;
		to.x = temp;
	}

	if (from.y > to.y)
	{
		int temp = from.y;
		from.y = to.y;
		to.y = temp;
	}

	if (to.x < clippingOffset.x || to.y < clippingOffset.y)
		return false;

	if (from.x >= clippingOffset.x + clippingSize.x || from.y >= clippingOffset.y + clippingSize.y)
		return false;

	return true;
}
