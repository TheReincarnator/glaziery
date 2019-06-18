/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ProgressBar::ProgressBar()
{
	ASSERTION_COBJECT(this);

	candyMode = false;
	candyModeDeferred = candyMode;
	current = 0;
	currentDeferred = current;
	end = 100;
	endDeferred = end;
	start = 0;
	startDeferred = start;

	setMinimumSize(Vector(64, 12));
	resizeInternal(Vector(128, 12), true);

	Desktop::getInstance()->getSkin()->setupProgressBar(this);
}

ProgressBar::~ProgressBar()
{
	ASSERTION_COBJECT(this);
}

void ProgressBar::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawProgressBar(context, this);
}

void ProgressBar::executeDeferrals()
{
	ASSERTION_COBJECT(this);

	Field::executeDeferrals();

	if (candyModeDeferred != candyMode)
		setCandyMode(candyModeDeferred);

	if (currentDeferred != current)
		setCurrent(currentDeferred);

	if (endDeferred != end)
		setEnd(endDeferred);

	if (startDeferred != start)
		setStart(startDeferred);
}

long ProgressBar::getConstrainedCurrent()
{
	ASSERTION_COBJECT(this);

	if (current < start)
		return start;
	else if (current > end)
		return end;
	else
		return current;
}

long ProgressBar::getCurrent()
{
	ASSERTION_COBJECT(this);
	return current;
}

long ProgressBar::getEnd()
{
	ASSERTION_COBJECT(this);
	return end;
}

long ProgressBar::getStart()
{
	ASSERTION_COBJECT(this);
	return start;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ProgressBar, Field);
#endif

void ProgressBar::increase(long amount)
{
	ASSERTION_COBJECT(this);

	setCandyMode(false);

	current += amount;
	invalidate();
}

void ProgressBar::increaseDeferred(long amount)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	currentDeferred += amount;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

bool ProgressBar::isCandyMode()
{
	ASSERTION_COBJECT(this);
	return candyMode;
}

void ProgressBar::setCandyMode(bool candyMode)
{
	ASSERTION_COBJECT(this);

	candyModeDeferred = candyMode;

	// Kill any pending current deferrals
	if (candyModeDeferred)
		currentDeferred = current;

	if (this->candyMode == candyMode)
		return;

	this->candyMode = candyMode;

	invalidate();

	Desktop::getInstance()->getSkin()->updateProgressBarCandyMode(this);
}

void ProgressBar::setCandyModeDeferred(bool candyMode)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	candyModeDeferred = candyMode;

	// Kill any pending current deferrals
	currentDeferred = current;

	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

void ProgressBar::setCurrent(long current)
{
	ASSERTION_COBJECT(this);

	setCandyMode(false);

	this->current = current;
	currentDeferred = current;

	invalidate();
}

void ProgressBar::setCurrentDeferred(long current)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	// Kill any pending candy mode deferrals
	candyModeDeferred = candyMode;

	currentDeferred = current;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

void ProgressBar::setCurrentToEnd()
{
	ASSERTION_COBJECT(this);
	setCurrent(end);
}

void ProgressBar::setCurrentToStart()
{
	ASSERTION_COBJECT(this);
	setCurrent(start);
}

void ProgressBar::setEnd(long end)
{
	ASSERTION_COBJECT(this);

	this->end = end;
	endDeferred = end;

	invalidate();
}

void ProgressBar::setEndDeferred(long end)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	endDeferred = end;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

void ProgressBar::setStart(long start)
{
	ASSERTION_COBJECT(this);

	this->start = start;
	startDeferred = start;

	invalidate();
}

void ProgressBar::setStartDeferred(long start)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	startDeferred = start;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

String ProgressBar::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("ProgressBar(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}
