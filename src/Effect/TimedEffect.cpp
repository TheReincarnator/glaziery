/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TimedEffect::TimedEffect(long time)
{
	ASSERTION_COBJECT(this);

	remainingTime = -1;
	timeCurve = NULL;
	totalTime = time >= 0 ? time : 0;
}

TimedEffect::~TimedEffect()
{
	ASSERTION_COBJECT(this);

	if (timeCurve != NULL) {delete timeCurve; timeCurve = NULL;}
}

bool TimedEffect::execute(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	if (remainingTime < 0)
	{
		remainingTime = totalTime;
		if (!onStart(context))
			return false;

		// Zero the time of the first frame to zero, as it may contain backlog of previous events
		time = 0;
	}

	if (remainingTime <= time)
	{
		int rest = time - remainingTime;
		if (!executeTimed(context, remainingTime))
			return false;

		if (!onEnd(context))
			return false;

		remainingTime = totalTime;
		if (!onStart(context))
			return false;

		if (rest < totalTime)
		{
			if (!executeTimed(context, rest))
				return false;
		}
		else
		{
			if (!executeTimed(context, totalTime))
				return false;

			if (!onEnd(context))
				return false;
		}
	}
	else
	{
		remainingTime -= time;
		if (!executeTimed(context, time))
			return false;
	}

	return true;
}

bool TimedEffect::executeTimed(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);
	return true;
}

long TimedEffect::getElapsedTime()
{
	ASSERTION_COBJECT(this);
	return totalTime - remainingTime;
}

double TimedEffect::getProgress()
{
	ASSERTION_COBJECT(this);

	double progress = (totalTime - remainingTime) * 1.0 / totalTime;
	if (timeCurve != NULL)
		progress = timeCurve->transform(progress);

	if (progress < 0.0)
		progress = 0.0;
	else if (progress > 1.0)
		progress = 1.0;

	return progress;
}

long TimedEffect::getRemainingTime()
{
	ASSERTION_COBJECT(this);
	return remainingTime;
}

TimeCurve * TimedEffect::getTimeCurve()
{
	ASSERTION_COBJECT(this);
	return timeCurve;
}

long TimedEffect::getTotalTime()
{
	ASSERTION_COBJECT(this);
	return totalTime;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TimedEffect, Effect);
#endif

bool TimedEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	return false;
}

bool TimedEffect::onStart(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	return true;
}

void TimedEffect::setProgress(double progress)
{
	ASSERTION_COBJECT(this);

	if (progress < 0.0)
		progress = 0.0;
	else if (progress > 1.0)
		progress = 1.0;

	if (timeCurve != NULL)
		progress = timeCurve->untransform(progress);

	remainingTime = (long) (totalTime - progress * totalTime);
}

void TimedEffect::setTimeCurve(TimeCurve * timeCurve)
{
	ASSERTION_COBJECT(this);

	if (this->timeCurve != NULL)
		delete this->timeCurve;
	this->timeCurve = timeCurve;
}

void TimedEffect::setTimeCurveToAcceleration()
{
	ASSERTION_COBJECT(this);

	TimeCurve * timeCurve;
	if ((timeCurve = new AccelerationTimeCurve) == NULL)
		throw EOUTOFMEMORY;
	setTimeCurve(timeCurve);
}

void TimedEffect::setTotalTime(long totalTime)
{
	ASSERTION_COBJECT(this);
	this->totalTime = totalTime;
}
