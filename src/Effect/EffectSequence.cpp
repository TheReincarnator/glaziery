/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


EffectSequence::EffectSequence()
{
	ASSERTION_COBJECT(this);
	firstEffectStarted = false;
}

EffectSequence::~EffectSequence()
{
	ASSERTION_COBJECT(this);

	while (!effects.IsEmpty())
		Desktop::getInstance()->addDisposable(effects.UnlinkFirst());
}

void EffectSequence::appendEffect(Effect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Append(effect);
	effect->addReference();
}

bool EffectSequence::execute(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	if (effects.IsEmpty())
		return false;

	Effect * effect = effects.GetFirst();

	if (!effect->isEnded() && !effect->isExecuting())
	{
		effect->setExecuting(true);
		firstEffectStarted = true;

		if (effect->isCanceled())
		{
			effect->onCancel(context);
			effect->setEnded();
		}
		else if (!effect->execute(context, time))
			effect->setEnded();

		effect->setExecuting(false);
	}

	if (effect->isEnded())
	{
		Desktop::getInstance()->addDisposable(effects.UnlinkFirst());
		firstEffectStarted = false;
		return !effects.IsEmpty();
	}

	return true;
}

const ArrayList<Effect> & EffectSequence::getEffects()
{
	ASSERTION_COBJECT(this);
	return effects;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(EffectSequence, Effect);
#endif

void EffectSequence::insertEffect(int position, Effect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Insert(effect, position);
	effect->addReference();
}

void EffectSequence::onAdded()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<effects.GetCount(); i++)
		effects.Get(i)->onAdded();
}

void EffectSequence::onCancel(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (!effects.IsEmpty() && firstEffectStarted)
	{
		effects.GetFirst()->onCancel(context);
		firstEffectStarted = false;
	}
}

void EffectSequence::prependEffect(Effect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Prepend(effect);
	effect->addReference();
}
