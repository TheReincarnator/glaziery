/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ImageElement::ImageElement()
{
	ASSERTION_COBJECT(this);

	image = NULL;
	setEditable(false);
}

ImageElement::~ImageElement()
{
	ASSERTION_COBJECT(this);
}

void ImageElement::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (image != NULL)
		Desktop::getInstance()->getSkin()->drawImageElement(context, this);
}

void ImageElement::edit(Vector position, Vector size, ScrollingField * scrollingField)
{
	ASSERTION_COBJECT(this);
	// Image elements do not support editing
}

Image * ImageElement::getImage()
{
	ASSERTION_COBJECT(this);
	return image;
}

Vector ImageElement::getSize()
{
	ASSERTION_COBJECT(this);

	if (image != NULL)
		return image->getSize();
	else
		return Vector();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ImageElement, FieldElement);
#endif

void ImageElement::setImage(Image * image)
{
	ASSERTION_COBJECT(this);

	if (this->image == image)
		return;

	this->image = image;
	getField()->onElementChanged(this);
}

void ImageElement::setImageId(const String & imageId)
{
	ASSERTION_COBJECT(this);
	setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(imageId, true));
}

void ImageElement::setImageId(const char * imageId)
{
	ASSERTION_COBJECT(this);
	setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(String(imageId), true));
}
