/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_PLATFORMADAPTER_INL
#define __GLAZIERY_PLATFORMADAPTER_INL


inline void PlatformAdapter::drawEllipse(DrawingContext & context, Vector center, Vector radius,
	unsigned long fillColor)
{
	drawEllipse(context, center, radius, fillColor, fillColor);
}

inline void PlatformAdapter::drawImage(DrawingContext & context, Image * image,
	Vector targetPosition, unsigned long modulationColor, BlendType blendType)
{
	drawImage(context, image, Vector(), image->getSize(), targetPosition, image->getSize(),
		modulationColor, blendType);
}

inline void PlatformAdapter::drawImage(DrawingContext & context, Image * image,
	Vector targetPosition, Vector targetSize, unsigned long modulationColor, BlendType blendType)
{
	drawImage(context, image, Vector(), image->getSize(), targetPosition, targetSize,
		modulationColor, blendType);
}

inline void PlatformAdapter::drawImage(DrawingContext & context, Image * image,
	Vector sourcePosition, Vector sourceSize, Vector targetPosition,
	unsigned long modulationColor, BlendType blendType)
{
	drawImage(context, image, sourcePosition, sourceSize, targetPosition, sourceSize,
		modulationColor, blendType);
}

inline void PlatformAdapter::drawRect(DrawingContext & context, Vector from, Vector to,
	unsigned long fillColor)
{
	drawRect(context, from, to, fillColor, fillColor);
}

inline void PlatformAdapter::drawRotatedImage(DrawingContext & context, Image * image,
	Vector targetPosition, Vector rotationAnchor, double rotationAngle,
	unsigned long modulationColor, BlendType blendType)
{
	drawRotatedImage(context, image, Vector(), image->getSize(), targetPosition,
		image->getSize(), rotationAnchor, rotationAngle, modulationColor, blendType);
}

inline void PlatformAdapter::drawRotatedImage(DrawingContext & context, Image * image,
	Vector targetPosition, Vector targetSize,
	Vector rotationAnchor, double rotationAngle,
	unsigned long modulationColor, BlendType blendType)
{
	drawRotatedImage(context, image, Vector(), image->getSize(), targetPosition, targetSize,
		rotationAnchor, rotationAngle, modulationColor, blendType);
}

inline void PlatformAdapter::drawRotatedImage(DrawingContext & context, Image * image,
	Vector sourcePosition, Vector sourceSize, Vector targetPosition,
	Vector rotationAnchor, double rotationAngle,
	unsigned long modulationColor, BlendType blendType)
{
	drawRotatedImage(context, image, sourcePosition, sourceSize, targetPosition, sourceSize,
		rotationAnchor, rotationAngle, modulationColor, blendType);
}

inline void PlatformAdapter::drawTriangle(DrawingContext & context, Vector * vertices,
	unsigned long fillColor)
{
	drawTriangle(context, vertices, fillColor, fillColor);
}

inline unsigned long PlatformAdapter::getPixel(Image * image, Vector position)
{
	return getPixel(DrawingContext(image), position);
}


#endif
