/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Image::Image(const String & path, Image::Type imageType)
{
	ASSERTION_COBJECT(this);

	this->imageType = imageType;
	this->path = path;
}

Image::~Image()
{
	ASSERTION_COBJECT(this);
}

Image::Type Image::getImageType()
{
	ASSERTION_COBJECT(this);
	return imageType;
}

Vector Image::getMaximumSize()
{
	ASSERTION_COBJECT(this);
	return maximumSize;
}

const String & Image::getPath()
{
	ASSERTION_COBJECT(this);
	return path;
}

Vector Image::getSize()
{
	ASSERTION_COBJECT(this);
	return size;
}

bool Image::hasAlpha()
{
	ASSERTION_COBJECT(this);
	return imageType != TYPE_RGB;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Image, Listenable);
#endif

void Image::setMaximumSize(Vector maximumSize)
{
	ASSERTION_COBJECT(this);

	this->maximumSize = maximumSize;

	Vector constrainedSize = size;
	if (maximumSize.x > 0 && constrainedSize.x > maximumSize.x)
		constrainedSize.x = maximumSize.x;
	if (maximumSize.y > 0 && constrainedSize.y > maximumSize.y)
		constrainedSize.y = maximumSize.y;

	if (constrainedSize != size)
		resize(constrainedSize);
}


void Image::Listener::onRenderTargetInvalidated(Image * renderTarget)
{
	ASSERTION_COBJECT(this);
}
