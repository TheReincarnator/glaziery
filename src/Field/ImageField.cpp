/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ImageField::ImageField()
{
	ASSERTION_COBJECT(this);

	image = NULL;

	Desktop::getInstance()->getSkin()->setupImageField(this);
}

ImageField::~ImageField()
{
	ASSERTION_COBJECT(this);
}

void ImageField::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawImageField(context, this);
}

Image * ImageField::getImage()
{
	ASSERTION_COBJECT(this);
	return image;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ImageField, Field);
#endif

void ImageField::setImage(Image * image)
{
	ASSERTION_COBJECT(this);

	if (this->image == image)
		return;

	this->image = image;
	if (image != NULL)
		resizeInternal(image->getSize(), true);

	invalidate();
}

void ImageField::setImageId(const String & imageId)
{
	ASSERTION_COBJECT(this);
	setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(imageId, true));
}

void ImageField::setImageId(const char * imageId)
{
	ASSERTION_COBJECT(this);
	setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(String(imageId), true));
}

String ImageField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("ImageField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}
