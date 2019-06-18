/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


IntCycleEffect::IntCycleEffect(long time, int * valuePointer, int maximum, Component * component)
	: TimedEffect(time)
{
	ASSERTION_COBJECT(this);

	this->component = component;
	componentDestroyListener = NULL;
	this->maximum = maximum;
	this->valuePointer = valuePointer;

	if (component != NULL)
	{
		// Create a listener that nulls the component property if the component dies.
		if ((componentDestroyListener = new ComponentDestroyListener(this)) == NULL)
			throw EOUTOFMEMORY;
		component->appendListener(componentDestroyListener);
	}
}

IntCycleEffect::~IntCycleEffect()
{
	ASSERTION_COBJECT(this);

	if (componentDestroyListener != NULL)
	{
		component->removeListener(componentDestroyListener);
		componentDestroyListener = NULL;
	}
}

Component * IntCycleEffect::getComponent()
{
	ASSERTION_COBJECT(this);
	return component;
}

int IntCycleEffect::getMaximum()
{
	ASSERTION_COBJECT(this);
	return maximum;
}

int * IntCycleEffect::getValuePointer()
{
	ASSERTION_COBJECT(this);
	return valuePointer;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(IntCycleEffect, TimedEffect);
#endif

void IntCycleEffect::onAdded()
{
	ASSERTION_COBJECT(this);

	if (component != NULL)
	{
		*valuePointer = NULL;
		component->invalidate();
	}
}

bool IntCycleEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (component != NULL)
	{
		*valuePointer = (*valuePointer + 1) % maximum;
		component->invalidate();
	}

	return true;
}


IntCycleEffect::ComponentDestroyListener::ComponentDestroyListener(Effect * effect)
{
	ASSERTION_COBJECT(this);
	this->effect = (IntCycleEffect *) effect;
}

IntCycleEffect::ComponentDestroyListener::~ComponentDestroyListener()
{
	ASSERTION_COBJECT(this);
	this->effect = effect;
}

void IntCycleEffect::ComponentDestroyListener::onDestroying(Component * component)
{
	ASSERTION_COBJECT(this);

	if (effect->getComponent() == component)
	{
		effect->cancel();
		effect->componentDestroyListener = NULL;
	}
}
