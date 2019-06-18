/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PointerEffect::PointerEffect(long time)
	: TimedEffect(time)
{
	ASSERTION_COBJECT(this);

	buttonEffect = BUTTONEFFECT_NONE;
	buttonOption1 = false;
	buttonOption2 = false;
	positionEndRelative = false;
	positionEndSet = false;
	positionStartRelative = false;
	positionStartSet = false;
}

bool PointerEffect::executeTimed(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	double progress = getProgress();

	if (positionStart != positionEnd)
		Desktop::getInstance()->movePointer(positionStart + (positionEnd - positionStart) * progress);

	return true;
}

PointerEffect::ButtonEffect PointerEffect::getButtonEffect()
{
	ASSERTION_COBJECT(this);
	return buttonEffect;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PointerEffect, TimedEffect);
#endif

bool PointerEffect::isButtonOption1()
{
	ASSERTION_COBJECT(this);
	return buttonOption1;
}

bool PointerEffect::isButtonOption2()
{
	ASSERTION_COBJECT(this);
	return buttonOption2;
}

bool PointerEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Ending component effect");

	Desktop * desktop = Desktop::getInstance();

	if (positionStart != positionEnd)
		desktop->movePointer(positionEnd);

	Desktop::getInstance()->getSkin()->addClickVisualizationEffect();

	if (buttonOption1)
		Desktop::getInstance()->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_OPTION1, false);
	if (buttonOption2)
		Desktop::getInstance()->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_OPTION2, false);

	InputEvent * event;
	switch (buttonEffect)
	{
		case BUTTONEFFECT_LEFTCLICK:
			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, true, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);

			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, false, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);
		break;

		case BUTTONEFFECT_DOUBLECLICK:
			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, true, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);

			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, false, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);

			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, true, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);

			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, false, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);
		break;

		case BUTTONEFFECT_RIGHTCLICK:
			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, true, false)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);

			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, false, false)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);
		break;

		case BUTTONEFFECT_DRAGDROP:
			if ((event = new PointerButtonEvent(buttonOption1, buttonOption2, false, true)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);
		break;
	}

	return false;
}

bool PointerEffect::onStart(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Starting pointer effect");

	Desktop * desktop = Desktop::getInstance();

	if (positionStartSet)
	{
		if (positionStartRelative)
			positionStart += desktop->getPointerPosition();
		desktop->movePointer(positionStart);
	}
	else
		positionStart = desktop->getPointerPosition();

	if (!positionEndSet)
		positionEnd = desktop->getPointerPosition();
	else if (positionEndRelative)
		positionEnd += desktop->getPointerPosition();

	if (buttonEffect == BUTTONEFFECT_DRAGDROP)
	{
		InputEvent * event;
		if ((event = new PointerButtonEvent(false, false, true, true)) == NULL)
			throw EOUTOFMEMORY;
		desktop->addEvent(event);
	}

	return true;
}

void PointerEffect::setButtonEffect(ButtonEffect buttonEffect)
{
	ASSERTION_COBJECT(this);
	this->buttonEffect = buttonEffect;
}

void PointerEffect::setButtonOption1(bool buttonOption1)
{
	ASSERTION_COBJECT(this);
	this->buttonOption1 = buttonOption1;
}

void PointerEffect::setButtonOption2(bool buttonOption2)
{
	ASSERTION_COBJECT(this);
	this->buttonOption2 = buttonOption2;
}

void PointerEffect::setPositionEnd(Vector positionEnd, bool relative)
{
	ASSERTION_COBJECT(this);

	this->positionEnd = positionEnd;
	positionEndRelative = relative;
	positionEndSet = true;
}

void PointerEffect::setPositionStart(Vector positionStart, bool relative)
{
	ASSERTION_COBJECT(this);

	this->positionStart = positionStart;
	positionStartRelative = relative;
	positionStartSet = true;
}
