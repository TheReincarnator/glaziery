/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SimplePointerType::SimplePointerType(Image * image, Vector hotSpot)
{
	ASSERTION_COBJECT(this);

	this->image = image;
	this->hotSpot = hotSpot;
}

void SimplePointerType::draw(DrawingContext & context, Vector position)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->drawImage(context, image, position - hotSpot);
}

Vector SimplePointerType::getHotSpot()
{
	ASSERTION_COBJECT(this);
	return hotSpot;
}

Image * SimplePointerType::getImage()
{
	ASSERTION_COBJECT(this);
	return image;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SimplePointerType, PointerType);
#endif
