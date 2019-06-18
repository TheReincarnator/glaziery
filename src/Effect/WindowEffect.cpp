/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


WindowEffect::WindowEffect(long time, Window * window)
	: CachableComponentEffect(time, window)
{
	ASSERTION_COBJECT(this);

	bringingToFront = false;
	closing = false;
	focusingType = FOCUSING_NEVER;
	maximizing = false;
	restoring = false;
	sendingToBack = false;
}

WindowEffect::FocusingType WindowEffect::getFocusingType()
{
	ASSERTION_COBJECT(this);
	return focusingType;
}

Window * WindowEffect::getWindow()
{
	ASSERTION_COBJECT(this);
	return (Window *) getComponent();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(WindowEffect, CachableComponentEffect);
#endif

bool WindowEffect::isBringingToFront()
{
	ASSERTION_COBJECT(this);
	return bringingToFront;
}

bool WindowEffect::isClosing()
{
	ASSERTION_COBJECT(this);
	return closing;
}

bool WindowEffect::isMaximizing()
{
	ASSERTION_COBJECT(this);
	return maximizing;
}

bool WindowEffect::isRestoring()
{
	ASSERTION_COBJECT(this);
	return restoring;
}

bool WindowEffect::isSendingToBack()
{
	ASSERTION_COBJECT(this);
	return sendingToBack;
}

bool WindowEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Ending window effect");

	Window * window = getWindow();
	if (window == NULL)
		return false;

	if (bringingToFront)
		window->bringToFront();

	if (closing)
	{
		window->close();
		setDestroying(false);
	}

	switch (focusingType)
	{
		case FOCUSING_ALWAYS:
			Desktop::getInstance()->setFocusWindow(window);
		break;

		case FOCUSING_IF_NONE:
			Desktop * desktop = Desktop::getInstance();
			if (desktop->getFocusWindowOrPopup() == NULL)
				Desktop::getInstance()->setFocusWindow(window);
		break;
	}

	if (maximizing)
		window->maximize();
	if (restoring)
		window->restore();
	if (sendingToBack)
		window->sendToBack();

	CachableComponentEffect::onEnd(context);

	return false;
}

bool WindowEffect::onStart(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (!CachableComponentEffect::onStart(context))
		return false;

	LOGPRINTF0(LOG_DEBUG, "Starting window effect");

	return true;
}

void WindowEffect::setBringingToFront(bool bringingToFront)
{
	ASSERTION_COBJECT(this);
	this->bringingToFront = bringingToFront;
}

void WindowEffect::setClosing(bool closing)
{
	ASSERTION_COBJECT(this);
	this->closing = closing;
}

void WindowEffect::setFocusingType(WindowEffect::FocusingType focusingType)
{
	ASSERTION_COBJECT(this);
	this->focusingType = focusingType;
}

void WindowEffect::setMaximizing(bool maximizing)
{
	ASSERTION_COBJECT(this);
	this->maximizing = maximizing;
}

void WindowEffect::setRestoring(bool restoring)
{
	ASSERTION_COBJECT(this);
	this->restoring = restoring;
}

void WindowEffect::setSendingToBack(bool sendingToBack)
{
	ASSERTION_COBJECT(this);
	this->sendingToBack = sendingToBack;
}
