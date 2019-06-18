/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DRAWINGCONTEXT_INL
#define __GLAZIERY_DRAWINGCONTEXT_INL


inline Vector DrawingContext::getClippingOffset()
{
	return clippingOffset;
}

inline Vector DrawingContext::getClippingSize()
{
	return clippingSize;
}

inline Vector DrawingContext::getDrawingOffset()
{
	return drawingOffset;
}

inline Image * DrawingContext::getTarget()
{
	return target;
}


#endif
