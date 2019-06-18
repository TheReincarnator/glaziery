/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ComponentEffect::ComponentEffect(long time, Component * component)
	: TimedEffect(time)
{
	ASSERTION_COBJECT(this);

	this->component = component;

	destroying = false;
	hiding = false;
	positionEndSet = false;
	positionStartSet = false;
	sizeEndSet = false;
	sizeStartSet = false;
	showing = false;
}

bool ComponentEffect::executeTimed(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	if (component == NULL)
		return false;

	double progress = getProgress();

	if (positionStart != positionEnd)
		component->moveTo(positionStart + (positionEnd - positionStart) * progress);

	if (sizeStart != sizeEnd)
		component->resize(sizeStart + (sizeEnd - sizeStart) * progress);

	return true;
}

Component * ComponentEffect::getComponent()
{
	ASSERTION_COBJECT(this);
	return component;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ComponentEffect, TimedEffect);
#endif

bool ComponentEffect::isDestroying()
{
	ASSERTION_COBJECT(this);
	return destroying;
}

bool ComponentEffect::isHiding()
{
	ASSERTION_COBJECT(this);
	return hiding;
}

bool ComponentEffect::isShowing()
{
	ASSERTION_COBJECT(this);
	return showing;
}

void ComponentEffect::onAdded()
{
	ASSERTION_COBJECT(this);
	component->addEffect(this);
}

void ComponentEffect::onCancel(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (component != NULL)
	{
		component->removeEffect(this);
		component = NULL;
	}
}

void ComponentEffect::onComponentDestroying()
{
	ASSERTION_COBJECT(this);

	component = NULL;
	cancel();
}

bool ComponentEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Ending component effect");

	if (component == NULL)
		return false;

	if (positionStart != positionEnd)
		component->moveTo(positionEnd);
	if (sizeStart != sizeEnd)
		component->resize(sizeEnd);

	if (hiding)
		component->hide();

	if (destroying)
		component->destroy();

	component->removeEffect(this);
	component = NULL;

	return false;
}

bool ComponentEffect::onStart(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Starting component effect");

	if (component == NULL)
		return false;

	if (showing)
		component->show();

	if (positionStartSet)
	{
		if (positionStartRelative)
			positionStart += component->getPosition();
		component->moveTo(positionStart);
	}
	else
		positionStart = component->getPosition();

	if (!positionEndSet)
		positionEnd = component->getPosition();
	else if (positionEndRelative)
		positionEnd += component->getPosition();

	if (sizeStartSet)
	{
		if (sizeStartRelative)
			sizeStart += component->getSize();
		component->resize(sizeStart);
	}
	else
		sizeStart = component->getSize();

	if (!sizeEndSet)
		sizeEnd = component->getSize();
	else if (sizeEndRelative)
		sizeEnd += component->getSize();

	return true;
}

void ComponentEffect::setDestroying(bool destroying)
{
	ASSERTION_COBJECT(this);
	this->destroying = destroying;
}

void ComponentEffect::setHiding(bool hiding)
{
	ASSERTION_COBJECT(this);
	this->hiding = hiding;
}

void ComponentEffect::setPositionEnd(Vector positionEnd, bool relative)
{
	ASSERTION_COBJECT(this);

	this->positionEnd = positionEnd;
	positionEndRelative = relative;
	positionEndSet = true;
}

void ComponentEffect::setPositionStart(Vector positionStart, bool relative)
{
	ASSERTION_COBJECT(this);

	this->positionStart = positionStart;
	positionStartRelative = relative;
	positionStartSet = true;
}

void ComponentEffect::setShowing(bool showing)
{
	ASSERTION_COBJECT(this);
	this->showing = showing;
}

void ComponentEffect::setSizeEnd(Vector sizeEnd, bool relative)
{
	ASSERTION_COBJECT(this);

	this->sizeEnd = sizeEnd;
	sizeEndRelative = relative;
	sizeEndSet = true;
}

void ComponentEffect::setSizeStart(Vector sizeStart, bool relative)
{
	ASSERTION_COBJECT(this);

	this->sizeStart = sizeStart;
	sizeStartRelative = relative;
	sizeStartSet = true;
}
