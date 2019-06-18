/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


DraggingSurrogate::DraggingSurrogate()
{
	ASSERTION_COBJECT(this);

	supportingPointerType = NULL;
	unsupportingPointerType = NULL;
}

DraggingSurrogate::~DraggingSurrogate()
{
	ASSERTION_COBJECT(this);
}

DraggingSurrogate * DraggingSurrogate::createDefault()
{
	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;

	return surrogate;
}

void DraggingSurrogate::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	// By default, draws nothing. This is useful to define the pointer types, only.
}

Vector DraggingSurrogate::getOffset()
{
	ASSERTION_COBJECT(this);
	return offset;
}

PointerType * DraggingSurrogate::getSupportingPointerType()
{
	ASSERTION_COBJECT(this);
	return supportingPointerType;
}

PointerType * DraggingSurrogate::getUnsupportingPointerType()
{
	ASSERTION_COBJECT(this);
	return unsupportingPointerType;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DraggingSurrogate, GlazieryObject);
#endif

void DraggingSurrogate::onCanceled(EventTarget * draggedObject)
{
	ASSERTION_COBJECT(this);
}

void DraggingSurrogate::onDragMove(EventTarget * draggedObject, const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);
}

void DraggingSurrogate::onDropped(EventTarget * draggedObject, EventTarget * dropTarget,
	Vector relativePosition, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void DraggingSurrogate::setOffset(Vector offset)
{
	ASSERTION_COBJECT(this);
	this->offset = offset;
}

void DraggingSurrogate::setSupportingPointerType(PointerType * supportingPointerType)
{
	ASSERTION_COBJECT(this);
	this->supportingPointerType = supportingPointerType;
}

void DraggingSurrogate::setUnsupportingPointerType(PointerType * unsupportingPointerType)
{
	ASSERTION_COBJECT(this);
	this->unsupportingPointerType = unsupportingPointerType;
}

bool DraggingSurrogate::supportsDropping(EventTarget * draggedObject,
	EventTarget * dropTarget, Vector relativePosition)
{
	ASSERTION_COBJECT(this);
	return false;
}
