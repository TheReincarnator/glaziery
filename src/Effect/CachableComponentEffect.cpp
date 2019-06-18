/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


CachableComponentEffect::CachableComponentEffect(long time, CachableComponent * cachableComponent)
	: ComponentEffect(time, cachableComponent)
{
	ASSERTION_COBJECT(this);

	focusAlphaEnd = 0;
	focusAlphaEndSet = false;
	focusAlphaStart = 0;
	focusAlphaStartSet = false;
	nonFocusAlphaEnd = 0;
	nonFocusAlphaEndSet = false;
	nonFocusAlphaStart = 0;
	nonFocusAlphaStartSet = false;
}

bool CachableComponentEffect::executeTimed(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	if (!ComponentEffect::executeTimed(context, time))
		return false;

	CachableComponent * component = getCachableComponent();
	if (component == NULL)
		return false;

	double progress = getProgress();

	if (focusAlphaStart != focusAlphaEnd)
		component->setFocusAlpha((unsigned char) (focusAlphaStart
			+ (focusAlphaEnd - focusAlphaStart) * progress));

	if (nonFocusAlphaStart != nonFocusAlphaEnd)
		component->setNonFocusAlpha((unsigned char) (nonFocusAlphaStart
			+ (nonFocusAlphaEnd - nonFocusAlphaStart) * progress));

	return true;
}

CachableComponent * CachableComponentEffect::getCachableComponent()
{
	ASSERTION_COBJECT(this);
	return (CachableComponent *) getComponent();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(CachableComponentEffect, ComponentEffect);
#endif

bool CachableComponentEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Ending cachable component effect");

	CachableComponent * component = getCachableComponent();
	if (component == NULL)
		return false;

	if (focusAlphaStart != focusAlphaEnd)
		component->setFocusAlpha(focusAlphaEnd);
	if (nonFocusAlphaStart != nonFocusAlphaEnd)
		component->setNonFocusAlpha(nonFocusAlphaEnd);

	ComponentEffect::onEnd(context);

	return false;
}

bool CachableComponentEffect::onStart(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (!ComponentEffect::onStart(context))
		return false;

	LOGPRINTF0(LOG_DEBUG, "Starting cachable component effect");

	CachableComponent * component = getCachableComponent();
	if (component == NULL)
		return false;

	if (!focusAlphaStartSet)
		focusAlphaStart = component->getFocusAlpha();
	if (!focusAlphaEndSet)
		focusAlphaEnd = component->getFocusAlpha();

	if (!nonFocusAlphaStartSet)
		nonFocusAlphaStart = component->getNonFocusAlpha();
	if (!nonFocusAlphaEndSet)
		nonFocusAlphaEnd = component->getNonFocusAlpha();

	return true;
}

void CachableComponentEffect::setFocusAlphaEnd(unsigned char focusAlphaEnd)
{
	ASSERTION_COBJECT(this);

	this->focusAlphaEnd = focusAlphaEnd;
	focusAlphaEndSet = true;
}

void CachableComponentEffect::setFocusAlphaStart(unsigned char focusAlphaStart)
{
	ASSERTION_COBJECT(this);

	this->focusAlphaStart = focusAlphaStart;
	focusAlphaStartSet = true;
}

void CachableComponentEffect::setNonFocusAlphaEnd(unsigned char nonFocusAlphaEnd)
{
	ASSERTION_COBJECT(this);

	this->nonFocusAlphaEnd = nonFocusAlphaEnd;
	nonFocusAlphaEndSet = true;
}

void CachableComponentEffect::setNonFocusAlphaStart(unsigned char nonFocusAlphaStart)
{
	ASSERTION_COBJECT(this);

	this->nonFocusAlphaStart = nonFocusAlphaStart;
	nonFocusAlphaStartSet = true;
}
