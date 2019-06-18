/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Effect::Effect()
{
	ASSERTION_COBJECT(this);

	canceled = false;
	ended = false;
	executing = false;

	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		LOGPRINTF1(LOG_DEBUG, "Created '%s'", GetRuntimeClass()->m_lpszClassName);
	#endif
}

Effect::~Effect()
{
	ASSERTION_COBJECT(this);

	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		LOGPRINTF1(LOG_DEBUG, "Destroying '%s'", GetRuntimeClass()->m_lpszClassName);
	#endif
}

void Effect::cancel()
{
	ASSERTION_COBJECT(this);
	canceled = true;
}

void Effect::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Effect, SharedObject);
#endif

bool Effect::isCanceled()
{
	ASSERTION_COBJECT(this);
	return canceled;
}

bool Effect::isEnded()
{
	ASSERTION_COBJECT(this);
	return ended;
}

bool Effect::isExecuting()
{
	ASSERTION_COBJECT(this);
	return executing;
}

void Effect::onAdded()
{
	ASSERTION_COBJECT(this);
}

void Effect::onCancel(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
}

void Effect::setEnded()
{
	ASSERTION_COBJECT(this);
	ended = true;
}

void Effect::setExecuting(bool executing)
{
	ASSERTION_COBJECT(this);
	this->executing = executing;
}

void Effect::waitFor()
{
	ASSERTION_COBJECT(this);

	while (!isEnded())
		Desktop::getInstance()->runOnce();
}
